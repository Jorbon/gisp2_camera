#ifndef SCCB_H
#define SCCB_H

#include "common.h"


#define I2S_ADDRESS 0x30

Result<Unit> get_error_message(u8 result) {
	switch(result) {
		case 0:	return Result<Unit>::ok(UNIT);
		case 2:	return Result<Unit>::err("NACK response from device\n");
		case 4:	return Result<Unit>::err("Connection error\n");
		case 5:	return Result<Unit>::err("FIFO error\n");
		default: return Result<Unit>::err("Unknown error\n");
	}
}

Result<Unit, const char*> sccb_write(u8 address, u8 reg, u8 value) {
	Wire2.beginTransmission(address);
	Wire2.write(reg);
	Wire2.write(value);
	return get_error_message(Wire2.endTransmission());
}

Result<u8, const char*> sccb_read(u8 address, u8 reg) {
	Wire2.beginTransmission(address);
	Wire2.write(reg);
	let result = get_error_message(Wire.endTransmission());
	
	Wire2.requestFrom((int) address, 1);
	let value = Wire2.read();
	
	switch (result.type) {
		case ResultType::Ok : return Result<u8>::ok(value);
		case ResultType::Err: return Result<u8>::err(result.value.err_value);
	}
	
	fprintf(stderr, "Result type error\n");
	exit(1);
}




#endif