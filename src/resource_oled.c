/*
 * Copyright (c) 2019 G.camp,
 *
 * Contact: Jin Seog Bang <seog814@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <peripheral_io.h>
#include "as-log.h"
#include "resource.h"

/* I2C */
#define SSD1306_ADDR 0x03c /* Address of SSD1306 OLED */

static struct {
	int opened;
	peripheral_i2c_h sensor_h;
} resource_sensor_s1;

void resource_close_OLED(void)
{
	if (!resource_sensor_s1.opened)
		return;

	_I("OLED display is finishing...");
	peripheral_i2c_close(resource_sensor_s1.sensor_h);
	resource_sensor_s1.opened = 0;
}

int resource_OLED_initial(int i2c_bus)
{
	int ret = PERIPHERAL_ERROR_NONE;
	static int write = 0;
	unsigned char buff_count ;
	unsigned char page_count;
	unsigned char buf[10] = { 0, };
	unsigned char tcbuf[10] = { 0, };

	unsigned char tmbuf[30] = { 0xae, 0xd5, 0x80, 0xa8, 0x3f, 
								0xd3, 0x00, 0x40, 0x8d, 0x14,
								0x20, 0x00, 0xda, 0x12, 0x81,
								0x9f, 0xd9, 0x22, 0xdb, 0x40,
								0xa4, 0xa6, 0xaf};

	if (!resource_sensor_s1.opened) {
		
		ret = peripheral_i2c_open(i2c_bus, SSD1306_ADDR, &resource_sensor_s1.sensor_h);
		if (ret != PERIPHERAL_ERROR_NONE) {
			_E("i2c open error OLED1 : %s", get_error_message(ret));
			 
			return -1;
		}
		resource_sensor_s1.opened = 1;
		write = 0;
	}

//	 buf[0] = 0x00;	// OLED control command
//	 buf[1] = 0xae;	// OLED command

#if 1
	for (buff_count = 0; buff_count < 24; buff_count++)
	{	
		if (!write) {
			buf[0] = 0x00;
			buf[1] = tmbuf[buff_count];

			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED : %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???
		}
	
		_I("iic OLED control initial .. step ");
	}
#endif	

#if 1
	buf[0] = 0x20;
	buf[1] = 0x10;
	for (buff_count = 0; buff_count < 2; buff_count++)
	{
		if (!write) {
			buf[0] = 0x00;
			buf[1] = tcbuf[buff_count];
			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 1: %s", get_error_message(ret));

				return -1;
			}
			write = 0;	//write = 1;???
		}
	}

	_I("iic OLED control pageaddr .. step ");
#endif

#if 1
	tcbuf[0] = 0x21;
	tcbuf[1] = 0x00;
	tcbuf[2] = 0x7f;
	for (buff_count = 0; buff_count < 3; buff_count++)
	{
		if (!write) {
			buf[0] = 0x00;
			buf[1] = tcbuf[buff_count];
			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 2: %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???

		}
	}
	_I("iic OLED control columnaddr .. step ");
#endif

#if 0	
	buf[0] = 0x22;
	buf[1] = 0x00;
	buf[2] = 0x00;
	for (buff_count = 0; buff_count < 3; buff_count++)
	{	
		if (!write) {
			buf[0] = 0x00;
			buf[1] = tcbuf[buff_count];
			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 3: %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???
		}
	}
	_I("iic OLED control pageaddr .. step ");
#endif

#if 1
	for (page_count = 0; page_count < 8; page_count ++)
	{
		tcbuf[0] = (0xb0 + page_count);
		buff_count = 0;
			if (!write) {
				buf[0] = 0x00;
				buf[1] = tcbuf[buff_count];
				ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
				if (ret != PERIPHERAL_ERROR_NONE) {
					_E("i2c write error OLED 4: %s", get_error_message(ret));
					return -1;
				}
			write = 0;	//write = 1;???
			}
			_I("iic OLED control pageaddr .. step ");


		for (buff_count = 0; buff_count < 128; buff_count++)
		{	
			if (!write) {
				buf[0] = 0x40;
				buf[1] = 0x00;
			
				ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
				if (ret != PERIPHERAL_ERROR_NONE) {
					_E("i2c write error OLED 5 : %s", get_error_message(ret));
					return -1;
				}
				write = 0;	//write = 1;???
			}
			_I("iic OLED data .. step ");
		
		}
	}
#endif	

	return 0;
}

int resource_OLED_disp_logo(int page_ad)
{
	int ret = PERIPHERAL_ERROR_NONE;
	static int write = 0;
	unsigned char buff_count ;
	unsigned char page_count;
	unsigned char buf[10] = { 0, };
	unsigned char tcbuf[10] = { 0, };
	unsigned char font_G_l[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x07, 0x18, 0x20, 0x20, 0x22, 0x1E, 0x02, 0x00, // G font
								//0x00, 0x1f, 0x20, 0x20, 0x20, 0x20, 0x1f, 0x00, // G font
								0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, // - font
								0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, // - font
								0x00, 0x1f, 0x20, 0x20, 0x20, 0x20, 0x10, 0x00, // C font
								0x00, 0x3f, 0x01, 0x01, 0x01, 0x01, 0x3f, 0x00, // A font
								0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, // M font
								0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // P font
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};	
	unsigned char font_G_h[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0xC0, 0x30, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, // G font
								//0x00, 0xf8, 0x04, 0x04, 0x84, 0x84, 0x88, 0x00, // G font
								0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, // - font
								0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, // - font
								0x00, 0xf8, 0x04, 0x04, 0x04, 0x04, 0x08, 0x00, // C font
								0x00, 0xc0, 0x30, 0x0c, 0x0c, 0x30, 0xc0, 0x00, // A font
								0x00, 0xfc, 0x08, 0x70, 0x70, 0x08, 0xfc, 0x00, // M font
								0x00, 0xfc, 0x84, 0x84, 0x84, 0x84, 0x78, 0x00, // P font
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
// resource_write_led(5, 1);


	tcbuf[0] = 0x21;
	tcbuf[1] = 0x00;
	tcbuf[2] = 0x7f;
	for (buff_count = 0; buff_count < 3; buff_count++)
	{
		if (!write) {
			buf[0] = 0x00;
			buf[1] = tcbuf[buff_count];
			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 2: %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???
		}
	}	
	_I("iic OLED control columnaddr .. step ");
	
		page_count = page_ad;
		tcbuf[0] = (0xb0 + page_count);
		buff_count = 0;
			if (!write) {
				buf[0] = 0x00;
				buf[1] = tcbuf[buff_count];
				ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
				if (ret != PERIPHERAL_ERROR_NONE) {
					_E("i2c write error OLED 6: %s", get_error_message(ret));
					return -1;
				}
			write = 0;	//write = 1;???
			}
			_I("iic OLED logo disp high pageaddr .. step ");


		for (buff_count = 0; buff_count < 120; buff_count++)
		{	
			if (!write) {
				buf[0] = 0x40;
				buf[1] = font_G_h[buff_count];
			
				ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
				if (ret != PERIPHERAL_ERROR_NONE) {
					_E("i2c write error OLED 7 : %s", get_error_message(ret));
					return -1;
				}
				write = 0;	//write = 1;???
			}
			_I("iic OLED logo font high data .. step ");
		
		}
		
	tcbuf[0] = 0x21;
	tcbuf[1] = 0x00;
	tcbuf[2] = 0x7e;
	for (buff_count = 0; buff_count < 3; buff_count++)
	{
		if (!write) {
			buf[0] = 0x00;
			buf[1] = tcbuf[buff_count];
			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 2: %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???
		}
	}	
	_I("iic OLED control columnaddr .. step ");
	
	page_count = page_ad + 1;
	tcbuf[0] = (0xb0 + page_count );
	buff_count = 0;
	if (!write) {
		buf[0] = 0x00;
		buf[1] = tcbuf[buff_count];
		ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
		if (ret != PERIPHERAL_ERROR_NONE) {
			_E("i2c write error OLED 6: %s", get_error_message(ret));
			return -1;
			}
		write = 0;	//write = 1;???
	}
	_I("iic OLED logo disp low pageaddr .. step ");
			
	for (buff_count = 0; buff_count < 120; buff_count++)
	{	
		if (!write) {
			buf[0] = 0x40;
			buf[1] = font_G_l[buff_count];
			
			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 7 : %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???
		}
		_I("iic OLED logo font high data .. step ");
	}
	return 0;	
}

int resource_OLED_disp_advise(char typ)
{
	int ret = PERIPHERAL_ERROR_NONE;
	unsigned char * lfont_buf;
	unsigned char * hfont_buf;
	static int write = 0;
	unsigned char buff_count ;
	unsigned char page_count;
	unsigned char buf[10] = { 0, };
	unsigned char tcbuf[10] = { 0, };
	unsigned char font_ST_l[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x38, 0x20, 0x21, 0x21, 0x22, 0x1C, 0x00, // S font
								0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00, // T font
								0x20, 0x3F, 0x20, 0x00, 0x03, 0x0C, 0x30, 0x20, // R font
								0x20, 0x3F, 0x20, 0x20, 0x23, 0x20, 0x18, 0x00, // E font
								0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00, // T font
								0x07, 0x18, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00, // C font
								0x20, 0x3F, 0x21, 0x01, 0x01, 0x21, 0x3F, 0x20, // H font
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char font_ST_h[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x70, 0x88, 0x08, 0x08, 0x08, 0x38, 0x00, // S font
								0x18, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x18, 0x00, // T font
								0x08, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, // R font
								0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, // E font
								0x18, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x18, 0x00, // T font
								0xC0, 0x30, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, // C font
								0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, // H font
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	unsigned char font_G_l[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x07, 0x18, 0x20, 0x20, 0x22, 0x1E, 0x02, 0x00, // G font
								0x0F, 0x10, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00, // O font
								0x0F, 0x10, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00, // O font
								0x20, 0x3F, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00, // D font
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char font_G_h[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0xC0, 0x30, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, // G font
								0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, // O font
								0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, // O font
								0x08, 0xF8, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, // D font
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	switch(typ)
	{
		case 'G' :
			lfont_buf = &font_G_l;
			hfont_buf = &font_G_h;
			break;
		case 'S' :
			lfont_buf = &font_ST_l;
			hfont_buf = &font_ST_h;
			break;
	}

	tcbuf[0] = 0x21;
	tcbuf[1] = 0x00;
	tcbuf[2] = 0x7f;

	for (buff_count = 0; buff_count < 3; buff_count++)
	{
		if (!write) {
			buf[0] = 0x00;
			buf[1] = tcbuf[buff_count];
			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 2: %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???
		}
	}
	_I("iic OLED control columnaddr .. step ");

	page_count = 0;
	tcbuf[0] = (0xb0 + page_count);
	buff_count = 0;
	if (!write) {
		buf[0] = 0x00;
		buf[1] = tcbuf[buff_count];
		ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
		if (ret != PERIPHERAL_ERROR_NONE) {
			_E("i2c write error OLED 6: %s", get_error_message(ret));
			return -1;
		}
		write = 0;	//write = 1;???
	}
	_I("iic OLED arabia font disp high pageaddr .. step ");


	for (buff_count = 0; buff_count < 120; buff_count++)
	{
		if (!write) {
			buf[0] = 0x40;
			buf[1] = hfont_buf[buff_count];

			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 7 : %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???
		}
			_I("iic OLED arabia font high data .. step ");
	}

	tcbuf[0] = 0x21;
	tcbuf[1] = 0x00;
	tcbuf[2] = 0x7e;
	for (buff_count = 0; buff_count < 3; buff_count++)
	{
		if (!write) {
			buf[0] = 0x00;
			buf[1] = tcbuf[buff_count];
			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 2: %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???
		}
	}
	_I("iic OLED control columnaddr .. step ");

	page_count = 1;
	tcbuf[0] = (0xb0 + page_count );
	buff_count = 0;
	if (!write) {
		buf[0] = 0x00;
		buf[1] = tcbuf[buff_count];
		ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
		if (ret != PERIPHERAL_ERROR_NONE) {
			_E("i2c write error OLED 6: %s", get_error_message(ret));
			return -1;
			}
		write = 0;	//write = 1;???
	}
	_I("iic OLED arabia font disp low pageaddr .. step ");

	for (buff_count = 0; buff_count < 120; buff_count++)
	{
		if (!write) {
			buf[0] = 0x40;
			buf[1] = lfont_buf[buff_count];
			//buf[1] = font_1_l[buff_count];

			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 7 : %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???
		}
		_I("iic OLED arabia font low data .. step ");
	}

	return 0;
}

int resource_OLED_disp_StringFloat(char* str, int page_sc, int col_sc)
{
	int tmp=0;
	while(str[tmp]!='\0'){
		resource_OLED_disp_val(str[tmp]-48,page_sc,col_sc+8*tmp);
		tmp++;
	}
}

int resource_OLED_disp_val(int arabi, int page_sc, int col_sc)
{
	int ret = PERIPHERAL_ERROR_NONE;
	static int write = 0;
	unsigned char * lfont_buf;
	unsigned char * hfont_buf;

	unsigned char buff_count ;
	unsigned char page_count;
	unsigned char buf[10] = { 0, };
	unsigned char tcbuf[10] = { 0, };

	unsigned char font_1_l[8] = {0x00, 0x00, 0x00, 0x20, 0x3f, 0x20, 0x00, 0x00};
	unsigned char font_1_h[8] = {0x00, 0x00, 0x00, 0x08, 0xfc, 0x00, 0x00, 0x00};

	unsigned char font_2_l[8] = {0x00, 0x3c, 0x22, 0x21, 0x20, 0x20, 0x20, 0x00};
	unsigned char font_2_h[8] = {0x00, 0x08, 0x04, 0x04, 0x84, 0x44, 0x38, 0x00};

	unsigned char font_3_l[8] = {0x00, 0x10, 0x20, 0x20, 0x20, 0x21, 0x1e, 0x00};
	unsigned char font_3_h[8] = {0x00, 0x08, 0x04, 0x84, 0x84, 0x44, 0x38, 0x00};

	unsigned char font_4_l[8] = {0x00, 0x03, 0x02, 0x02, 0x02, 0x3f, 0x02, 0x00};
	unsigned char font_4_h[8] = {0x00, 0xc0, 0x20, 0x10, 0x08, 0xfc, 0x00, 0x00};

	unsigned char font_5_l[8] = {0x00, 0x10, 0x20, 0x20, 0x20, 0x20, 0x1f, 0x00};
	unsigned char font_5_h[8] = {0x00, 0x3c, 0x44, 0x44, 0x44, 0x44, 0x84, 0x00};

	unsigned char font_6_l[8] = {0x00, 0x1f, 0x20, 0x20, 0x20, 0x20, 0x1f, 0x00};
	unsigned char font_6_h[8] = {0x00, 0xf8, 0x84, 0x84, 0x84, 0x84, 0x08, 0x00};

	unsigned char font_7_l[8] = {0x00, 0x30, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x00};
	unsigned char font_7_h[8] = {0x00, 0x04, 0x04, 0x04, 0xc4, 0x34, 0x0c, 0x00};

	unsigned char font_8_l[8] = {0x00, 0x1e, 0x21, 0x21, 0x21, 0x21, 0x1e, 0x00};
	unsigned char font_8_h[8] = {0x00, 0x78, 0x84, 0x84, 0x84, 0x84, 0x78, 0x00};
	
	unsigned char font_9_l[8] = {0x00, 0x10, 0x20, 0x20, 0x20, 0x20, 0x1f, 0x00};
	unsigned char font_9_h[8] = {0x00, 0x78, 0x84, 0x84, 0x84, 0x84, 0xf8, 0x00};

	unsigned char font_0_l[8] = {0x00, 0x1f, 0x20, 0x23, 0x23, 0x20, 0x1f, 0x00};
	unsigned char font_0_h[8] = {0x00, 0xf8, 0x04, 0xc4, 0xc4, 0x04, 0xf8, 0x00};

	unsigned char font_C_l[8] = {0x00, 0x1f, 0x20, 0x20, 0x20, 0x20, 0x10, 0x00};
	unsigned char font_C_h[8] = {0x00, 0xf8, 0x04, 0x04, 0x04, 0x04, 0x08, 0x00};

	unsigned char font_M_l[8] = {0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00};
	unsigned char font_M_h[8] = {0x00, 0xfc, 0x08, 0x70, 0x70, 0x08, 0xfc, 0x00};

	unsigned char font_SP_l[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char font_SP_h[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	unsigned char font_l[8] = {0x00, 0x01, 0x01, 0x07, 0x07, 0x01, 0x01, 0x00};
	unsigned char font_h[8] = {0x00, 0x80, 0x80, 0xe0, 0xe0, 0x80, 0x80, 0x00};


	
//	unsigned char font_l[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//	unsigned char font_h[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
// resource_write_led(5, 1);


	switch(arabi)
	{
		case 1 :
			lfont_buf = &font_1_l;
			hfont_buf = &font_1_h;
			break;
		case 2 :
			lfont_buf = &font_2_l;
			hfont_buf = &font_2_h;
			break;
		case 3 :
			lfont_buf = &font_3_l;
			hfont_buf = &font_3_h;
			break;
		case 4 :
			lfont_buf = &font_4_l;
			hfont_buf = &font_4_h;
			break;
		case 5 :
			lfont_buf = &font_5_l;
			hfont_buf = &font_5_h;
			break;
		case 6 :
			lfont_buf = &font_6_l;
			hfont_buf = &font_6_h;
			break;	
		case 7 :
			lfont_buf = &font_7_l;
			hfont_buf = &font_7_h;
			break;				
		case 8 :
			lfont_buf = &font_8_l;
			hfont_buf = &font_8_h;
			break;				
		case 9 :
			lfont_buf = &font_9_l;
			hfont_buf = &font_9_h;
			break;
		case 0 :
			lfont_buf = &font_0_l;
			hfont_buf = &font_0_h;
			break;
		case 'C' :
			lfont_buf = &font_C_l;
			hfont_buf = &font_C_h;
			break;
		case 'M' :
			lfont_buf = &font_M_l;
			hfont_buf = &font_M_h;
			break;			
		case ' ' :
			lfont_buf = &font_SP_l;
			hfont_buf = &font_SP_h;
			break;
			
		default :
			lfont_buf = &font_l;
			hfont_buf = &font_h;
			break;
	}

	tcbuf[0] = 0x21;
	tcbuf[1] = 0x00+col_sc;
	tcbuf[2] = 0x7f;

	for (buff_count = 0; buff_count < 3; buff_count++)
	{
		if (!write) {
			buf[0] = 0x00;
			buf[1] = tcbuf[buff_count];
			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 2: %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???
		}
	}	
	_I("iic OLED control columnaddr .. step ");
	
		page_count = page_sc;
		tcbuf[0] = (0xb0 + page_count);
		buff_count = 0;
			if (!write) {
				buf[0] = 0x00;
				buf[1] = tcbuf[buff_count];
				ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
				if (ret != PERIPHERAL_ERROR_NONE) {
					_E("i2c write error OLED 6: %s", get_error_message(ret));
					return -1;
				}
			write = 0;	//write = 1;???
			}
			_I("iic OLED arabia font disp high pageaddr .. step ");


		for (buff_count = 0; buff_count < 8; buff_count++)
		{	
			if (!write) {
				buf[0] = 0x40;
				buf[1] = hfont_buf[buff_count];
				//buf[1] = font_1_h[buff_count];
			
				ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
				if (ret != PERIPHERAL_ERROR_NONE) {
					_E("i2c write error OLED 7 : %s", get_error_message(ret));
					return -1;
				}
				write = 0;	//write = 1;???
			}
			_I("iic OLED arabia font high data .. step ");
		
		}
		
	tcbuf[0] = 0x21;
	tcbuf[1] = 0x00+col_sc;
	tcbuf[2] = 0x7e;
	for (buff_count = 0; buff_count < 3; buff_count++)
	{
		if (!write) {
			buf[0] = 0x00;
			buf[1] = tcbuf[buff_count];
			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 2: %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???
		}
	}	
	_I("iic OLED control columnaddr .. step ");
	
	page_count = page_sc + 1;
	tcbuf[0] = (0xb0 + page_count );
	buff_count = 0;
	if (!write) {
		buf[0] = 0x00;
		buf[1] = tcbuf[buff_count];
		ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
		if (ret != PERIPHERAL_ERROR_NONE) {
			_E("i2c write error OLED 6: %s", get_error_message(ret));
			return -1;
			}
		write = 0;	//write = 1;???
	}
	_I("iic OLED arabia font disp low pageaddr .. step ");
			
	for (buff_count = 0; buff_count < 8; buff_count++)
	{	
		if (!write) {
			buf[0] = 0x40;
			buf[1] = lfont_buf[buff_count];
			//buf[1] = font_1_l[buff_count];
			
			ret = peripheral_i2c_write(resource_sensor_s1.sensor_h, buf, 2);
			if (ret != PERIPHERAL_ERROR_NONE) {
				_E("i2c write error OLED 7 : %s", get_error_message(ret));
				return -1;
			}
			write = 0;	//write = 1;???
		}
		_I("iic OLED arabia font low data .. step ");
	}
	return 0;
}

