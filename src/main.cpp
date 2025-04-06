#include <Arduino.h>
#include <Wire.h>
#include <cstdlib>
#include <DMAChannel.h>

#include "common.h"
#include "sccb.h"
#include "reg_lists.h"




#define VSYNC_gpio 2
#define HREF_gpio 3
#define PCLK_gpio 4
#define XCLK_gpio 12
#define RESET_gpio 6

static const u8 DATA_PINS[8] = { 22, 23, 20, 21, 38, 39, 26, 27 };


#define XCLK_FREQUENCY 24e6


void write_regs(u8 address, RegisterEntry const* regs) {
	while (true) {
		let reg = *regs;
		if (reg.address == 0xff && reg.value == 0xff) break;
		sccb_write(address, reg.address, reg.value).unwrap();
		regs++;
	}
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


void on_pclk() {
	if (digitalReadFast(HREF_gpio) || digitalReadFast(VSYNC_gpio)) {
		u8 value = GPIO6_PSR >> 24;
		Serial.write(value);
	}
}



int main() {
	
	Wire.begin();
	Serial.begin(115200);
	
	
	// GPIO1_GDIR &= ~(0xff000000u); // Set data pins to input
	// IOMUXC_GPR_GPR26 &= ~(0xff000000u); // Set data pins to use GPIO1
	
	// dma_channel.begin();
	// dma_channel.source(GPIO1_DR);
	// dma_channel.destinationBuffer(buf, BUF_SIZE * 4);
	// dma_channel.interruptAtCompletion();
	// dma_channel.attachInterrupt(on_dma);
	
	// CCM_CCGR2 |= CCM_CCGR2_XBAR1(CCM_CCGR_ON);
	// IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_06 = 3;
	// IOMUXC_GPR_GPR6 &= ~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8);
	// IOMUXC_XBAR1_IN08_SELECT_INPUT = 0;
	// XBARA1_CTRL0 = XBARA_CTRL_STS0 | XBARA_CTRL_EDGE0(2) | XBARA_CTRL_DEN0;
	// xbar_connect(XBARA1_IN_IOMUX_XBAR_INOUT08, XBARA1_OUT_DMA_CH_MUX_REQ30);
	// dma_channel.triggerAtHardwareEvent(DMAMUX_SOURCE_XBAR1_0);
	// dma_channel.disableOnCompletion();
	
	
	
	pinMode(RESET_gpio, OUTPUT);
	pinMode(XCLK_gpio, OUTPUT);
	
	pinMode(VSYNC_gpio, INPUT);
	pinMode(HREF_gpio, INPUT);
	
	analogWriteFrequency(XCLK_gpio, XCLK_FREQUENCY);
	analogWrite(XCLK_gpio, 128);
	digitalWrite(RESET_gpio, HIGH);
	
	
	
	delay(1);
	
	sccb_write(0x30, 0xff, 1).unwrap();
	sccb_write(0x30, 0x12, 0x80).unwrap();
	
	delay(50);
	
	
	
#ifdef OLD_INIT
	write_regs(0x30, ov2640_init_regs);
	write_regs(0x30, ov2640_size_change_preamble_regs);
	
	write_regs(0x30, ov2640_qcif_regs);
	// write_regs(0x30, ov2640_qvga_regs);
	// write_regs(0x30, ov2640_vga_regs);
	// write_regs(0x30, ov2640_uxga_regs);
	
	// write_regs(0x30, ov2640_raw10_regs);
	// write_regs(0x30, ov2640_rgb565_le_regs);
	write_regs(0x30, ov2640_jpeg_regs);
	
	
	// sccb_write(0x30, 0xff, 0).unwrap();
	// sccb_write(0x30, 0xf7, 0x62).unwrap();
#else
	
	write_regs(0x30, OV2640_JPEG_INIT);
	write_regs(0x30, OV2640_YUV422);
	write_regs(0x30, OV2640_JPEG);
	sccb_write(0x30, 0xff, 0x01);
	sccb_write(0x30, 0x15, 0x00);
	
	// write_regs(0x30, OV2640_160x120_JPEG);
	// write_regs(0x30, OV2640_320x240_JPEG);
	// write_regs(0x30, OV2640_640x480_JPEG);
	write_regs(0x30, OV2640_1600x1200_JPEG);
	
	
#endif
	
	
	
	sccb_write(0x30, 0xff, 1).unwrap();
	sccb_write(0x30, CLKRC, CLKRC_DIV_SET(64)).unwrap(); // Clock divider (max 64)
	// sccb_write(CTRL1, ~(CTRL1_AWB | CTRL1_AWB_GAIN)).unwrap();
	
	
	
	
	
	
	while (!Serial) delay(50);
	
	// attachInterrupt(digitalPinToInterrupt(VSYNC_gpio), on_vsync, RISING);
	// attachInterrupt(digitalPinToInterrupt(HREF_gpio), on_href, CHANGE);
	attachInterrupt(digitalPinToInterrupt(PCLK_gpio), on_pclk, FALLING);
	
	return 0;
	
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