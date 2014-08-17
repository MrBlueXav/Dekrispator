/**
 ******************************************************************************
 * File Name		: timers.h
 * Author			: Xavier Halgand
 * Date				:
 * Description		:
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMERS_H
#define __TIMERS_H


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "CONSTANTS.h"
#include "audio.h"
#include "my_stm32f4_discovery_audio_codec.h"
#include "my_stm32f4_discovery.h"
#include "soundGen.h"


/* Exported functions ------------------------------------------------------- */
void 		TimingDelay_Decrement(void);
void 		wait(uint32_t nTime);
void 		setSysTick(void);
uint32_t 	getTime(void);
void 		setTime(uint32_t val);

/*************************************************************************************/
#endif /* __TIMERS_H */
