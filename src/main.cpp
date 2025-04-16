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
	22, 23, 20, 21, 38, 39, 26, 27, // GPIO6_PSR >> 24
	19, 18, 14, 15, 40, 41, 17, 16, // GPIO6_PSR >> 16
	37, 36,  7,  8, 13, 11, 12, 10, // ((GPIO7_PSR >> 12) & 0b11110000) | (GPIO7_PSR | 0b1111)
};

// Remaining pins:
//	Pin		Bank.Bit	Pad		Xbar	PWM
//	 2		9. 4		EMC_04	 6		yes
//	 3		9. 5		EMC_05	 7		yes
//	 4		9. 6		EMC_06	 8		yes
//	 5		9. 8		EMC_08	17		yes
//	 6		7.10		B0_10			yes
//	 9		7.11		B0_11			yes
//	28		8.18		EMC_32			yes
//	29		9.31		EMC_31			yes
//	30		8.23		EMC_37	23		no
//	31		8.22		EMC_36	22		no
//	32		7.12		B0_12	10		no
//	33		9. 7		EMC_07	 9		yes
//	34		7.29		B1_13			no
//	35		7.28		B1_12			no

const u8 PCLK_PINS[3] = {
	30, //	EMC_37	23
	31, //	EMC_36	22
	32, //	B0_12	10
};

const u8 XCLK_PINS [3] = { 28, 29, 33 };
const u8 RESET_PINS[3] = {  4,  5,  6 };
const u8 VSYNC_PINS[3] = {  9, 34, 35 };




#define XCLK_FREQUENCY 24e6


const RegisterEntry* size_reg_lists[] = {
	OV2640_160x120_JPEG,	// 0 => 2.560us
	OV2640_320x240_JPEG,	// 1 => 2.560us
	OV2640_352x288_JPEG,	// 2 => 2.560us
	OV2640_640x480_JPEG,	// 3 => 1.707us
	OV2640_800x600_JPEG,	// 4 => 853ns
	OV2640_1024x768_JPEG,	// 5 => 853ns
	OV2640_1280x1024_JPEG,	// 6 => 853ns
	OV2640_1600x1200_JPEG,	// 7 => 853ns
	
	// OV2640_176x144_JPEG,
};

// max interrupt speed: 5x 2.560us



void init_camera(u8 id, u8 resolution, u8 clock_divisor) {
	
	// Activate the camera & reset
	digitalWrite(RESET_PINS[id - 1], HIGH);
	delay(10);
	sccb_write(CAM_ID(0), 0xff, 1).unwrap();
	sccb_write(CAM_ID(0), 0x12, 0x80).unwrap();
	delay(10);
	
	u8 address = CAM_ID(id);
	
	// Assign the camera to the specific address
	sccb_write(CAM_ID(0), 0xff, 0).unwrap();
	sccb_write(CAM_ID(0), 0xf7, address << 1).unwrap();
	
	
	// write_regs(address, ov2640_init_regs);
	// write_regs(address, ov2640_size_change_preamble_regs);
	// write_regs(address, ov2640_qvga_regs);
	// write_regs(address, ov2640_jpeg_regs);
	
	write_regs(address, OV2640_JPEG_INIT);
	write_regs(address, OV2640_YUV422);
	write_regs(address, OV2640_JPEG);
	sccb_write(address, 0xff, 0x01);
	sccb_write(address, 0x15, 0x00);
	
	write_regs(address, size_reg_lists[resolution]);
	
	
	sccb_write(address, 0xff, 1).unwrap();
	sccb_write(address, CLKRC, clock_divisor).unwrap();
}




// #define BUF_SIZE 320*240/3

// static u32 buf[BUF_SIZE] = { 0 };

// static DMAChannel dma_channel;




// void on_href() {
// 	if (digitalReadFast(HREF_gpio)) {
// 		dma_channel.enable();
// 	} else {
		
// 		dma_channel.destinationBuffer(buf, BUF_SIZE * 4);
		
// 		let c = dma_channel.TCD->CITER;
// 		printf("\n%d\n", c);
// 		// for (u32 i = c; i < BUF_SIZE; i++) {
// 		// 	Serial.write(0);
// 		// }
		
// 		for (u32 i = 0; i < c; i++) {
// 			Serial.write(buf[i] >> 24);
// 		}
		
// 		// static const u8 href_pattern[] = { 1, 1, 254, 254 };
// 		// Serial.write(href_pattern, 4);
// 	}
// }

// void on_vsync() {
// 	dma_channel.enable();
	
// 	// dma_channel.destinationBuffer(buf, BUF_SIZE * 4);
	
// 	// let c = dma_channel.TCD->CITER;
// 	// printf("\n%d\n", c);
// 	// for (u32 i = c; i < BUF_SIZE; i++) {
// 	// 	Serial.write(0);
// 	// }
	
// 	// for (u32 i = 0; i < c; i++) {
// 	// 	Serial.write(buf[i] >> 24);
// 	// }
// 	// static const u8 vsync_pattern[] = { 2, 2, 253, 253 };
// 	// Serial.write(vsync_pattern, 4);
// }


// void on_dma() {
// 	dma_channel.clearInterrupt();
// 	asm("DSB");
// 	// arm_dcache_delete(buf, BUF_SIZE*4);
	
// 	for (u32 i = 0; i < BUF_SIZE; i++) {
// 		Serial.write(buf[i] >> 24);
// 	}
	
// }



void on_pclk_0() {
	u8 value = GPIO6_PSR >> 24;
	Serial.write(value);
}

void on_pclk_1() {
	u8 value = GPIO6_PSR >> 16;
	Serial.write(value);
}

void on_pclk_2() {
	u8 value = ((GPIO7_PSR >> 12) & 0b11110000) | (GPIO7_PSR | 0b1111);
	Serial.write(value);
}


void on_vsync() {
	if (digitalReadFast(VSYNC_PINS[0])) {
		attachInterrupt(digitalPinToInterrupt(PCLK_PINS[0]), on_pclk_0, FALLING);
	} else {
		detachInterrupt(digitalPinToInterrupt(PCLK_PINS[0]));
	}
}


void configure_cameras(u8 resolution, u8 clock_divisor) {
	detachInterrupt(digitalPinToInterrupt(VSYNC_PINS[0]));
	for (u8 i = 0; i < N_CAMERAS; i++) detachInterrupt(digitalPinToInterrupt(PCLK_PINS[i]));
	
	for (u8 i = 0; i < N_CAMERAS; i++) digitalWrite(RESET_PINS[i], LOW);
	delay(10);
	for (u8 id = 1; id <= N_CAMERAS; id++) init_camera(id, resolution, clock_divisor);
	
	attachInterrupt(digitalPinToInterrupt(VSYNC_PINS[0]), on_vsync, CHANGE);
}





int main() {
	
	Wire2.begin();
	Serial.begin(115200);
	
	
	// GPIO1_GDIR &= ~(0xff000000u); // Set data pins to input
	// IOMUXC_GPR_GPR26 &= ~(0xff000000u); // Set data pins to use GPIO1
	
	// dma_channel.begin();
	// dma_channel.source(GPIO1_DR);
	// dma_channel.destinationBuffer(buf, BUF_SIZE * 4);
	// dma_channel.interruptAtCompletion();
	// dma_channel.attachInterrupt(on_dma);
	
	// CCM_CCGR2 |= CCM_CCGR2_XBAR1(CCM_CCGR_ON); // Enable clocking to the xbar
	// IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_06 = 3;
	// IOMUXC_GPR_GPR6 &= ~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8);
	// IOMUXC_XBAR1_IN08_SELECT_INPUT = 0;
	// XBARA1_CTRL0 = XBARA_CTRL_STS0 | XBARA_CTRL_EDGE0(2) | XBARA_CTRL_DEN0;
	// xbar_connect(XBARA1_IN_IOMUX_XBAR_INOUT08, XBARA1_OUT_DMA_CH_MUX_REQ30);
	
	// dma_channel.triggerAtHardwareEvent(DMAMUX_SOURCE_XBAR1_0);
	// dma_channel.disableOnCompletion();
	
	
	while (!Serial) delay(50);
	
	for (u8 i = 0; i < N_CAMERAS; i++) {
		pinMode(VSYNC_PINS[i], INPUT);
		pinMode(RESET_PINS[i], OUTPUT);
		pinMode(XCLK_PINS[i], OUTPUT);
		analogWriteFrequency(XCLK_PINS[i], XCLK_FREQUENCY);
		analogWrite(XCLK_PINS[i], 128);
	}
	
	
	
	u8 resolution = 0;
	u8 clock_divisor = CLKRC_DIV_SET(64);
	
	configure_cameras(resolution, clock_divisor);
	
	
	
	while (true) {
		
		if (Serial.available() == 0) continue;
		
		u8 byte = Serial.read();
		
		switch (byte >> 6) {
			case 0b00:
				resolution = byte & 0b111;
				configure_cameras(resolution, clock_divisor);
			break;
			case 0b01:
				clock_divisor = byte & 0b111111;
				configure_cameras(resolution, clock_divisor);
			break;
		}
		
		
	}
	
	
	
	// u32 time = ARM_DWT_CYCCNT;
	// u8 value = GPIO6_PSR >> 24;
}










	// Serial.printf("ATTR: %d\n", dma_channel.TCD->ATTR);
	// Serial.printf("ATTR_DST: %d\n", dma_channel.TCD->ATTR_DST);
	// Serial.printf("ATTR_SRC: %d\n", dma_channel.TCD->ATTR_SRC);
	// Serial.printf("BITER: %d\n", dma_channel.TCD->BITER);
	// Serial.printf("BITER_ELINKNO: %d\n", dma_channel.TCD->BITER_ELINKNO);
	// Serial.printf("BITER_ELINKYES: %d\n", dma_channel.TCD->BITER_ELINKYES);
	// Serial.printf("CITER: %d\n", dma_channel.TCD->CITER);
	// Serial.printf("CITER_ELINKNO: %d\n", dma_channel.TCD->CITER_ELINKNO);
	// Serial.printf("CITER_ELINKYES: %d\n", dma_channel.TCD->CITER_ELINKYES);
	// Serial.printf("CSR: %d\n", dma_channel.TCD->CSR);
	// Serial.printf("DADDR: %x\n", dma_channel.TCD->DADDR);
	// Serial.printf("DLASTSGA: %d\n", dma_channel.TCD->DLASTSGA);
	// Serial.printf("DOFF: %d\n", dma_channel.TCD->DOFF);
	// Serial.printf("NBYTES: %d\n", dma_channel.TCD->NBYTES);
	// Serial.printf("NBYTES_MLNO: %d\n", dma_channel.TCD->NBYTES_MLNO);
	// Serial.printf("NBYTES_MLOFFNO: %d\n", dma_channel.TCD->NBYTES_MLOFFNO);
	// Serial.printf("NBYTES_MLOFFYES: %d\n", dma_channel.TCD->NBYTES_MLOFFYES);
	// Serial.printf("SADDR: %x\n", dma_channel.TCD->SADDR);
	// Serial.printf("SLAST: %d\n", dma_channel.TCD->SLAST);
	// Serial.printf("SOFF: %d\n", dma_channel.TCD->SOFF);