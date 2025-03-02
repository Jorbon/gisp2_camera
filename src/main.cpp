#include <Arduino.h>
#include <Wire.h>
#include <cstdlib>

#include "common.h"
#include "sccb.h"
#include "ov2640_regs.h"



#define RESET 10
#define POWER_DOWN 11

#define VSYNC 20
#define HREF 21
#define PCLK 22
#define XCLK 23


void write_regs(RegisterEntry const* regs) {
	while (true) {
		let reg = *regs;
		if (reg.address == 0xff && reg.value == 0xff) break;
		printf("Writing %x to %x\n", reg.value, reg.address);
		sccb_write(reg.address, reg.value).unwrap();
		
		regs++;
	}
}



static u8 regs[512] = { 0 };

void print_regs() {
	for (int i = 0; i < 512; i += 16) {
		printf("%x:\t", i);
		for (int j = i; j < i+16; j++) {
			printf("%x\t", regs[j]);
		}
		printf("\n");
	}
}

void read_regs() {
	u8 value = sccb_read(0xff).unwrap();
	sccb_write(0xff, value & 0b11111110).unwrap();
	for (int i = 0; i < 256; i++) regs[i] = sccb_read(i).unwrap();
	sccb_write(0xff, value | 0b00000001).unwrap();
	for (int i = 0; i < 256; i++) regs[i + 256] = sccb_read(i).unwrap();
}

void report_reg_diffs() {
	let no_changes = true;
	u8 value = sccb_read(0xff).unwrap();
	sccb_write(0xff, value & 0b11111110).unwrap();
	for (int i = 0; i < 256; i++) {
		u8 new_value = sccb_read(i).unwrap();
		if (regs[i] != new_value) {
			printf("Register %x changed: %x -> %x\n", i, regs[i], new_value);
			no_changes = false;
		}
	}
	sccb_write(0xff, value | 0b00000001).unwrap();
	for (int i = 0; i < 256; i++) {
		u8 new_value = sccb_read(i).unwrap();
		if (regs[i+256] != new_value) {
			printf("Register %x changed: %x -> %x\n", i+256, regs[i+256], new_value);
			no_changes = false;
		}
	}
	if (no_changes) printf("No registers changed.\n");
}

void read_regs_report_diffs() {
	let no_changes = true;
	u8 value = sccb_read(0xff).unwrap();
	sccb_write(0xff, value & 0b11111110).unwrap();
	for (int i = 0; i < 256; i++) {
		u8 new_value = sccb_read(i).unwrap();
		if (regs[i] != new_value) {
			printf("Register %x updated: %x -> %x\n", i, regs[i], new_value);
			regs[i] = new_value;
			no_changes = false;
		}
	}
	sccb_write(0xff, value | 0b00000001).unwrap();
	for (int i = 0; i < 256; i++) {
		u8 new_value = sccb_read(i).unwrap();
		if (regs[i+256] != new_value) {
			printf("Register %x updated: %x -> %x\n", i+256, regs[i+256], new_value);
			regs[i+256] = new_value;
			no_changes = false;
		}
	}
	if (no_changes) printf("No registers updated.\n");
}



void setup() {
	
	Wire.begin();
	Serial.begin(115200);
	
	pinMode(RESET, OUTPUT);
	pinMode(POWER_DOWN, OUTPUT);
	pinMode(XCLK, OUTPUT);
	
	pinMode(VSYNC, INPUT);
	pinMode(HREF, INPUT);
	pinMode(PCLK, INPUT);
	for (u8 i = 0; i < 10; i++) pinMode(i, INPUT);
	
	digitalWrite(RESET, HIGH);
	
	// digitalWrite(RESET, LOW);
	
	analogWriteFrequency(XCLK, 24e6);
	analogWrite(XCLK, 128);
	
	delay(10);
	
	sccb_write(0xff, 1).unwrap();
	sccb_write(0x12, 0x80).unwrap();
	
	delay(50);
	
	while (!Serial) delay(100);
	printf("Hello!\n");
	
	
	write_regs(OV2640_QVGA);
	
	
}


static u8 past = 0;
static u32 count = 0;
static u32 i = 0;

static int buf[256] = { -1 };

void loop() {
	
	// u8 value = digitalReadFast(PCLK);
	// if (value == past) count++;
	// else {
	// 	buf[i] = count;
	// 	i++;
	// 	if (i == 256) {
	// 		for (u32 i = 0; i < 256; i++) printf("%d ", buf[i]);
	// 		printf("\n");
	// 		exit(0);
	// 	}
		
	// 	count = 0;
	// 	past = value;
	// }
	
	u8 data[] = {
		digitalReadFast(0),
		digitalReadFast(1),
		digitalReadFast(2),
		digitalReadFast(3),
		digitalReadFast(4),
		digitalReadFast(5),
		digitalReadFast(6),
		digitalReadFast(7),
		digitalReadFast(8),
		digitalReadFast(9),
	};
	
	let value = 0;
	for (u8 i = 0; i < 10; i++) {
		value |= data[i] << (9 - i);
	}
	printf("%d\n", value);
	
	
	// delayMicroseconds(1);
}


// Nonzero regs: 0x5, 0x28, 0x29, 0x2b, 0x33, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47, 0x48, 0x49, 0x4a, 0x51, 0x52, 0x55, 0x5a, 0x5b, 0x60, 0x76, 0x78, 0x79, 0x86, 0x87, 0x88, 0x89, 0x8b, 0x91, 0x93, 0x97, 0xa7, 0xb0, 0xb1, 0xb2, 0xb5, 0xb8, 0xb9, 0xba, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc9, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd2, 0xd3, 0xd5, 0xd8, 0xd9, 0xdd, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3, 0xe5, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xf0, 0xf1, 0xf2, 0xf3, 0xf7, 0xf9, 0xfe, 0x103, 0x104, 0x108, 0x10a, 0x10b, 0x10c, 0x10d, 0x10e, 0x10f, 0x110, 0x113, 0x114, 0x117, 0x118, 0x119, 0x11a, 0x11c, 0x11d, 0x11e, 0x124, 0x125, 0x126, 0x127, 0x128, 0x12f, 0x130, 0x131, 0x132, 0x133, 0x134, 0x135, 0x137, 0x138, 0x139, 0x13a, 0x13b, 0x13c, 0x13d, 0x13e, 0x13f, 0x142, 0x14a, 0x14b, 0x14c, 0x14d, 0x14f, 0x150, 0x151, 0x152, 0x153, 0x154, 0x155, 0x156, 0x15d, 0x15e, 0x15f, 0x160, 0x161, 0x162, 0x16c, 0x16f, 0x170, 0x171, 0x172, 0x173, 0x174, 0x175, 0x176, 0x177, 0x178, 0x179, 0x17a, 0x17b, 0x17c, 0x17d

// Regs that change on their own:
// 0x93: 9
// 0xc9: 8
// 0xfa: 115
// 0xfc: 114
// 0x100: 13
// 0x104: 6
// 0x110: 7
// 0x12f: 5
// 0x145: 5
// 0x164: 7
// 0x165: 7
// 0x166: 5
// 0x167: 5
// 0x168: 7
// 0x169: 7
// 0x16a: 5
// 0x16b: 5
// 0x17f: 10



