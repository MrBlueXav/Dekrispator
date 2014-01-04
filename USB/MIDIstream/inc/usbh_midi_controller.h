/**
  ******************************************************************************
  * @file    usbh_hid_keybd.h
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive -----------------------------------------------*/
#ifndef __USBH_MIDI_CONTROLLER_H
#define __USBH_MIDI_CONTROLLER_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include "usbh_midi_core.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_MIDI_CLASS
  * @{
  */
  
/** @defgroup USBH_MIDI_CONTROLLER
  * @brief This file is the Header file for USBH_MIDI_CONTROLLER.c
  * @{
  */ 


/** @defgroup USBH_MIDI_CONTROLLER_Exported_Types
  * @{
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/** @defgroup USBH_MIDI_CONTROLLER_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_MIDI_CONTROLLER_Exported_Variables
  * @{
  */ 

//extern MIDI_cb_t MIDI_CONTROLLER_cb;
/**
  * @}
  */ 

/** @defgroup USBH_MIDI_CONTROLLER_Exported_FunctionsPrototype
  * @{
  */ 
void  USR_CONTROLLER_Init (void);
void  USR_CONTROLLER_ProcessData (uint8_t pbuf);
/**
  * @}
  */ 

#endif /* __USBH_MIDI_CONTROLLER_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

