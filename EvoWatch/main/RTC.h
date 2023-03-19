/*
 * RTC.h
 *
 *  Created on: 22 mars 2021
 *      Author: wasso
 */

#ifndef MAIN_RTC_H_
#define MAIN_RTC_H_

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "i2c_init.h"

#define RTC_ADDR_7BITS 0x56
#define BCD2BIN(val) ((((val) & 0x0f) + ((val) >> 4) * 10))
#define BIN2BCD(val) ((((val)/10)<<4) + (val)%10)
#define GPIO_ALARM     27
#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_ALARM)

enum AlarmStat{
	ALARM_DISABLE = 0x00,
	ALARM_ENABLE = 0x01
};

#define RV3029_A_AE_X			BIT(7)

enum rv3029_registers {
	/* Control page */
	RV3029_REG_ONOFFCTRL	= 0x00,
	RV3029_REG_IRQCTRL,
	RV3029_REG_IRQFLAGS,
	RV3029_REG_STATUS,
	RV3029_REG_RSTCTRL,

	/* Watch page */
	RV3029_REG_WSECONDS	= 0x08,
	RV3029_REG_WMINUTES,
	RV3029_REG_WHOURS,
	RV3029_REG_WDATE,
	RV3029_REG_WDOW,
	RV3029_REG_WMONTHS,
	RV3029_REG_WYEARS,

	/* Alarm page */
	RV3029_REG_ASECONDS	= 0x10,
	RV3029_REG_AMINUTES,
	RV3029_REG_AHOURS,
	RV3029_REG_ADATE,
	RV3029_REG_ADOW,
	RV3029_REG_AMONTHS,
	RV3029_REG_AYEARS,

	/* Timer page */
	RV3029_REG_TIMLOW	= 0x18,
	RV3029_REG_TIMHIGH,

	/* Temperature page */
	RV3029_REG_TEMP		= 0x20,

	/* EEPROM data page */
	RV3029_REG_EEDATA0	= 0x28,
	RV3029_REG_EEDATA1,

	/* EEPROM control page */
	RV3029_REG_EECTRL	= 0x30,
	RV3029_REG_XTALOFFSET,
	RV3029_REG_QCOEF,
	RV3029_REG_TURNOVER,

	/* RAM page */
	RV3029_REG_RAMDATA0	= 0x38,
	RV3029_REG_RAMDATA1,
	RV3029_REG_RAMDATA2,
	RV3029_REG_RAMDATA3,
	RV3029_REG_RAMDATA4,
	RV3029_REG_RAMDATA5,
	RV3029_REG_RAMDATA6,
	RV3029_REG_RAMDATA7,
};

typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;
	uint8_t dow;
	uint8_t months;
	uint8_t years;
}Time;

int32_t i2c_RTC_read_reg(uint8_t i2c_num, uint8_t regaddr, uint8_t *data_rd, uint16_t size);
int32_t i2c_RTC_write_reg(uint8_t i2c_num, uint8_t regaddr, uint8_t *data_wr, uint16_t size);
void RTC_Set_Time(Time* time);
Time RTC_Get_Time();
void get_rtc_task(void *args);
void RTC_Set_Alarm(Time* time);
Time RTC_Get_Alarm();
void RTC_Handle_Alarm();


#endif /* MAIN_RTC_H_ */
