






int32_t ov2640_init(void) {
	uint8_t addr = 0x60 >> 1;

	sccb_write_reg(addr, 0xff, 1);    /* set to page 1 */
	sccb_write_reg(addr, 0x12, 80);   /* software reset */
	chThdSleepMilliseconds(50);

	/* Write selected arrays to the camera to initialize it and set the
	 * desired output format. */
	ov2640_write_array(ov2640_init_regs);

	ov2640_write_array(ov2640_size_change_preamble_regs);
	ov2640_write_array(ov2640_vga_regs);

	ov2640_write_array(ov2640_format_change_preamble_regs);
	ov2640_write_array(ov2640_yuyv_regs);

	ov2640_write_array(ov2640_jpeg_regs);

	return 0;
}


on_href_change(rising) {
	if (rising) {
		// start dma
	} else {
		// stop dma
	}
}


on_vsync() {
	// i dunno do stuff ig
}



int32_t dma_init(void) {
	dmastp  =	STM32_DMA_STREAM(STM32_DMA_STREAM_ID(2, 3));
	dmamode =	STM32_DMA_CR_CHSEL(6) |
			STM32_DMA_CR_PL(2) |
			STM32_DMA_CR_DIR_P2M |
			STM32_DMA_CR_MSIZE_BYTE |
			STM32_DMA_CR_PSIZE_BYTE |
			STM32_DMA_CR_MINC |
			STM32_DMA_CR_DMEIE |
			STM32_DMA_CR_TEIE |
			STM32_DMA_CR_CIRC |
			STM32_DMA_CR_TCIE |
			STM32_DMA_CR_HTIE;

	dmaStreamAllocate(dmastp, 2, (stm32_dmaisr_t)dma_interrupt, NULL);

	/* GPIOE pins 8-15 are used for data input. Configure GPIOE->IDR's
	 * second byte as the DMA source (assuming little-endian byte order). */
	dmaStreamSetPeripheral(dmastp, (uint8_t *)(&(GPIOE->IDR)) + 1);
	dmaStreamSetMemory0(dmastp, sample_buffer);
	dmaStreamSetTransactionSize(dmastp, sizeof(sample_buffer));

	dmaStreamSetMode(dmastp, dmamode);

	TIM_TypeDef *tim = (TIM_TypeDef *)STM32_TIM1;
	rccEnableTIM1(FALSE);
	rccResetTIM1();
	tim->CCR1 = 0;
	tim->CCER = 0;
	tim->ARR  = 10;
	tim->CNT  = 0;
	tim->SR   = 0;

	/* set channel 1 to input */
	tim->CCMR1 = TIM_CCMR1_CC1S_0;
	/* enable first input capture channel */
	tim->CCER = TIM_CCER_CC1E;

	tim->CR2 = 0;
	tim->CR1 = 0;

	return 0;
}


/**
 * This single thread initializes the camera and continuously fetches images
 * from it. They are sent over the USART interface as simple newline delimited
 * hexdumps of data.
 */
static msg_t Thread1(void *arg) {
	(void)arg;

	chRegSetThreadName("test");

	/* I2C1_SDA, I2C1, AF4 */
	palSetPadMode(GPIOB, 7, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_PUDR_PULLUP);
	/* I2C1_SCL, I2C1, AF4 */
	palSetPadMode(GPIOB, 8, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_PUDR_PULLUP);

	/* XCLK, timer 3, channel 1, AF2 */
	palSetPadMode(GPIOC, 6, PAL_MODE_ALTERNATE(2));
	/* Reset */
	palSetPadMode(GPIOC, 8, PAL_MODE_OUTPUT_PUSHPULL);
	/* PWDN */
	palSetPadMode(GPIOC, 9, PAL_MODE_OUTPUT_PUSHPULL);

	/* PCLK, timer1 input capture, channel 1 */
	palSetPadMode(GPIOA, 8, PAL_MODE_ALTERNATE(1));

	/* USART2 TX */
	palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));


	/* MCO1 */
	//~ palSetPadMode(GPIOA, 8, PAL_MODE_ALTERNATE(0));

	/* apply clock here */
	pwmStart(&PWMD3, &pwmcfg3);
	pwmEnableChannel(&PWMD3, 0, 1);
	chThdSleepMilliseconds(5);

	/* reset the camera */
	palClearPad(GPIOC, 8);
	chThdSleepMilliseconds(5);
	palSetPad(GPIOC, 8);
	chThdSleepMilliseconds(5);

	/* set PWDN low to exit power down */
	palClearPad(GPIOC, 9);
	chThdSleepMilliseconds(10);

	/* intialize serial output */
	sdStart(&SD2, &uart2_config);

	i2cStart(&I2CD1, &i2cfg1);
	ov2640_init();

	chSemInit(&frame_ready_sem, 0);

	dma_init();
	//~ dma_start();
	exti_start();

	while (1) {
		//~ uint8_t vid;

		//~ sccb_write_reg(0x60 >> 1, 0xff, 1);    /* set to page 1 */
		//~ if (sccb_read_reg(0x60 >> 1, 0x0a, &vid) == 0) {
			//~ if (vid == 0x26) {
				//~ palTogglePad(GPIOD, 14);
			//~ }
		//~ }

		chSemWait(&frame_ready_sem);

		palSetPad(GPIOD, 15);
		uint32_t data_len = sizeof(sample_buffer) - dmastp->stream->NDTR;

		if (data_len == 0) {
			data_len = sizeof(sample_buffer);
		}

		/* There is a chance that some redundant data will be captured
		 * before the actual JPEG output is started. Move the buffer
		 * forward until we find 0xffd8 (start of the JPEG image). */
		uint8_t *buf = sample_buffer;
		while (data_len > 0) {
			if (*buf == 0xff && *(buf + 1) == 0xd8) {
				break;
			}
			data_len--;
			buf++;
		}

		//~ chprintf((struct BaseSequentialStream *)(&SD2), "HEADER %d\r\n", data_len);
		for (uint32_t i = 0; i < data_len; i++) {
			chprintf((struct BaseSequentialStream *)(&SD2), "%02x", buf[i]);
		}
		chprintf((struct BaseSequentialStream *)(&SD2), "\r\n");

		/* Wait some time and capture again. */
		chThdSleepMilliseconds(500);
		palClearPad(GPIOD, 15);
	}

	return 0;
}


int main(void) {

	halInit();
	chSysInit();
	chHeapInit(&ccm_heap, (void *)0x10000000, 0x10000);

	/* leds */
	palSetPadMode(GPIOD, 12, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOD, 13, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOD, 14, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOD, 15, PAL_MODE_OUTPUT_PUSHPULL);

	palClearPad(GPIOD, 12);
	palClearPad(GPIOD, 13);
	palClearPad(GPIOD, 14);
	palClearPad(GPIOD, 15);


	chThdCreateFromHeap(NULL, 4000, NORMALPRIO, Thread1, NULL);

	while (1) {
		chThdSleepMilliseconds(500);
	}
}
