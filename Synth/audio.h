/**
 ******************************************************************************
 * File Name          : audio.h
 * Author				: Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_H
#define __AUDIO_H


/* Includes ------------------------------------------------------------------*/
//#include "stm32f4xx_gpio.h"
//#include "mx_gpio.h"
#include <stdint.h>
#include <stdbool.h>
#include "CONSTANTS.h"
#include "my_stm32f4_discovery_audio_codec.h"
#include "my_stm32f4_discovery.h"
#include "soundGen.h"

/* Exported functions ------------------------------------------------------- */
void audio_init(void);
void toggleSound(void);
void incVol(void);
void decVol(void);
void Volume_set(uint8_t val);



#endif /* __AUDIO_H */
