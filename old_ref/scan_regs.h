#ifndef SCAN_REGS_H
#define SCAN_REGS_H

#include <stdio.h>
#include "common.h"
#include "sccb.h"


static u8 regs[512] = { 0 };

void print_regs() {
	printf("\t");
	for (int j = 0; j < 16; j++) printf("%x\t", j);
	printf("\n");
	for (int j = 0; j < 16; j++) printf("--------");
	printf("\n ");
	
	for (int i = 0; i < 512; i += 16) {
		if (!(i >> 8)) printf(" ");
		printf("%x |\t", i);
		for (int j = i; j < i+16; j++) {
			printf("%x\t", regs[j]);
		}
		printf("\n");
	}
}

void read_regs() {
	sccb_write(0xff, 0).unwrap();
	for (int i = 0; i < 256; i++) {
		let r = sccb_read(i);
		regs[i] = r.value.ok_value;
		if (r.type == ResultType::Err) printf("H %x\n", i);
	}
	sccb_write(0xff, 1).unwrap();
	for (int i = 0; i < 256; i++) {
		regs[i + 256] = sccb_read(i).unwrap();
		// printf("H2 %x\n", i);
	}
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


#endif