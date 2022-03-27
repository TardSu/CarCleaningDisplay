//
// HARDWARE_VERSION  "FDXY.ZZ"
// FD = Fuel Dispensor
//  X = MCU Nane (1 = Nuvoton, 2 = LPC)
//  Y = Rev.Number (1 = Rev.A, 2 = Rev.B, 3 = Rev.C)
// ZZ = Sub Version
//
//#define HARDWARE_VERSION_SERVER								"Fd12.01"
//#define HARDWARE_VERSION_DIS									"FD1201"
//#define FIRMWARE_VERSION_SERVER								"F105.04"
//#define FIRMWARE_VERSION_DIS									"F10504"

//#define FIRMWARE_VALUE_VALID									0x9267A55A		// value valid firmware config check

//#define nSCREEN																14						// Number of page

//#define MENU_LOOP_SETTING											18

//#define NULL																	0
//#define PRESS																	1
//#define RELEASE																2
//#define HOLD																	3
/*********************************************************************************/

/**************************** EEPROM *********************************************/
//#define EP_SIZE																8192
//#define EP_BLOCK_SIZE													32

//#define EP_START_BLOCK												0
//#define EP_END_BLOCK													EP_SIZE


//#define EP_CFG_SIZE_BLOCK											64
//#define EP_CFG_START_BLOCK										EP_END_BLOCK - (EP_CFG_SIZE_BLOCK * 6)

//#define EP_TRANS_SIZE_BLOCK										32
//#define EP_TRANS_START_BLOCK									6400

//#define EP_RUNSELL_START_BLOCK								352

/*********************************************************************************/

/**************************** UART ************************************************/
//#define CONSOLE
//#define	OIL_SER																UART0
//#define	BAUDRATE															9600

/*********************** Pluse Coin & Bill ***************************************/
//#define TIME_PLUSE_COIN												48
//#define TIME_PLUSE_BILL												50
/*********************************************************************************/

/************************ Max & Min Value ****************************************/
//#define MAX_MONEY_THAI												99999.99
//#define MAX_MONEY_OTHER												999999
//#define MAX_PAY_LITER													200
//#define RECEIVE_MAX_MONEY_THAI								500
//#define RECEIVE_MAX_MONEY_OTHER								100000

//#define DISP_DIGIT														6

//#define MIN_OIL_LV_TERMINATE									20
//#define MAX_OIL_LV_TERMINATE									250

//#define TIME_OUT_FLOWCUT											1500
//#define MAX_VALVE_DRAIN_ON										20
//#define MAX_WASTE_MONEY												600
//#define	MAX_OIL_TYPE													2
//#define	MAX_PAUSE_PUMP												5
//#define MAX_SET_MIN_MONEY_THAI								99
//#define MAX_SET_MIN_MONEY_OTHER								10000
/*********************************************************************************/

/************************** Default Value ****************************************/
//#define VALUE_ZERO														0
//#define VALUE_VALID														0xA55AA55A

#define CFG_BUTTON_MODE_SINGLE									0
#define CFG_BUTTON_MODE_MULTI										1

//#define	CFG_FLOW_1LITER												1150
//#define	CFG_FLOW_CT														55
//#define	CFG_LIGHT_OFF_HOUR										8
//#define	CFG_LIGHT_OFF_MIN											VALUE_ZERO
//#define	CFG_LIGHT_OFF_SEC											VALUE_ZERO
//#define CFG_LIGHT_ON_HOUR											18
//#define	CFG_LIGHT_ON_MIN											VALUE_ZERO
//#define	CFG_LIGHT_ON_SEC											VALUE_ZERO
//#define	CFG_MIN_OILX100												2500
//#define	CFG_SELL_PRICE												20
//#define	CFG_MIN_PRICE													10
//#define	CFG_BILL_PER_BAHT											10
//#define	CFG_VALVE_DRAIN_ON										0
//#define	CFG_WASTE_MONEY												0

//#define	CFG_SOUND_HI_HOUR											8
//#define	CFG_SOUND_HI_MIN											VALUE_ZERO
//#define	CFG_SOUND_HI_SEC											VALUE_ZERO
//#define	CFG_SOUND_LOW_HOUR										18
//#define	CFG_SOUND_LOW_MIN											VALUE_ZERO
//#define	CFG_SOUND_LOW_SEC											VALUE_ZERO
//#define	CFG_SOUND_LV_HI												3
//#define	CFG_SOUND_LV_LOW											1

//#define	CFG_BIT																0

//#define	TRANS_OIL_N_TANK											VALUE_ZERO
//#define TRANS_MONEY_N_BOX											VALUE_ZERO
//#define	TRANS_OIL_N_SELL											VALUE_ZERO

//#define	RUNSELL_OIL_N_RUN											VALUE_ZERO
//#define RUNSELL_MONEY_N_RUN										VALUE_ZERO
//#define RUNSELL_MONEY_N_BOX_RUN								VALUE_ZERO

/*********************************************************************************/

/************************** Sound Level ****************************************/
//#define MAX_LV_SND														5

//#define SND_VOL_LV0														255
//#define SND_VOL_LV1														50
//#define SND_VOL_LV2														25
//#define SND_VOL_LV3														10
//#define SND_VOL_LV4														5
//#define SND_VOL_LV5														0
/*********************************************************************************/

// Screen Control //////////////////////////////////////////////////////////////////
#define ACTION_INIT															0
#define ACTION_BUTTON														1
#define ACTION_KEYPAD														2

///////////////////////////////////////////////////////////////////////////////////

// Button & Keypad ////////////////////////////////////////////////////////////////
#define BUTTON_NUM															6
#define BUTTON_BUFFER_SIZE											4

#define STATUS_NULL															0
#define STATUS_PRESS														1
#define STATUS_RELEASE													2
#define STATUS_HOLD															3

#define BT_RLY1																	'1'
#define BT_RLY2																	'2'
#define BT_RLY3																	'3'
#define BT_RLY4																	'4'
#define BT_RLY5																	'5'
#define BT_RLY6																	'6'

///////////////////////////////////////////////////////////////////////////////////

// Display 7-Segment //////////////////////////////////////////////////////////////
#define DISPLAY_7SEG_COLUMN_0										0x00
#define DISPLAY_7SEG_COLUMN_1										0x20

#define DISPLAY_7SEG_ROW_0											0x00
#define DISPLAY_7SEG_ROW_1											0x01

#define DISPLAY_7SEG_CMD_NORMAL									0
#define DISPLAY_7SEG_CMD_SHIFTLEFT							1
#define DISPLAY_7SEG_CMD_SHIFTRIGHT							2
#define DISPLAY_7SEG_CMD_TIMER									3
#define DISPLAY_7SEG_CMD_BLINK									4

#define BLINK_AT1																0x08
#define BLINK_AT2																0x04
#define BLINK_AT3																0x02
#define BLINK_AT4																0x01

#define DISPLAY_7SEG_DOT_OFF										0x00
#define DISPLAY_7SEG_DOT_ONALL									0xFF
#define DISPLAY_7SEG_DOT_ON1										0x01
#define DISPLAY_7SEG_DOT_ON2										0x02
#define DISPLAY_7SEG_DOT_ON3										0x04
#define DISPLAY_7SEG_DOT_ON4										0x08

///////////////////////////////////////////////////////////////////////////////////

// Control Relay //////////////////////////////////////////////////////////////////
#define RELAY_CTL_OFF														0x00
#define RELAY_CTL_ON														0x01

///////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

#include "Mini51Series.h"

#include "Interface/interface_hardware.h"
//#include "Interface/interface_screen.h"
//#include "Interface/interface_softserial1.h"
//#include "Interface/interface_uca.h"
//#include "Interface/interface_tk77.h"
//#include "Interface/interface_button.h"
//#include "Interface/interface_eeprom.h"
//#include "Interface/interface_uart.h"

#include "Application/application_main.h"
//#include "Application/application_display7seg.h"
