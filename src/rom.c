/*
WolfBox Fantasy Workstation
Copyright 2022-2022 Talon396

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "icebus.h"
#include "rom.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define FIRMWARE_SIZE (128 * 1024)

uint8_t Firmware[FIRMWARE_SIZE];

int ROMRead(uint64_t addr, uint64_t len, void *buf) {
    if (addr+len > FIRMWARE_SIZE) {
		return 1;
    }
    memcpy((uint8_t*)buf, (uint8_t*)&Firmware[addr], len);
    return 0;
}

int ROMWrite(uint64_t addr, uint64_t len, void *buf) {
    return 0;
}

bool ROMLoad(char *name) {
    FILE* rom = fopen(name, "r");
    if(!rom) {
        fprintf(stderr, "\x1b[31mUnable to open ROM file \"%s\"\x1b[0m\n", name);
        return false;
    }
    fread(&Firmware, FIRMWARE_SIZE, 1, rom);
    fclose(rom);
    return true;
}

bool ROMInit() {
    IceBusBanks[1].Used = true;
    IceBusBanks[1].Write = ROMWrite;
	IceBusBanks[1].Read = ROMRead;
    return ROMLoad("firmware.bin");
}