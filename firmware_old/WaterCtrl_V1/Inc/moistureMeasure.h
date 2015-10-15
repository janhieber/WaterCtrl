/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** moistureMeasure.h
** Header for moistureMeasure module
**
** Author: dropedout
** -------------------------------------------------------------------------*/

#ifndef MOISTUREMEASURE_H
#define MOISTUREMEASURE_H

int initMoistureMeasure(TIM_HandleTypeDef*);

int deinitMoistureMeasure(void);

uint_fast64_t getMoisture(int channel);

void printMoisture();


#endif // MOISTUREMEASURE_H
