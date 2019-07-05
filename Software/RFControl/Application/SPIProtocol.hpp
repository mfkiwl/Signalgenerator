#pragma once

#include <stdint.h>

namespace Protocol {

using FrontToRF = struct fronttorf {
	uint64_t frequency;
	int16_t dbm;
	struct {
		uint16_t UseIntRef :1;
		uint16_t reserved :15;
	} Status;
	uint8_t reserved[4];
	uint16_t modulation_registers[8];
} __attribute__((packed, aligned(8)));

static constexpr uint32_t MagicConstant = 0xAE795C0D;

using RFToFront = struct rftofront {
	struct {
		uint16_t IntRefON :1;
		uint16_t MainPLLON :1;
		uint16_t MainPLLUnlocked :1;
		uint16_t HeterodynePLLON :1;
		uint16_t HeterodynePLLUnlock :1;
		uint16_t IQModEnabled :1;
		uint16_t Filter :3;
		uint16_t n15dbm1 :1;
		uint16_t n15dbm2 :1;
		uint16_t n15dbm3 :1;
		uint16_t AmplitudeUnlevel :1;
		uint16_t reserved :3;
	} Status;
	uint16_t reserved1;
	uint32_t MagicConstant;
	uint8_t reserved2[24];
} __attribute__((packed, aligned(8)));;

}
