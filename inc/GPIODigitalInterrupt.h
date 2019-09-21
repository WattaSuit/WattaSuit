/*
 * GPIODigitalInterrupt.h
 *
 *  Created on: Sep 12, 2019
 *      Author: Hell Maker - Duke Kim
 */


#ifndef __GPIO_DIGITAL_INTERRUPT_H__
#define __GPIO_DIGITAL_INTERRUPT_H__

#include <peripheral_io.h>



class GPIO_Digital_Interrrupt {

private:
	peripheral_gpio_h m_hGPIO;
	int	m_nGPIO_PIN;

	uint32_t m_uValue;

/*
 * 	Constructor &* Destructor
 */
public:

	GPIO_Digital_Interrrupt(int pGPIO_PIN) :  m_hGPIO (NULL), m_nGPIO_PIN(-1)
	{

		int ret = PERIPHERAL_ERROR_NONE;

		ret = peripheral_gpio_open(pGPIO_PIN, &m_hGPIO);
		if (ret != PERIPHERAL_ERROR_NONE) {
			dlog_print(DLOG_ERROR, LOG_TAG, "%s : %s (%s=>%d)", "GPIO_Digital_Interrrupt failed.", rindex(__FILE__, '/') + 1, __func__, __LINE__);

			m_nGPIO_PIN = NULL;

			return;
		}


		// It should be removed after peripheral-io is patched.
		peripheral_gpio_set_direction(m_hGPIO, PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW);
		if (ret) {
			peripheral_gpio_close(m_hGPIO);

			dlog_print(DLOG_ERROR, LOG_TAG, "%s : %s (%s=>%d)", "peripheral_gpio_set_direction failed.", rindex(__FILE__, '/') + 1, __func__, __LINE__);

			m_hGPIO = NULL;
			return;
		}

		ret = peripheral_gpio_set_direction(m_hGPIO, PERIPHERAL_GPIO_DIRECTION_IN);
		if (ret != PERIPHERAL_ERROR_NONE) {
			peripheral_gpio_close(m_hGPIO);

			dlog_print(DLOG_ERROR, LOG_TAG, "%s : %s (%s=>%d)", "peripheral_gpio_set_direction failed.", rindex(__FILE__, '/') + 1, __func__, __LINE__);

			m_hGPIO = NULL;
			return;
		}

		m_nGPIO_PIN = pGPIO_PIN;
	}

	~GPIO_Digital_Interrrupt()
	{
		if (!m_hGPIO) return;

		peripheral_gpio_close(m_hGPIO);

		m_hGPIO = NULL;
		m_nGPIO_PIN = -1;
	}


private :

	int gpio_interrupt(peripheral_gpio_interrupted_cb interrupted_cb,peripheral_gpio_edge_e edge_mode)
	{

		int ret = PERIPHERAL_ERROR_NONE;

		if( !interrupted_cb )
			return -1;

		if( !m_hGPIO )
			return -1;

		if( m_nGPIO_PIN	== -1 )
			return -1;

		ret = peripheral_gpio_set_edge_mode(m_hGPIO, edge_mode);
		if(ret != PERIPHERAL_ERROR_NONE )
			return -1;

		//	g_interrupted_data.motion_value = 0;
		ret = peripheral_gpio_read(m_hGPIO, &m_uValue);
		if(ret != PERIPHERAL_ERROR_NONE)
			return -1;

		m_uValue = 1;

		ret = peripheral_gpio_set_interrupted_cb(m_hGPIO, interrupted_cb, (void*)m_uValue);
		if( ret != PERIPHERAL_ERROR_NONE)
			goto ERROR;


		return 0;


	ERROR:
		peripheral_gpio_unset_interrupted_cb(m_hGPIO);

		return -1;
	}


public:


	/*
	 * read from GPIO PIN
	 */
	int read()
	{
		int ret = PERIPHERAL_ERROR_NONE;
		uint32_t  value;

		ret = peripheral_gpio_read(m_hGPIO, &value);
		if( ret != PERIPHERAL_ERROR_NONE)
			return -1;

		return (int)value;
	}

	/*
	 *
	 */
	int rise(peripheral_gpio_interrupted_cb interrupted_cb)
	{
		return gpio_interrupt(interrupted_cb, PERIPHERAL_GPIO_EDGE_RISING );
	}

	/*
	 *
	 */
	int fall(peripheral_gpio_interrupted_cb interrupted_cb)
	{
		return gpio_interrupt(interrupted_cb, PERIPHERAL_GPIO_EDGE_FALLING );
	}

	/*
	 *
	 */
	int both(peripheral_gpio_interrupted_cb interrupted_cb)
	{
		return gpio_interrupt(interrupted_cb, PERIPHERAL_GPIO_EDGE_BOTH );
	}


};


#endif	//	__GPIO_DIGITAL_INTERRUPT_H__
