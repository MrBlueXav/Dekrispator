/**
 ******************************************************************************
 * File Name          : random.h
 * Author			  : Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */
#ifndef __RANDOM_H__
#define __RANDOM_H__

#include  <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "stm32f4xx_rng.h"
#include "stm32f4xx_rcc.h"
#include "CONSTANTS.h"


//---------------------------------------------------------------------------
void 		randomGen_init (void);
float_t 	frand_a_b (float_t a, float_t b);
float_t 	randomNum (void);
uint8_t 	MIDIrandVal(void);


/************************************************************************************/
#endif
