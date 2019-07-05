#include "app.h"

#include "RF.hpp"
#include "SPIProtocol.hpp"
#include <string.h>
#include "stm32f0xx_hal.h"
#include "System/log.h"
#include "Drivers/fpga.hpp"

static Protocol::RFToFront send;
static Protocol::FrontToRF spi_new, recv;

static volatile bool new_data = false;

extern SPI_HandleTypeDef hspi2;

static_assert(sizeof(Protocol::RFToFront) == 32);
static_assert(sizeof(Protocol::FrontToRF) == 32);

void app(void) {
	log_init();
	send.MagicConstant = Protocol::MagicConstant;
	Protocol::FrontToRF spi_current;
	memset(&spi_current, 0, sizeof(spi_current));
	HAL_Delay(3000);
	RF::Init(&send);

	HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t*) &send, (uint8_t*) &recv,
			sizeof(spi_new));

	while (1) {
		while (!new_data)
			;
		new_data = false;
		HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t*) &send, (uint8_t*) &recv,
					sizeof(spi_new));
//		LOG(Log_System, LevelInfo,
//				"SPI data, freq: %lu, dbm: %u, intref: 0x%02x",
//				(uint32_t ) spi_new.frequency, spi_new.dbm,
//				(uint8_t ) spi_new.Status.UseIntRef);
		if (spi_new.frequency != spi_current.frequency
				|| spi_new.dbm != spi_current.dbm) {
			RF::Configure(spi_new.frequency, spi_new.dbm);
		}
		if (spi_new.Status.UseIntRef != spi_current.Status.UseIntRef) {
			RF::InternalReference(spi_new.Status.UseIntRef);
		}
//		LOG(Log_System, LevelInfo,
//				"SPI data, Mod0: 0x%04x, Mod1: 0x%04x, Mod2: 0x%04x",
//				spi_new.modulation_registers[0],
//				spi_new.modulation_registers[1],
//				spi_new.modulation_registers[2]);
		if (memcmp(spi_new.modulation_registers,
				spi_current.modulation_registers,
				sizeof(spi_new.modulation_registers))) {
			// Update registers on FPGA
			for (uint8_t i = 0; i < 8; i++) {
				FPGA::WriteReg((FPGA::Reg) (i + (int) FPGA::Reg::MOD_DATA_L),
						spi_new.modulation_registers[i]);
			}
		}
		spi_current = spi_new;
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
	if (hspi == &hspi2) {
		// got new data from frontpanel
		spi_new = recv;
		new_data = true;
	}
}
