#ifndef SCCB_H
#define SCCB_H

#include "common.h"


#define I2S_ADDRESS 0x30

Result<Unit> get_error_message(u8 result) {
	switch(result) {
		case 0:	return Result<Unit>::ok(UNIT);
		case 2:	return Result<Unit>::err("NACK response from device");
		case 4:	return Result<Unit>::err("Connection error");
		case 5:	return Result<Unit>::err("FIFO error");
		default: return Result<Unit>::err("Unknown error");
	}
}

Result<Unit> sccb_write(u8 address, u8 reg, u8 value) {
	Wire2.beginTransmission(address);
	Wire2.write(reg);
	Wire2.write(value);
	return get_error_message(Wire2.endTransmission());
}


#define RETRIES 8

Result<Unit> sccb_write_with_retries(u8 address, u8 reg, u8 value) {
	const u32 retry_timeouts[RETRIES] = {0, 0, 0, 1, 5, 10, 50, 100};
	
	let result = sccb_write(address, reg, value);
	if (result.type == ResultType::Ok) return Result<Unit>::ok(UNIT);
	
	for (let i = 0; i < RETRIES; i++) {
		delay(retry_timeouts[i]);
		result = sccb_write(address, reg, value);
		if (result.type == ResultType::Ok) return result;
	}
	
	return result;
}



Result<u8> sccb_read(u8 address, u8 reg) {
	Wire2.beginTransmission(address);
	Wire2.write(reg);
	let result = get_error_message(Wire.endTransmission());
	
	Wire2.requestFrom((int) address, 1);
	let value = Wire2.read();
	
	switch (result.type) {
		case ResultType::Ok : return Result<u8>::ok(value);
		case ResultType::Err: return Result<u8>::err(result.value.err_value);
		default: return Result<u8>::err("Result type error");
	}
}




#endif