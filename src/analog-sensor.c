/* ****************************************************************
 *
 * Copyright 2019 Jeonghoon Park All rights reserved.
 *
 * Contact: Jeonghoon Park <jeonghoon79@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <tizen.h>
#include <service_app.h>
#include <Ecore.h>
#include <flex-sensor.h>
#include "as-log.h"
#include "resource.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct app_data_s {
	Ecore_Timer *getter_timer;
} app_data;

/*
typedef struct _app_data {
	Ecore_Idler *idler;
}app_data;
// idler fuction
static Eina_Bool __get_value(void *data)
{
	int ret = 0;
	unsigned int value = 0;

	app_data *ad = data;

	if (!ad) {
		_E("failed to get app_data");
		service_app_exit();
		return ECORE_CALLBACK_CANCEL;
	}

	ret = flex_sensor_read(0, &value);
	retv_if(ret != 0, ECORE_CALLBACK_RENEW);

//	_D("sensor value - [%u]", value);

	return ECORE_CALLBACK_RENEW;
}*/
static Eina_Bool spi_timer_cb(void *data){
	int ret = 0;
		int32_t value = 0;

		// Get value from motion sensor
		ret = flex_sensor_read(0, &value);
		retv_if(ret != 0, ECORE_CALLBACK_RENEW);


		char buf[17]={"                "};
		sprintf(buf,"%16d",(value-490)*9/16);
		resource_OLED_disp_StringFloat(buf,4,0);

		if (value > 600){
			resource_OLED_disp_advise('S');
		}
		else resource_OLED_disp_advise('G');

		_I("flex sensor value : %u", value);


		return ECORE_CALLBACK_RENEW;
}
int timeCnt = 0;

static Eina_Bool watch_timer_cb(void *data){

         // print oled code //
		 char buf[17]={"                "};
		 sprintf(buf,"%16d",timeCnt);
		 resource_OLED_disp_StringFloat(buf,6,0);

		 //_D("[%d] Timer END", count);
		 timeCnt += 1;
	     //timer = NULL;
		 return ECORE_CALLBACK_RENEW;
}


static bool service_app_create(void *data)
{
	//resource_write_led(5, 1);

	resource_OLED_initial(1);
		//resource_OLED_disp_logo(PAGE_SCR);

    return true;
}

static void service_app_terminate(void *data)
{
	flex_sensor_close();
	// timer
	app_data *ad = data;

	// Delete timer
	if (ad->getter_timer) ecore_timer_del(ad->getter_timer);



	// Free app data
	free(ad);

	FN_END;
    return;
}

static void service_app_control(app_control_h app_control, void *data)
{
	//app_data *ad = data;
	//ret_if(!ad);

	app_data *ad = data;


		// Delete old timer if there is one
		if (ad->getter_timer)
			ecore_timer_del(ad->getter_timer);

		// Create a timer to call the given function in given period of time
		ad->getter_timer = ecore_timer_add(0.1, spi_timer_cb, ad);
		ad->getter_timer = ecore_timer_add(1,watch_timer_cb,ad);
		if (!ad->getter_timer) {
			_E("Failed to add getter timer");
			return;
		}
	//idler function
	/*if (ad->idler) {
		ecore_idler_del(ad->idler);
		ad->idler = NULL;
	}

	ad->idler = ecore_idler_add(__get_value, ad);
	if (!ad->idler)
		_E("Failed to add idler");
*/
    return;
}

int main(int argc, char* argv[])
{
	//timer fuction
	app_data *ad = NULL;
	service_app_lifecycle_callback_s event_callback;

	ad = calloc(1, sizeof(app_data));
	retv_if(!ad, -1);

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	return service_app_main(argc, argv, &event_callback, ad);

	/* idler function
	app_data ad;
	service_app_lifecycle_callback_s event_callback;

	ad.idler = NULL;

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	return service_app_main(argc, argv, &event_callback, &ad);
   */
}
