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
#ifdef __cplusplus
extern "C"{
#endif
#define DS3231_ADDR										(0x68)


#define DS3231_ALARM1_ONCE_PER_SECOND					(0x0F)
#define DS3231_ALARM1_ON_SECONDS_MATCH					(0x0E)
#define DS3231_ALARM1_ON_MINUTES_AND_SECONDS_MATCH		(0x0C)
#define DS3231_ALARM1_ON_H_AND_M_AND_S_MATCH			(0x08)
#define DS3231_ALARM1_ON_DATE_AND_H_AND_M_AND_S_MATCH	(0x00)
#define DS3231_ALARM1_ON_DAY_AND_H_AND_M_AND_S_MATCH	(0x10)

/** Timekeeping structure for use with ds3231
 * or maybe also other rtcs.
*/
typedef struct rtc_datetime
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;

	uint8_t wday;
	uint8_t day;
	uint8_t month;
	uint8_t year;
  	uint8_t century;	
}rtc_datetime_t;

/*
 * Clock initialization
 */
void rtc3231_init(void);

/**
 * Reading only time from clock
 * @param dt: datetime structure with reading data
 */
void rtc3231_read_time(rtc_datetime_t *dt);

/**
 * Reading only date from clock
 * @param dt: datetime structure with reading data
 */
void rtc3231_read_date(rtc_datetime_t *dt);

/**
 * Reading date and time from clock
 * @param dt: datetime structure with reading data
 */
void rtc3231_read_datetime(rtc_datetime_t *dt);

/**
 * Writing new time in clock
 * @param dt: datetime structure for writing data
 */
void rtc3231_write_time(rtc_datetime_t *dt);

/**
 * Writing new date in clock
 * @param dt: datetime structure for writing data
 */
void rtc3231_write_date(rtc_datetime_t *dt);

/**
* Writing new datetime in clock
* @param dt: datetime structure for writing data
*/
void rtc3231_write_datetime(rtc_datetime_t *dt);

/**
 * Enable/Disable 32kHz Output signal
 * @param enable: bool
 */
void rtc3231_32khz_output(bool enable);

/**
 * Read the Busy flag from status register.
 * @return true if busy
 */
bool rtc3231_isBusy();

//TODO add functions:
//bool rtc3231_isAlarm1Armed();
//bool rtc3231_isAlarm2Armed();
//void rtc3231_armAlarm1(bool enable);
//void rtc3231_armAlarm2(bool enable);
//uint16_t rtc3231_getTemperature();
#ifdef __cplusplus
}
#endif
#endif
