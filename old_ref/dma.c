
// u32 time = ARM_DWT_CYCCNT;




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









	
	// write_regs(address, ov2640_init_regs);
	// write_regs(address, ov2640_size_change_preamble_regs);
	// write_regs(address, ov2640_qvga_regs);
	// write_regs(address, ov2640_jpeg_regs);





// void on_vsync_0_rising();
// void on_vsync_1_rising();
// void on_vsync_2_rising();
// void on_vsync_0_falling();
// void on_vsync_1_falling();
// void on_vsync_2_falling();


// void on_vsync_0_rising() {
// 	attachInterrupt(digitalPinToInterrupt(PCLK_PINS[0]), on_pclk_0, FALLING);
// 	detachInterrupt(digitalPinToInterrupt(VSYNC_PINS[0]));
// 	attachInterrupt(digitalPinToInterrupt(VSYNC_PINS[0]), on_vsync_0_falling, FALLING);
// 	Serial.write("___VS_AAA_ON___");
// }

// void on_vsync_0_falling() {
// 	detachInterrupt(digitalPinToInterrupt(PCLK_PINS[0]));
// 	detachInterrupt(digitalPinToInterrupt(VSYNC_PINS[0]));
// 	attachInterrupt(digitalPinToInterrupt(VSYNC_PINS[1]), on_vsync_1_rising, RISING);
// 	Serial.write("___VS_AAA_OFF___");
// }

// void on_vsync_1_rising() {
// 	attachInterrupt(digitalPinToInterrupt(PCLK_PINS[1]), on_pclk_1, FALLING);
// 	detachInterrupt(digitalPinToInterrupt(VSYNC_PINS[1]));
// 	attachInterrupt(digitalPinToInterrupt(VSYNC_PINS[1]), on_vsync_1_falling, FALLING);
// 	Serial.write("___VS_BBB_ON___");
// }

// void on_vsync_1_falling() {
// 	detachInterrupt(digitalPinToInterrupt(PCLK_PINS[1]));
// 	detachInterrupt(digitalPinToInterrupt(VSYNC_PINS[1]));
// 	attachInterrupt(digitalPinToInterrupt(VSYNC_PINS[2]), on_vsync_2_rising, RISING);
// 	Serial.write("___VS_BBB_OFF___");
// }

// void on_vsync_2_rising() {
// 	attachInterrupt(digitalPinToInterrupt(PCLK_PINS[2]), on_pclk_2, FALLING);
// 	detachInterrupt(digitalPinToInterrupt(VSYNC_PINS[2]));
// 	attachInterrupt(digitalPinToInterrupt(VSYNC_PINS[2]), on_vsync_2_falling, FALLING);
// 	Serial.write("___VS_CCC_ON___");
// }

// void on_vsync_2_falling() {
// 	detachInterrupt(digitalPinToInterrupt(PCLK_PINS[2]));
// 	detachInterrupt(digitalPinToInterrupt(VSYNC_PINS[2]));
// 	attachInterrupt(digitalPinToInterrupt(VSYNC_PINS[0]), on_vsync_0_rising, RISING);
// 	Serial.write("___VS_CCC_OFF___");
// }





