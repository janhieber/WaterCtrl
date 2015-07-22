/*
 * https://github.com/janhieber/WaterCtrl/
 *
 * @file    firmware/fw_v1/moistureMeasure.c
 * @author  dropedout
 * @version 0
 * @date    2015-04-27
 * @brief   Header for moistureMeasure module
 *
 *
 *
 *
 */

int initMoistureMeasure(void);

int deinitMoistureMeasure(void);

uint_fast64_t getMoisture(int channel);

void printMoisture();
