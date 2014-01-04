/**
 ******************************************************************************
 * File Name          	: midi_interface.h
 * Author				: Xavier Halgand
 * Date               	:
 * Description        	:
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MIDI_INTERFACE_H
#define __MIDI_INTERFACE_H

/*--------------------- Global Defines ------------------------ */

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "CONSTANTS.h"
#include "random.h"
#include "drifter.h"
#include "sinetable.h"
#include "notesTables.h"
#include "drifter.h"
#include "soundGen.h"
#include "audio.h"
//#include "my_stm32f4_discovery.h"

/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
void MIDI_Decode(uint8_t * outBuf);
void MagicFX(uint8_t val);
void MagicPatch(uint8_t val);


/****************************************************************************************************************************/
#endif /* __MIDI_INTERFACE_H */
