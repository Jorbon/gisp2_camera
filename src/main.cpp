#include <Arduino.h>
#include <Wire.h>
#include <cstdlib>
#include <DMAChannel.h>

#include "common.h"
#include "sccb.h"
#include "reg_lists.h"


#define CAM_ID(n) (0x30|(n))

#define N_CAMERAS 3


#define SERIAL_RX_PIN  0
#define SERIAL_TX_PIN  1
#define I2C_CLOCK_PIN 24
#define I2C_DATA_PIN  25

const u8 DATA_PINS[8 * 3] = {
	37, 36,  7,  8, 13, 11, 12, 10, // ((GPIO7_PSR >> 12) & 0b11110000) | (GPIO7_PSR & 0b1111)
	22, 23, 20, 21, 38, 39, 26, 27, // GPIO6_PSR >> 24
	19, 18, 14, 15, 40, 41, 17, 16, // GPIO6_PSR >> 16
};


const u8 PCLK_PINS [3] = { 34, 32,  5 };
const u8 XCLK_PINS [3] = { 33, 28, 29 };
const u8 RESET_PINS[3] = { 30,  6,  9 };
const u8 HREF_PINS [3] = { 35, 31,  4 };




#define XCLK_FREQUENCY 24e6


const RegisterEntry* size_reg_lists[] = {
	OV2640_160x120_JPEG,	// 0 => 2.560us
	OV2640_320x240_JPEG,	// 1 => 2.560us
	OV2640_640x480_JPEG,	// 2 => 1.707us
	OV2640_800x600_JPEG,	// 3 => 853ns
	OV2640_1024x768_JPEG,	// 4 => 853ns
	OV2640_1600x1200_JPEG,	// 5 => 853ns
	OV2640_352x288_JPEG,	// 6 => 2.560us
	OV2640_1280x1024_JPEG,	// 7 => 853ns
	
	// OV2640_176x144_JPEG,
};

// max interrupt speed: 5x 2.560us


Result<Unit> init_camera(u8 i, u8 resolution, u8 clock_divisor) {
	
	// Activate the camera & reset
	digitalWrite(RESET_PINS[i], HIGH);
	delay(10);
	let result = sccb_write_with_retries(CAM_ID(0), 0xff, 1); if (result.type == ResultType::Err) return result;
	result = sccb_write_with_retries(CAM_ID(0), 0x12, 0x80); if (result.type == ResultType::Err) return result;
	delay(10);
	
	u8 address = CAM_ID(i + 1);
	
	// Assign the camera to the specific address
	result = sccb_write_with_retries(CAM_ID(0), 0xff, 0); if (result.type == ResultType::Err) return result;
	result = sccb_write_with_retries(CAM_ID(0), 0xf7, address << 1); if (result.type == ResultType::Err) return result;
	
	
	result = write_regs(address, OV2640_JPEG_INIT); if (result.type == ResultType::Err) return result;
	result = write_regs(address, OV2640_YUV422); if (result.type == ResultType::Err) return result;
	result = write_regs(address, OV2640_JPEG); if (result.type == ResultType::Err) return result;
	result = sccb_write_with_retries(address, 0xff, 0x01); if (result.type == ResultType::Err) return result;
	result = sccb_write_with_retries(address, 0x15, 0x00); if (result.type == ResultType::Err) return result;
	
	result = write_regs(address, size_reg_lists[resolution]); if (result.type == ResultType::Err) return result;
	
	result = sccb_write_with_retries(address, 0xff, 1); if (result.type == ResultType::Err) return result;
	result = sccb_write_with_retries(address, CLKRC, clock_divisor); if (result.type == ResultType::Err) return result;
	
	return Result<Unit>::ok(UNIT);
}








// unsigned int last = 0;

// u8 last_byte = 0;
// unsigned int repeat_count = 0;

// u8 c = '0';

	// if (value >= 33 && value <= 126) printf("%c ", value);
	// else printf("%02x ", value);
	
	// unsigned int t = ARM_DWT_CYCCNT;
	// printf("%d ", (t - last) / 25);
	// last = t;
	
	// Serial.write(c);
	// if (c++ == '9') c = '0';
	
	// if (value == last_byte) {
	// 	repeat_count += 1;
	// } else {
	// 	printf("%u ", repeat_count);
	// 	last_byte = value;
	// 	repeat_count = 1;
	// }



void on_pclk_0();
void on_pclk_1();
void on_pclk_2();


bool previous_ff = false;
bool frame_started = false;



void on_pclk_0() {
	if (digitalReadFast(HREF_PINS[0]) == 0) return;
	
	u32 reg = GPIO7_PSR;
	u8 value = ((reg >> 12) & 0b11110000) | (reg & 0b1111);
	Serial.write(value);
	
	switch (value) {
		case 0xff:
			previous_ff = true;
			break;
		case 0xd8:
			if (previous_ff) {
				frame_started = true;
				previous_ff = false;
			}
			break;
		case 0xd9:
			if (previous_ff) {
				if (frame_started) {
					detachInterrupt(digitalPinToInterrupt(PCLK_PINS[0]));
					frame_started = false;
					Serial.write('1');
					attachInterrupt(digitalPinToInterrupt(PCLK_PINS[1]), on_pclk_1, FALLING);
				}
				previous_ff = false;
			}
			break;
		default:
			previous_ff = false;
			break;
	}
}

void on_pclk_1() {
	if (digitalReadFast(HREF_PINS[1]) == 0) return;
	
	u32 reg = GPIO6_PSR;
	u8 value = reg >> 24;
	Serial.write(value);
	
	switch (value) {
		case 0xff:
			previous_ff = true;
			break;
		case 0xd8:
			if (previous_ff) {
				frame_started = true;
				previous_ff = false;
			}
			break;
		case 0xd9:
			if (previous_ff) {
				if (frame_started) {
					detachInterrupt(digitalPinToInterrupt(PCLK_PINS[1]));
					frame_started = false;
					Serial.write('2');
					attachInterrupt(digitalPinToInterrupt(PCLK_PINS[2]), on_pclk_2, FALLING);
				}
				previous_ff = false;
			}
			break;
		default:
			previous_ff = false;
			break;
	}
}

void on_pclk_2() {
	if (digitalReadFast(HREF_PINS[2]) == 0) return;
	
	u32 reg = GPIO6_PSR;
	u8 value = reg >> 16;
	Serial.write(value);
	
	switch (value) {
		case 0xff:
			previous_ff = true;
			break;
		case 0xd8:
			if (previous_ff) {
				frame_started = true;
				previous_ff = false;
			}
			break;
		case 0xd9:
			if (previous_ff) {
				if (frame_started) {
					detachInterrupt(digitalPinToInterrupt(PCLK_PINS[2]));
					frame_started = false;
					Serial.write('0');
					attachInterrupt(digitalPinToInterrupt(PCLK_PINS[0]), on_pclk_0, FALLING);
				}
				previous_ff = false;
			}
			break;
		default:
			previous_ff = false;
			break;
	}
}







void configure_cameras(u8 resolution, u8 clock_divisor) {
	for (u8 i = 0; i < N_CAMERAS; i++) {
		// detachInterrupt(digitalPinToInterrupt(VSYNC_PINS[i]));
		detachInterrupt(digitalPinToInterrupt(PCLK_PINS[i]));
	}
	
	for (u8 i = 0; i < N_CAMERAS; i++) digitalWrite(RESET_PINS[i], LOW);
	delay(10);
	for (u8 i = 0; i < N_CAMERAS; i++) {
		let result = init_camera(i, resolution, clock_divisor);
		if (result.type == ResultType::Err) {
			printf("Error setting up camera %d: %s", i, result.value.err_value);
		}
	}
	
	attachInterrupt(digitalPinToInterrupt(PCLK_PINS[0]), on_pclk_0, FALLING);
}





int main() {
	
	Wire2.begin();
	Serial.begin(115200);
	
	
	for (u8 i = 0; i < N_CAMERAS; i++) {
		pinMode(HREF_PINS[i], INPUT);
		pinMode(RESET_PINS[i], OUTPUT);
		pinMode(XCLK_PINS[i], OUTPUT);
		analogWriteFrequency(XCLK_PINS[i], XCLK_FREQUENCY);
		analogWrite(XCLK_PINS[i], 128);
	}
	
	
	while (!Serial) delay(50);
	
	u8 resolution = 0;
	u8 clock_divisor = CLKRC_DIV_SET(32);
	
	configure_cameras(resolution, clock_divisor);
	
	
	while (true) {
		
		if (Serial.available() == 0) continue;
		
		u8 byte = Serial.read();
		
		clock_divisor = byte & 0b11111;
		resolution = (byte >> 5) & 0b111;
		configure_cameras(resolution, clock_divisor);
		
	}
	
	
	
}


