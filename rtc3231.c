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
#include "i2c_master.h"

/* BITPOSITIONS */
/* Control register */
#define A1IE					(0x00)
#define A2IE					(0x01)
#define INTCN					(0x02)
#define RS1						(0x03)
#define RS2						(0x04)
#define CONV					(0x05)
#define BBSQW					(0x06)
#define EOSC					(0x07)
/* Status register */
#define A1F						(0x00)
#define A2F						(0x01)
#define BSY						(0x02)
#define EN32KHZ					(0x03)
#define OSF						(0x07)


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
	i2c_master_start(DS3231_ADDR, I2C_WRITE);
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
	i2c_master_write(DS3231_SECONDS);
    i2c_master_write(bin(dt->wday));
    i2c_master_write(bin(dt->day));
	i2c_master_write(bin(dt->month));
	i2c_master_write(bin(dt->year));
    i2c_master_stop();
}
