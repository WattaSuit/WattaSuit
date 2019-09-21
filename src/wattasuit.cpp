#include <tizen.h>
#include <service_app.h>
#include "wattasuit.h"
#include "GPIODigitalOut.h"
#include "GPIODigitalInterrupt.h"
#include "LiquidCrystal_I2C.h"
#include <unistd.h>

#define SENSOR_LED_GPIO_NUMBER (24)
#define SENSOR_MOTION_GPIO_NUMBER (18)

GPIO_Digital_Out GPIOLedOut( SENSOR_LED_GPIO_NUMBER );
GPIO_Digital_Interrrupt MotionIn(SENSOR_MOTION_GPIO_NUMBER);

LiquidCrystal_I2C lcd(0x27, 16, 2);

static void motion_interrupted_cb(peripheral_gpio_h gpio, peripheral_error_e error, void *user_data)
{

	dlog_print(DLOG_INFO, LOG_TAG, "Detected motion value=%u  : %s (%s=>%d)", (uint32_t)user_data, rindex(__FILE__, '/') + 1, __func__, __LINE__);

	// Set LED light with value
	//GPIOLedOut = MotionIn.read();
}

bool service_app_create(void *data)
{
	dlog_print(DLOG_INFO, LOG_TAG, "%s : %s (%s=>%d)", "service_app_create", rindex(__FILE__, '/') + 1, __func__, __LINE__);

    // Todo: add your code here.
    return true;
}

void service_app_terminate(void *data)
{
	dlog_print(DLOG_INFO, LOG_TAG, "%s : %s (%s=>%d)", "service_app_terminate", rindex(__FILE__, '/') + 1, __func__, __LINE__);
    return;
}

void service_app_control(app_control_h app_control, void *data)
{
	dlog_print(DLOG_INFO, LOG_TAG, "%s : %s (%s=>%d)", "service_app_control", rindex(__FILE__, '/') + 1, __func__, __LINE__);

	//MotionIn.both(motion_interrupted_cb );


	//----------------------------------------------------------------------------------
	// initialize the LCD
	//----------------------------------------------------------------------------------
	lcd.begin();
	lcd.backlight();

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Watta Suit");

	sleep(2);

	lcd.setCursor(0, 1);
	lcd.print("MARK 0.1");

	sleep(2);
}



int main(int argc, char* argv[])
{
    char ad[50] = {0,};
	service_app_lifecycle_callback_s event_callback;

	event_callback.create 		= service_app_create;
	event_callback.terminate 	= service_app_terminate;
	event_callback.app_control 	= service_app_control;


	return service_app_main(argc, argv, &event_callback, ad);
}
