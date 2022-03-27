#include "./Application/application_config.h"

volatile unsigned char time_1ms_cnt;
volatile unsigned char time_10ms_cnt;
volatile unsigned char time_100ms_cnt;
volatile unsigned char time_500ms_cnt;

volatile unsigned int cnt_read_dat;

char dataSerialRead[64];

unsigned char time_1ms_flag;
unsigned char time_10ms_flag;
unsigned char time_100ms_flag;
unsigned char time_500ms_flag;

unsigned char step_scan = 0;
unsigned char cnt_scan_100ms = 0;
unsigned char data_display[4] = {0xFF,0xFF,0xFF,0xFF};
unsigned char data_display_buf[4] = {0xFF,0xFF,0xFF,0xFF};
unsigned char data_display_cmd1 = DISPLAY_7SEG_CMD_NORMAL;
unsigned char data_display_cmd2 = 0;

unsigned char read_dat_ok = 0;

//###########################################################################
//###########################################################################
//##################                                   ######################
//##################             App Init All          ######################
//##################                                   ######################
//###########################################################################
//###########################################################################
void SysTick_Handler(void){
	time_1ms_cnt++;
	time_1ms_flag = 1;
	if(time_1ms_cnt >= 10){
		time_1ms_cnt = 0;
		time_10ms_cnt++;
		time_10ms_flag = 1;
		if(time_10ms_cnt >= 10){
			time_10ms_cnt = 0;
			time_100ms_cnt++;
			time_100ms_flag = 1;
			if(time_100ms_cnt >= 5){
				time_100ms_cnt = 0;
				time_500ms_cnt++;
				time_500ms_flag = 1;
			}
		}
	}
}

void AppTimerTask(void){
	if(time_1ms_flag){
		AppTask1ms();
		time_1ms_flag = 0;
	}
	if(time_10ms_flag){
		AppTask10ms();
		time_10ms_flag = 0;
	}
	if(time_100ms_flag){
		AppTask100ms();
    time_100ms_flag = 0;
	}
	if(time_500ms_flag){
		AppTask500ms();
		time_500ms_flag = 0;
	}
}

void SYS_Init(void){
	/*---------------------------------------------------------------------------------------------------------*/
	/* Init System Clock                                                                                       */
	/*---------------------------------------------------------------------------------------------------------*/
	/* Unlock protected registers */
	SYS_UnlockReg();

	/* Enable internal 22.1184MHz */
	CLK_EnableXtalRC(CLK_PWRCON_IRC22M_EN_Msk);
	
	/* Waiting for clock ready */
	CLK_WaitClockReady(CLK_CLKSTATUS_IRC22M_STB_Msk);

	/* Switch HCLK clock source to XTL */
	CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_IRC22M,CLK_CLKDIV_HCLK(1));

	/* STCLK to XTL STCLK to XTL */
	CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_IRC22M_DIV2);
	
	/* Enable peripheral clock */
	CLK_EnableModuleClock(UART_MODULE);

	/* Peripheral clock source */
	CLK_SetModuleClock(UART_MODULE,CLK_CLKSEL1_UART_S_IRC22M,CLK_CLKDIV_UART(1));
		
	/* Update System Core Clock */
	/* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
	SystemCoreClockUpdate();
	
	/*---------------------------------------------------------------------------------------------------------*/
	/* Init I/O Multi-function                                                                                 */
	/*---------------------------------------------------------------------------------------------------------*/
	/* Set P1 multi-function pins for UART1 RXD and TXD  */
	SYS->P1_MFP &= ~(SYS_MFP_P12_Msk | SYS_MFP_P13_Msk);
	SYS->P1_MFP |= (SYS_MFP_P12_RXD | SYS_MFP_P13_TXD);
		
//	FMC_Open();
//	chipUID[0] = FMC_ReadUID(0);
//	chipUID[1] = FMC_ReadUID(1);
//	chipUID[2] = FMC_ReadUID(2);
//	FMC_Close();
	
	/* Lock protected registers */
	SYS_LockReg();
}

void HardwareIOInit(void){
	GPIO_SetMode(LED_BLINK_PORT, LED_BLINK_BIT, GPIO_PMD_OUTPUT);

	GPIO_SetMode(SEG_A_PORT, SEG_A_BIT, GPIO_PMD_OUTPUT);
	GPIO_SetMode(SEG_B_PORT, SEG_B_BIT, GPIO_PMD_OUTPUT);
	GPIO_SetMode(SEG_C_PORT, SEG_C_BIT, GPIO_PMD_OUTPUT);
	GPIO_SetMode(SEG_D_PORT, SEG_D_BIT, GPIO_PMD_OUTPUT);
	GPIO_SetMode(SEG_E_PORT, SEG_E_BIT, GPIO_PMD_OUTPUT);
	GPIO_SetMode(SEG_F_PORT, SEG_F_BIT, GPIO_PMD_OUTPUT);
	GPIO_SetMode(SEG_G_PORT, SEG_G_BIT, GPIO_PMD_OUTPUT);
	GPIO_SetMode(SEG_DP_PORT, SEG_DP_BIT, GPIO_PMD_OUTPUT);
	
	GPIO_SetMode(SEG_COL1_PORT, SEG_COL1_BIT, GPIO_PMD_OUTPUT);
	GPIO_SetMode(SEG_COL2_PORT, SEG_COL2_BIT, GPIO_PMD_OUTPUT);
	GPIO_SetMode(SEG_COL3_PORT, SEG_COL3_BIT, GPIO_PMD_OUTPUT);
	GPIO_SetMode(SEG_COL4_PORT, SEG_COL4_BIT, GPIO_PMD_OUTPUT);
	
	SEG_A_PIN = 1;
	SEG_B_PIN = 1;
	SEG_C_PIN = 1;
	SEG_D_PIN = 1;
	SEG_E_PIN = 1;
	SEG_F_PIN = 1;
	SEG_G_PIN = 1;
	SEG_DP_PIN = 1;
	
	SEG_COL2_PIN = 0;
	SEG_COL3_PIN = 0;
	SEG_COL4_PIN = 0;
	SEG_COL1_PIN = 0;
}

void AppCarCleaningDisplayInit(void){
	SYS_Init();
	if(SysTick_Config(SystemCoreClock / 1000)){ while (1);}
	HardwareIOInit();
	UART_Open(UART, 9600);
}

void AppWhileLoop(void){
	UARTListen();
}

//###########################################################################
//###########################################################################
//##################                                   ######################
//##################             Timer Task            ######################
//##################                                   ######################
//###########################################################################
//###########################################################################
void AppTask1ms(void){
	DisplayScan();
}

void AppTask10ms(void){
	UARTTask10ms();
}

void AppTask100ms(void){
	if(data_display_cmd1 == DISPLAY_7SEG_CMD_NORMAL){
		memcpy(data_display,data_display_buf,4);
	}else if(data_display_cmd1 == DISPLAY_7SEG_CMD_SHIFTLEFT){
		if(cnt_scan_100ms == 40){
			data_display[1] = data_display_buf[0];
			data_display[2] = data_display_buf[1];
			data_display[3] = data_display_buf[2];
			cnt_scan_100ms = 0;
		}else if(cnt_scan_100ms == 35){
			data_display[2] = data_display_buf[0];
			data_display[3] = data_display_buf[1];
		}else if(cnt_scan_100ms == 30){
			data_display[3] = data_display_buf[0];
		}else if(cnt_scan_100ms == 25){
			memset(data_display,0,4);
		}else if(cnt_scan_100ms == 20){
			data_display[0] = data_display_buf[3];
			data_display[1] = 0;
			data_display[2] = 0;
			data_display[3] = 0;
		}else if(cnt_scan_100ms == 15){
			data_display[0] = data_display_buf[2];
			data_display[1] = data_display_buf[3];
			data_display[2] = 0;
			data_display[3] = 0;
		}else if(cnt_scan_100ms == 10){
			data_display[0] = data_display_buf[1];
			data_display[1] = data_display_buf[2];
			data_display[2] = data_display_buf[3];
			data_display[3] = 0;
		}else if(cnt_scan_100ms == 5){
			data_display[0] = data_display_buf[0];
			data_display[1] = data_display_buf[1];
			data_display[2] = data_display_buf[2];
			data_display[3] = data_display_buf[3];
		}
		cnt_scan_100ms++;
	}else if(data_display_cmd1 == DISPLAY_7SEG_CMD_SHIFTRIGHT){
		if(cnt_scan_100ms == 40){
			data_display[0] = data_display_buf[1];
			data_display[1] = data_display_buf[2];
			data_display[2] = data_display_buf[3];
			cnt_scan_100ms = 0;
		}else if(cnt_scan_100ms == 35){
			data_display[0] = data_display_buf[2];
			data_display[1] = data_display_buf[3];
		}else if(cnt_scan_100ms == 30){
			data_display[0] = data_display_buf[3];
		}else if(cnt_scan_100ms == 25){
			memset(data_display,0,4);
		}else if(cnt_scan_100ms == 20){
			data_display[0] = 0;
			data_display[1] = 0;
			data_display[2] = 0;
			data_display[3] = data_display_buf[0];
		}else if(cnt_scan_100ms == 15){
			data_display[0] = 0;
			data_display[1] = 0;
			data_display[2] = data_display_buf[0];
			data_display[3] = data_display_buf[1];
		}else if(cnt_scan_100ms == 10){
			data_display[0] = 0;
			data_display[1] = data_display_buf[0];
			data_display[2] = data_display_buf[1];
			data_display[3] = data_display_buf[2];
		}else if(cnt_scan_100ms == 5){
			data_display[0] = data_display_buf[0];
			data_display[1] = data_display_buf[1];
			data_display[2] = data_display_buf[2];
			data_display[3] = data_display_buf[3];
		}
		cnt_scan_100ms++;
	}else if(data_display_cmd1 == DISPLAY_7SEG_CMD_TIMER){
		if(cnt_scan_100ms >= 10){
			data_display[0] = data_display_buf[0];
			data_display[1] = data_display_buf[1] & 0xFE;
			data_display[2] = data_display_buf[2];
			data_display[3] = data_display_buf[3];
			cnt_scan_100ms = 0;
		}else if(cnt_scan_100ms == 5){
			data_display[0] = data_display_buf[0];
			data_display[1] = data_display_buf[1] | 0x01;
			data_display[2] = data_display_buf[2];
			data_display[3] = data_display_buf[3];
		}
		cnt_scan_100ms++;
	}else if(data_display_cmd1 == DISPLAY_7SEG_CMD_BLINK){
			if(cnt_scan_100ms >= 10){
				if((data_display_cmd2 & BLINK_AT1) == BLINK_AT1){
					data_display[0] = 0;
				}else{
					data_display[0] = data_display_buf[0];
				}
				
				if((data_display_cmd2 & BLINK_AT2) == BLINK_AT2){
					data_display[1] = 0;
				}else{
					data_display[1] = data_display_buf[1];
				}
				
				if((data_display_cmd2 & BLINK_AT3) == BLINK_AT3){
					data_display[2] = 0;
				}else{
					data_display[2] = data_display_buf[2];
				}
				
				if((data_display_cmd2 & BLINK_AT4) == BLINK_AT4){
					data_display[3] = 0;
				}else{
					data_display[3] = data_display_buf[3];
				}
				cnt_scan_100ms = 0;
			}else if(cnt_scan_100ms == 5){
				data_display[0] = data_display_buf[0];
				data_display[1] = data_display_buf[1];
				data_display[2] = data_display_buf[2];
				data_display[3] = data_display_buf[3];
			}
			cnt_scan_100ms++;
		
//		
//		if(cnt_scan_100ms >= 10){
//			memcpy(data_display,data_display_buf,4);
//			cnt_scan_100ms = 0;
//		}else if(cnt_scan_100ms == 5){
//			memset(data_display,0,4);
//		}
//		cnt_scan_100ms++;
//		
//		if(data_display_mode == DISPLAY_7SEG_CMD_BLINK_AT1){
//			if(cnt_scan_100ms >= 10){
//				data_display[0] = 0;
//				data_display[1] = data_display_buf[1];
//				data_display[2] = data_display_buf[2];
//				data_display[3] = data_display_buf[3];
//				cnt_scan_100ms = 0;
//			}else if(cnt_scan_100ms == 5){
//				data_display[0] = data_display_buf[0];
//				data_display[1] = data_display_buf[1];
//				data_display[2] = data_display_buf[2];
//				data_display[3] = data_display_buf[3];
//			}
//			cnt_scan_100ms++;
//		}else if(data_display_mode == DISPLAY_7SEG_CMD_BLINK_AT2){
//			if(cnt_scan_100ms >= 10){
//				data_display[0] = data_display_buf[0];
//				data_display[1] = 0;
//				data_display[2] = data_display_buf[2];
//				data_display[3] = data_display_buf[3];
//				cnt_scan_100ms = 0;
//			}else if(cnt_scan_100ms == 5){
//				data_display[0] = data_display_buf[0];
//				data_display[1] = data_display_buf[1];
//				data_display[2] = data_display_buf[2];
//				data_display[3] = data_display_buf[3];
//			}
//			cnt_scan_100ms++;
//		}else if(data_display_mode == DISPLAY_7SEG_CMD_BLINK_AT3){
//			if(cnt_scan_100ms >= 10){
//				data_display[0] = data_display_buf[0];
//				data_display[1] = data_display_buf[1];
//				data_display[2] = 0;
//				data_display[3] = data_display_buf[3];
//				cnt_scan_100ms = 0;
//			}else if(cnt_scan_100ms == 5){
//				data_display[0] = data_display_buf[0];
//				data_display[1] = data_display_buf[1];
//				data_display[2] = data_display_buf[2];
//				data_display[3] = data_display_buf[3];
//			}
//			cnt_scan_100ms++;
//		}else if(data_display_mode == DISPLAY_7SEG_CMD_BLINK_AT4){
//			if(cnt_scan_100ms >= 10){
//				data_display[0] = data_display_buf[0];
//				data_display[1] = data_display_buf[1];
//				data_display[2] = data_display_buf[2];
//				data_display[3] = 0;
//				cnt_scan_100ms = 0;
//			}else if(cnt_scan_100ms == 5){
//				data_display[0] = data_display_buf[0];
//				data_display[1] = data_display_buf[1];
//				data_display[2] = data_display_buf[2];
//				data_display[3] = data_display_buf[3];
//			}
//			cnt_scan_100ms++;
	}
}

void AppTask500ms(void){
	LED_BLINK_PIN ^= 1;
}

//###########################################################################
//###########################################################################
//##################                                   ######################
//##################               UART App            ######################
//##################                                   ######################
//###########################################################################
//###########################################################################
void UARTListen(void){
	unsigned char c;
	while(UART_IS_RX_READY(UART)){
		c = UART_READ(UART);
		UART_WRITE(UART,c);
		dataSerialRead[cnt_read_dat] = c;
		if(c == 0x01){
			cnt_read_dat = 0;
		}else if(c == 0x03){
			dataSerialRead[cnt_read_dat] = 0x00;
			read_dat_ok = 1;
		}else{
			cnt_read_dat++;
			if(cnt_read_dat >= 64){
				cnt_read_dat = 0;
			}
		}
	}
}

void UUdecode(char *dat,char *uudecode){
	char str[64];
	int i,j,k,loop;
	unsigned int shift;
	unsigned int diff;
	memset(str,0x00,sizeof(str));
	memcpy(str,dat,dat[0]);
	
	loop = strlen(str);
	for(i=0;i<loop;i++){
		if(str[i] == 0x60){
			str[i] = 0;
		}else{
			str[i] -= 0x20;
		}
	}
	
	loop /= 4;
	for(i=0;i<loop;i++){
		diff = 0;
		shift = str[(i*4)];
		shift <<= 18;
		diff |= shift;
		
		shift = str[(i*4)+1];
		shift <<= 12;
		diff |= shift;
		
		shift = str[(i*4)+2];
		shift <<= 6;
		diff |= shift;

		shift = str[(i*4)+3];
		diff |= shift;

		for(j=16,k=0;j>=0;j-=8,k++){
			uudecode[(i *3) + k] = (diff >> j);
		}
	}	
}

void UARTTask10ms(void){
	char buf1[64];
	unsigned int length;
	unsigned char i;
	char sum;
	if(read_dat_ok == 1){
		UUdecode(dataSerialRead,buf1);
		length = buf1[0];
		sum = buf1[0];
		for(i=1;i<length-1;i++){
			sum ^= buf1[i];
		}
		
		if(sum == buf1[length-1]){
			memcpy(data_display_buf,&buf1[4],4);
			if(buf1[2] != data_display_cmd1){
				if(buf1[2] == DISPLAY_7SEG_CMD_NORMAL){	
					cnt_scan_100ms = 0;
				}else if(buf1[2] == DISPLAY_7SEG_CMD_SHIFTLEFT){
					cnt_scan_100ms = 5;
				}else if(buf1[2] == DISPLAY_7SEG_CMD_SHIFTRIGHT){
					cnt_scan_100ms = 5;
				}else if(buf1[2] == DISPLAY_7SEG_CMD_BLINK){
					cnt_scan_100ms = 5;
				}else{
//				}else if(buf1[2] == DISPLAY_7SEG_CMD_BLINK_AT1){
//					cnt_scan_100ms = 5;
//				}else if(buf1[2] == DISPLAY_7SEG_CMD_BLINK_AT2){
//					cnt_scan_100ms = 5;
//				}else if(buf1[2] == DISPLAY_7SEG_CMD_BLINK_AT3){
//					cnt_scan_100ms = 5;
//				}else if(buf1[2] == DISPLAY_7SEG_CMD_BLINK_AT4){
//					cnt_scan_100ms = 5;
				}
			}
			data_display_cmd1 = buf1[2];
			data_display_cmd2 = buf1[3];
		}else{
			//Error Check Sum
		}
		read_dat_ok = 0;
	}
}

//###########################################################################
//###########################################################################
//##################                                   ######################
//##################            Scan Display           ######################
//##################                                   ######################
//###########################################################################
//###########################################################################
void DisplayScan(void){
	if(step_scan == 0){
		SEG_COL2_PIN = 0;
		SEG_COL3_PIN = 0;
		SEG_COL4_PIN = 0;
		
		SEG_A_PIN = (data_display[3] & 0x02) ? 0 : 1;
		SEG_B_PIN = (data_display[3] & 0x04) ? 0 : 1;
		SEG_C_PIN = (data_display[3] & 0x08) ? 0 : 1;
		SEG_D_PIN = (data_display[3] & 0x10) ? 0 : 1;
		SEG_E_PIN = (data_display[3] & 0x20) ? 0 : 1;
		SEG_F_PIN = (data_display[3] & 0x40) ? 0 : 1;
		SEG_G_PIN = (data_display[3] & 0x80) ? 0 : 1;
		SEG_DP_PIN = (data_display[3] & 0x01) ? 0 : 1;

		SEG_COL1_PIN = 1;
	}else if(step_scan == 1){
		SEG_COL3_PIN = 0;
		SEG_COL4_PIN = 0;
		SEG_COL1_PIN = 0;
		
		SEG_A_PIN = (data_display[2] & 0x02) ? 0 : 1;
		SEG_B_PIN = (data_display[2] & 0x04) ? 0 : 1;
		SEG_C_PIN = (data_display[2] & 0x08) ? 0 : 1;
		SEG_D_PIN = (data_display[2] & 0x10) ? 0 : 1;
		SEG_E_PIN = (data_display[2] & 0x20) ? 0 : 1;
		SEG_F_PIN = (data_display[2] & 0x40) ? 0 : 1;
		SEG_G_PIN = (data_display[2] & 0x80) ? 0 : 1;
		SEG_DP_PIN = (data_display[2] & 0x01) ? 0 : 1;
		
		SEG_COL2_PIN = 1;		
	}else if(step_scan == 2){
		SEG_COL4_PIN = 0;
		SEG_COL1_PIN = 0;
		SEG_COL2_PIN = 0;
		
		SEG_A_PIN = (data_display[1] & 0x02) ? 0 : 1;
		SEG_B_PIN = (data_display[1] & 0x04) ? 0 : 1;
		SEG_C_PIN = (data_display[1] & 0x08) ? 0 : 1;
		SEG_D_PIN = (data_display[1] & 0x10) ? 0 : 1;
		SEG_E_PIN = (data_display[1] & 0x20) ? 0 : 1;
		SEG_F_PIN = (data_display[1] & 0x40) ? 0 : 1;
		SEG_G_PIN = (data_display[1] & 0x80) ? 0 : 1;
		SEG_DP_PIN = (data_display[1] & 0x01) ? 0 : 1;

		SEG_COL3_PIN = 1;		
	}else if(step_scan == 3){
		SEG_COL1_PIN = 0;
		SEG_COL2_PIN = 0;
		SEG_COL3_PIN = 0;
		
		SEG_A_PIN = (data_display[0] & 0x02) ? 0 : 1;
		SEG_B_PIN = (data_display[0] & 0x04) ? 0 : 1;
		SEG_C_PIN = (data_display[0] & 0x08) ? 0 : 1;
		SEG_D_PIN = (data_display[0] & 0x10) ? 0 : 1;
		SEG_E_PIN = (data_display[0] & 0x20) ? 0 : 1;
		SEG_F_PIN = (data_display[0] & 0x40) ? 0 : 1;
		SEG_G_PIN = (data_display[0] & 0x80) ? 0 : 1;
		SEG_DP_PIN = (data_display[0] & 0x01) ? 0 : 1;

		SEG_COL4_PIN = 1;
	}

	step_scan++;
	if(step_scan > 3){
		step_scan = 0;
	}
}
