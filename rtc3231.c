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


#include "rtc3231.h"
#include <i2c_master.h>

/* BITPOSITIONS */

/* Control register */
// Alarm 1 Interrupt enable
#define A1IE					(0x00)
//Alarm 2 Interrupt enable
#define A2IE					(0x01)
// Interrupt Control
#define INTCN					(0x02)
// Rate Select 1 Bit
#define RS1						(0x03)
// Rate Select 2 Bit
#define RS2						(0x04)
// Convert Temperature Bit
#define CONV					(0x05)
// Battery-Backed Square-Wave Enable
#define BBSQW					(0x06)
// Enable Oscillator (active low)
#define EOSC					(0x07)

/* Status register */
// Alarm 1 Interrupt flag
#define A1F						(0x00)
// Alarm 2 Interrupt flag
#define A2F						(0x01)
// Busy flag
#define BSY						(0x02)
// Enable 32kHz Output
#define EN32KHZ					(0x03)
#define OSF						(0x07)

/* Timekeeping Registers */

/* DS3231 ADDRESS MAP */
// BCD encoded seconds
#define	DS3231_SECONDS									(0x00)
// BCD encoded minutes
#define	DS3231_MINUTES									(0x01)
// BCD encoded hours
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
#define	DS3231_TEMP_LSB									(0x13)

/* Timekeeping Bitpositions */
// Depending on 12/24h mode. PM if set
#define DS3231_AM_PM									(0x05)
// 12h mode if set
#define DS3231_12_HOUR_MODE								(0x06)
// Century bit
#define DS3231_CENTURY									(0x07)
#define DS3231_A1M1										(0x07)
#define DS3231_A1M2										(0x07)
#define DS3231_A1M3										(0x07)
#define DS3231_A1M4										(0x07)
#define DS3231_DY_DT									(0x06)
#define DS3231_A2M2										(0x07)
#define DS3231_A2M3										(0x07)
#define DS3231_A2M4										(0x07)

static unsigned char bcd (unsigned char data)
{
	unsigned char bc;

	bc = ((((data & (1 << 6)) | (data & (1 << 5)) | (data & (1 << 4)))*0x0A) >> 4)
	+ ((data & (1 << 3))|(data & (1 << 2))|(data & (1 << 1))|(data & 0x01));

  return bc;
}

static unsigned char bin(unsigned char dec)
{
	char bcd;
	char n, dig, num, count;

	num = dec;
	count = 0;
	bcd = 0;

	for (n = 0; n < 4; n++) {
		dig = num % 10;
		num = num / 10;
		bcd = (dig << count) | bcd;
		count += 4;
	}
	return bcd;
}


void rtc3231_init(void)
{
	uint8_t _state;
	i2c_master_init(I2C_SCL_FREQUENCY_400);
	_state = i2c_master_start(DS3231_ADDR, I2C_WRITE);
	if (_state != I2C_STATUS_SUCCESS)
	{
		return;
	}
	i2c_master_write(DS3231_CONTROL);
	i2c_master_write((1<<CONV)); // Convert Temperature
	i2c_master_write((1<<OSF));
	i2c_master_stop();
}

void rtc3231_read_time(rtc_datetime_t *dt)
{
	i2c_master_start(DS3231_ADDR, I2C_WRITE);
	i2c_master_write(DS3231_SECONDS);
	i2c_master_stop();
	
	i2c_master_start(DS3231_ADDR, I2C_READ);
	dt->sec = bcd(i2c_master_readAck());
	dt->min = bcd(i2c_master_readAck());
	dt->hour = bcd(i2c_master_readAck());
	i2c_master_stop();
}

void rtc3231_read_date(rtc_datetime_t *dt)
{
	i2c_master_start(DS3231_ADDR, I2C_WRITE);
	i2c_master_write(DS3231_DAY);
	i2c_master_stop();
	
	i2c_master_start(DS3231_ADDR, I2C_READ);
	dt->wday = bcd(i2c_master_readAck());
	dt->day = bcd(i2c_master_readAck());
	dt->month = bcd(i2c_master_readAck());
	dt->year = bcd(i2c_master_readNack());
	i2c_master_stop();
}

void rtc3231_read_datetime(rtc_datetime_t *dt)
{
	i2c_master_start(DS3231_ADDR, I2C_WRITE);
	i2c_master_write(DS3231_SECONDS);
	i2c_master_stop();

	i2c_master_start(DS3231_ADDR, I2C_READ);
	dt->sec = bcd(i2c_master_readAck());
	dt->min = bcd(i2c_master_readAck());
	dt->hour = bcd(i2c_master_readAck());

	dt->wday = bcd(i2c_master_readAck());
	dt->day = bcd(i2c_master_readAck());
	dt->month = bcd(i2c_master_readAck());
	dt->year = bcd(i2c_master_readNack());
	i2c_master_stop();
}

void rtc3231_write_time(rtc_datetime_t *dt)
{
    i2c_master_start(DS3231_ADDR, I2C_WRITE);
    i2c_master_write(DS3231_SECONDS);
	i2c_master_write(bin(dt->sec));
	i2c_master_write(bin(dt->min));
	i2c_master_write(bin(dt->hour));
	i2c_master_stop();
}

void rtc3231_write_date(rtc_datetime_t *dt)
{
	i2c_master_start(DS3231_ADDR, I2C_WRITE);
	i2c_master_write(DS3231_DAY);
    i2c_master_write(bin(dt->wday));
    i2c_master_write(bin(dt->day));
	i2c_master_write(bin(dt->month) & 0x1F);
	i2c_master_write(bin(dt->year));
    i2c_master_stop();
}

void rtc3231_write_datetime(rtc_datetime_t *dt)
{
	i2c_master_start(DS3231_ADDR, I2C_WRITE);
	i2c_master_write(DS3231_SECONDS);
	i2c_master_write(bin(dt->sec));
	i2c_master_write(bin(dt->sec));
	i2c_master_write(bin(dt->min));
	i2c_master_write(bin(dt->hour));
	i2c_master_write(bin(dt->wday));
	i2c_master_write(bin(dt->day));
	i2c_master_write(bin(dt->month) & 0x1F);
	i2c_master_write(bin(dt->year));
	i2c_master_stop();
}

void rtc3231_32khz_output(bool enable)
{
	i2c_master_start(DS3231_ADDR, I2C_WRITE);
	i2c_master_write(DS3231_STATUS);
	i2c_master_stop();
	i2c_master_start(DS3231_ADDR, I2C_READ);
	uint8_t _sreg = i2c_master_readNack();
	i2c_master_stop();

	if(enable)
	{
		_sreg |= (1 << EN32KHZ);
	}
	else
	{
		_sreg &= ~(1 << EN32KHZ);
	}
	i2c_master_start(DS3231_ADDR, I2C_WRITE);
	i2c_master_write(DS3231_STATUS);
	i2c_master_write(_sreg);
	i2c_master_stop();
}

bool rtc3231_isBusy()
{
	i2c_master_start(DS3231_ADDR, I2C_WRITE);
	i2c_master_write(DS3231_STATUS);
	i2c_master_stop();
	i2c_master_start(DS3231_ADDR, I2C_READ);
	uint8_t _sreg = i2c_master_readNack();
	i2c_master_stop();
	return _sreg & (1 << BSY);
}