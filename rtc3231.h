/* DS3231 Real Time Clock AVR Lirary
 *
 * Copyright (C) 2024 Chris Goertz
 * Written by Christian Goertz (chris.goertz@gmx.de)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by Adafruit Industries. MIT license.
 */

#ifndef __DS3231_H__
#define __DS3231_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#define DS3231_ADDR										(0x68)

/* DS3231 ADDRESS MAP */
#define	DS3231_SECONDS									(0x00)
#define	DS3231_MINUTES									(0x01)
#define	DS3231_HOURS									(0x02)
#define	DS3231_DAY										(0x03)
#define	DS3231_DATE										(0x04)
#define	DS3231_MONTH									(0x05)
#define	DS3231_YEAR										(0x06)
#define	DS3231_ALARM1_SECONDS							(0x07)
#define	DS3231_ALARM1_MINUTES							(0x08)
#define	DS3231_ALARM1_HOURS								(0x09)
#define	DS3231_ALARM1_DAY								(0x0A)
#define	DS3231_ALARM2_MINUTES							(0x0B)
#define	DS3231_ALARM2_HOURS								(0x0C)
#define	DS3231_ALARM2_DAY								(0x0D)
#define	DS3231_ALARM2_DATE								(0x0E)
#define	DS3231_CONTROL									(0x0F)
#define	DS3231_STATUS									(0x10)
#define	DS3231_AGING_OFFSET								(0x11)
#define	DS3231_TEMP_MSB									(0x12)
#define	DS3131_TEMP_LSB									(0x13)
	

#define DS3231_ALARM1_ONCE_PER_SECOND					(0x0F)
#define DS3231_ALARM1_ON_SECONDS_MATCH					(0x0E)
#define DS3231_ALARM1_ON_MINUTES_AND_SECONDS_MATCH		(0x0C)
#define DS3231_ALARM1_ON_H_AND_M_AND_S_MATCH			(0x08)
#define DS3231_ALARM1_ON_DATE_AND_H_AND_M_AND_S_MATCH	(0x00)
#define DS3231_ALARM1_ON_DAY_AND_H_AND_M_AND_S_MATCH	(0x10)
typedef struct rtc_datetime
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;

	uint8_t wday;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	
}rtc_datetime_t;
/*
 * Clock initialization
 */
void rtc3231_init(void);

/**
 * Reading only time from clock
 * @dt: datetime structure with reading data
 */
void rtc3231_read_time(rtc_datetime_t *dt);

/**
 * Reading only date from clock
 * @dt: datetime structure with reading data
 */
void rtc3231_read_date(rtc_datetime_t *dt);

/**
 * Reading date and time from clock
 * @dt: datetime structure with reading data
 */
void rtc3231_read_datetime(rtc_datetime_t *dt);

/**
 * Writing new time in clock
 * @dt: datetime structure for writing data
 */
void rtc3231_write_time(rtc_datetime_t *dt);

/**
 * Writing new date in clock
 * @dt: datetime structure for writing data
 */
void rtc3231_write_date(rtc_datetime_t *dt);

/**
* Writing new datetime in clock
* @dt: datetime structure for writing data
*/
void rtc3231_write_datetime(rtc_datetime_t *dt);

//TODO add functions:
//void rtc3231_32khz_output(bool enable);
//bool rtc3231_isBusy();
//bool rtc3231_isAlarm1Armed();
//bool rtc3231_isAlarm2Armed();
//void rtc3231_armAlarm1(bool enable);
//void rtc3231_armAlarm2(bool enable);
//uint16_t rtc3231_getTemperature();
#endif
