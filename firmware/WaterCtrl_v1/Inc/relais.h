/*
 * relais.h
 *
 *  Created on: Jan 1, 2017
 *      Author: micha
 */

#ifndef INC_RELAIS_H_
#define INC_RELAIS_H_

#include "stdlib.h"
#include "stdbool.h"

#include "stm32f1xx_hal.h"
#include "gpio.h"

#include "main.h"

#define SYSNAME "Relais"

typedef enum _relaisChannels {
	RELAIS_CHAN_0=0,
	RELAIS_CHAN_1,
	RELAIS_CHAN_2,
	RELAIS_CHAN_3,
	RELAIS_CHAN_4

}eRelaisChannel;

/**
 * \brief Set relais to close the relais switch.
 *
 * @return true if relais is in wished state, false if channel number is out of range.
 */
bool SetRelais(eRelaisChannel channel);

/**
 * \brief Clear relais to open the switch.
 *
 * @return true if relais is in wished state, false if channel number is out of range.
 */
bool ClearRelais(eRelaisChannel channel);

/**
 * \brief Toggle relais to toggle the urrent switch state.
 *
 * @return true if relais is in wished state, false if channel number is out of range.
 */
bool ToggleRelais(eRelaisChannel channel);

/**
 * \brief Get the status of the channel.
 *
 * @return true if switch is closed (relais is set)
 */
bool IsRelaisSet(eRelaisChannel channel);

/**
 * \brief init function.
 */

bool InitRelais();


#endif /* INC_RELAIS_H_ */
