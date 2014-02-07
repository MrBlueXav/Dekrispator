/**
 ******************************************************************************
 * @file    usbh_midi_core.c
 * @author	Xavier Halgand
 * @version
 * @date
 * @brief   Very basic driver for USB Host MIDI class.
 *
 * @verbatim
 *
 *          ===================================================================
 *                                MIDI Class  Description
 *          ===================================================================
 *
 *
 *  @endverbatim
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
*/


/* Includes ------------------------------------------------------------------*/
#include "usbh_midi_core.h"
//#include "usbh_midi_controller.h"


/** @defgroup USBH_MIDI_CORE_Private_Variables
 * @{
 */
extern USB_OTG_CORE_HANDLE	USB_OTG_Core_dev  ;

MIDI_Machine_t				MIDI_Machine  ;

USB_Setup_TypeDef			MIDI_Setup  ;

//USBH_MIDIDesc_t				MIDI_Desc  ;

__IO uint8_t 				start_toggle = 0;

//int State;

/*-----------------------------------------------------------------------------------------*/

static USBH_Status 	USBH_MIDI_InterfaceInit  (USB_OTG_CORE_HANDLE *pdev ,
		void *phost);

static void 		USBH_MIDI_InterfaceDeInit  (USB_OTG_CORE_HANDLE *pdev ,
		void *phost);

static USBH_Status 	USBH_MIDI_ClassRequest(USB_OTG_CORE_HANDLE *pdev ,
		void *phost);

static USBH_Status 	USBH_MIDI_Handle(USB_OTG_CORE_HANDLE *pdev ,
		void *phost);


/****************** MIDI interface ****************************/

USBH_Class_cb_TypeDef  MIDI_cb =
{
		USBH_MIDI_InterfaceInit,
		USBH_MIDI_InterfaceDeInit,
		USBH_MIDI_ClassRequest,
		USBH_MIDI_Handle
};

/*-----------------------------------------------------------------------------------------*/
/**
 * @brief  USBH_MIDI_InterfaceInit
 *         The function init the MIDI class.
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval  USBH_Status :Response for USB MIDI driver intialization
 */
static USBH_Status USBH_MIDI_InterfaceInit ( USB_OTG_CORE_HANDLE *pdev,
		void *phost)
{

	USBH_HOST *pphost = phost;
	USBH_Status status = USBH_BUSY ;
	MIDI_Machine.state = MIDI_ERROR;


	if((pphost->device_prop.Itf_Desc[0].bInterfaceClass == USB_AUDIO_CLASS) && \
			(pphost->device_prop.Itf_Desc[0].bInterfaceSubClass == USB_MIDISTREAMING_SubCLASS))
	{
		if(pphost->device_prop.Ep_Desc[0][0].bEndpointAddress & 0x80)
		{
			MIDI_Machine.MIDIBulkInEp = (pphost->device_prop.Ep_Desc[0][0].bEndpointAddress);
			MIDI_Machine.MIDIBulkInEpSize  = pphost->device_prop.Ep_Desc[0][0].wMaxPacketSize;
		}
		else
		{
			MIDI_Machine.MIDIBulkOutEp = (pphost->device_prop.Ep_Desc[0][0].bEndpointAddress);
			MIDI_Machine.MIDIBulkOutEpSize  = pphost->device_prop.Ep_Desc[0] [0].wMaxPacketSize;
		}

		if(pphost->device_prop.Ep_Desc[0][1].bEndpointAddress & 0x80)

		{
			MIDI_Machine.MIDIBulkInEp = (pphost->device_prop.Ep_Desc[0][1].bEndpointAddress);
			MIDI_Machine.MIDIBulkInEpSize  = pphost->device_prop.Ep_Desc[0][1].wMaxPacketSize;
		}
		else
		{
			MIDI_Machine.MIDIBulkOutEp = (pphost->device_prop.Ep_Desc[0][1].bEndpointAddress);
			MIDI_Machine.MIDIBulkOutEpSize  = pphost->device_prop.Ep_Desc[0][1].wMaxPacketSize;
		}

		MIDI_Machine.hc_num_out = USBH_Alloc_Channel(pdev,
				MIDI_Machine.MIDIBulkOutEp);
		MIDI_Machine.hc_num_in = USBH_Alloc_Channel(pdev,
				MIDI_Machine.MIDIBulkInEp);

		/* Open the new channels */
		USBH_Open_Channel  (pdev,
				MIDI_Machine.hc_num_out,
				pphost->device_prop.address,
				pphost->device_prop.speed,
				EP_TYPE_BULK,
				MIDI_Machine.MIDIBulkOutEpSize);

		USBH_Open_Channel  (pdev,
				MIDI_Machine.hc_num_in,
				pphost->device_prop.address,
				pphost->device_prop.speed,
				EP_TYPE_BULK,
				MIDI_Machine.MIDIBulkInEpSize);

		MIDI_Machine.state  = MIDI_GET_DATA;
		start_toggle =0;
		status = USBH_OK;

	}

	else
	{
		pphost->usr_cb->DeviceNotSupported();
	}

	return status ;

}


/*-----------------------------------------------------------------------------------------*/
/**
 * @brief  USBH_MIDI_InterfaceDeInit
 *         The function DeInit the Host Channels used for the MIDI class.
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval None
 */
void USBH_MIDI_InterfaceDeInit ( USB_OTG_CORE_HANDLE *pdev,
		void *phost)
{
	if ( MIDI_Machine.hc_num_out)
	{
		USB_OTG_HC_Halt(pdev, MIDI_Machine.hc_num_out);
		USBH_Free_Channel  (pdev, MIDI_Machine.hc_num_out);
		MIDI_Machine.hc_num_out = 0;     /* Reset the Channel as Free */
	}

	if ( MIDI_Machine.hc_num_in)
	{
		USB_OTG_HC_Halt(pdev, MIDI_Machine.hc_num_in);
		USBH_Free_Channel  (pdev, MIDI_Machine.hc_num_in);
		MIDI_Machine.hc_num_in = 0;     /* Reset the Channel as Free */
	}
	start_toggle = 0;
}
/*-----------------------------------------------------------------------------------------*/
/**
 * @brief  USBH_MIDI_ClassRequest
 *         The function is responsible for handling MIDI Class requests
 *         for MIDI class.
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval  USBH_Status :Response for USB Set Protocol request
 */
static USBH_Status USBH_MIDI_ClassRequest(USB_OTG_CORE_HANDLE *pdev ,
		void *phost)
{
	USBH_Status status = USBH_OK ;

	return status;
}

/*-----------------------------------------------------------------------------------------*/
/**
 * @brief  USBH_MIDI_Handle
 *         The function is for managing state machine for MIDI data transfers
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval USBH_Status
 */
static USBH_Status USBH_MIDI_Handle(USB_OTG_CORE_HANDLE *pdev ,
		void   *phost)
{
	USBH_HOST *pphost = phost;
	USBH_Status status = USBH_OK;

	//uint8_t appliStatus = 0;
	//USBH_Status status = USBH_BUSY;


	if(HCD_IsDeviceConnected(pdev))
	{
		//appliStatus = pphost->usr_cb->UserApplication(); // this will call USBH_USR_MIDI_Application()

		switch (MIDI_Machine.state)
		{

		case MIDI_GET_DATA:

			USBH_BulkReceiveData( &USB_OTG_Core_dev, MIDI_Machine.buff, 64, MIDI_Machine.hc_num_in);
			start_toggle = 1;
			MIDI_Machine.state = MIDI_POLL;
			//STM_EVAL_LEDOn(LED_Blue);

			break;

		case MIDI_POLL:

			if(HCD_GetURB_State(pdev , MIDI_Machine.hc_num_in) <= URB_DONE)
			{
				if(start_toggle == 1) /* handle data once */
				{
					start_toggle = 0;
					//MIDI_Machine.cb->Decode(MIDI_Machine.buff);
					MIDI_Decode(MIDI_Machine.buff);
					MIDI_Machine.buff[1] = 0; // the whole buffer should be cleaned...

				}
				MIDI_Machine.state = MIDI_GET_DATA;
			}
			else if(HCD_GetURB_State(pdev, MIDI_Machine.hc_num_in) == URB_STALL) /* IN Endpoint Stalled */
			{

				/* Issue Clear Feature on IN endpoint */
				if( (USBH_ClrFeature(pdev,
						pphost,
						MIDI_Machine.MIDIBulkInEp,
						MIDI_Machine.hc_num_in)) == USBH_OK)
				{
					/* Change state to issue next IN token */
					MIDI_Machine.state = MIDI_GET_DATA;

				}

			}
			break;

		default:
			break;
		}

	}

	return status;


}
/*-----------------------------------------------------------------------------------------*/
/* look up a MIDI message size from spec */
/*Return */
/* 0 : undefined message */
/* > 0 : valid message size (1-3) */
uint8_t MIDI_lookupMsgSize(uint8_t midiMsg)
{
	uint8_t msgSize = 0;

	if( midiMsg < 0xf0 ) midiMsg &= 0xf0;
	switch(midiMsg) {
	//3 bytes messages
	case 0xf2 : //system common message(SPP)
	case 0x80 : //Note off
	case 0x90 : //Note on
	case 0xa0 : //Poly KeyPress
	case 0xb0 : //Control Change
	case 0xe0 : //PitchBend Change
		msgSize = 3;
		break;

		//2 bytes messages
	case 0xf1 : //system common message(MTC)
	case 0xf3 : //system common message(SongSelect)
	case 0xc0 : //Program Change
	case 0xd0 : //Channel Pressure
		msgSize = 2;
		break;

		//1 bytes messages
	case 0xf8 : //system realtime message
	case 0xf9 : //system realtime message
	case 0xfa : //system realtime message
	case 0xfb : //system realtime message
	case 0xfc : //system realtime message
	case 0xfe : //system realtime message
	case 0xff : //system realtime message
		msgSize = 1;
		break;

		//undefine messages
	default :
		break;
	}
	return msgSize;
}

/*-----------------------------------------------------------------------------------------*/

/* Receive data from MIDI device */
uint8_t MIDI_RcvData(uint8_t *outBuf)
{
	URB_STATE urb_status;
	urb_status = HCD_GetURB_State( &USB_OTG_Core_dev , MIDI_Machine.hc_num_in);

	if(urb_status <= URB_DONE)
	{
		USBH_BulkReceiveData( &USB_OTG_Core_dev, MIDI_Machine.buff, 64, MIDI_Machine.hc_num_in);
		if( MIDI_Machine.buff[0] == 0 && MIDI_Machine.buff[1] == 0 && MIDI_Machine.buff[2] == 0 && MIDI_Machine.buff[3] == 0 )
			return 0;
		outBuf[0] = MIDI_Machine.buff[1];
		outBuf[1] = MIDI_Machine.buff[2];
		outBuf[2] = MIDI_Machine.buff[3];
		return MIDI_lookupMsgSize(MIDI_Machine.buff[1]);
	}
	else return 0;

}
/*-----------------------------------------------------------------------------------------*/
//************************************** DRAFT !!! ******************************************
/* Receive data from MIDI device */

uint8_t MIDI_RcvDataDraft(uint8_t *outBuf)
{
	//	HC_STATUS hc_state;

	//URB_STATE urb_status;

	//	//USBH_Status rcode = USBH_OK; //return code
	//
	//	hc_state = HCD_GetHCState (&USB_OTG_Core_dev , MIDI_Machine.hc_num_in);

	//urb_status = HCD_GetURB_State( &USB_OTG_Core_dev , MIDI_Machine.hc_num_in);

	//if (urb_status == URB_NOTREADY)
	//{
	//	STM_EVAL_LEDOn(LED_Red);
	//	return 0;
	//}
	//else
	//{
	USBH_BulkReceiveData( &USB_OTG_Core_dev, MIDI_Machine.buff, 64, MIDI_Machine.hc_num_in);
	if( MIDI_Machine.buff[0] == 0 && MIDI_Machine.buff[1] == 0 && MIDI_Machine.buff[2] == 0 && MIDI_Machine.buff[3] == 0 )
		return 0;
	outBuf[0] = MIDI_Machine.buff[1];
	outBuf[1] = MIDI_Machine.buff[2];
	outBuf[2] = MIDI_Machine.buff[3];
	return MIDI_lookupMsgSize(MIDI_Machine.buff[1]);
	//STM_EVAL_LEDOff(LED_Red);
	//}

	//
	//	if ((urb_status == URB_IDLE) || (urb_status == URB_DONE))
	//	{
	//		USBH_BulkReceiveData( &USB_OTG_Core_dev, MIDI_Machine.buff, 64, MIDI_Machine.hc_num_in);
	//		//USB_OTG_BSP_mDelay(1);
	//		//if all data is zero, no valid data received.
	//		if( MIDI_Machine.buff[0] == 0 && MIDI_Machine.buff[1] == 0 && MIDI_Machine.buff[2] == 0 && MIDI_Machine.buff[3] == 0 ) {
	//			return 0;
	//		}
	//		outBuf[0] = MIDI_Machine.buff[1];
	//		outBuf[1] = MIDI_Machine.buff[2];
	//		outBuf[2] = MIDI_Machine.buff[3];
	//		return MIDI_lookupMsgSize(MIDI_Machine.buff[1]);
	//	}
	//	if (urb_status == URB_ERROR) STM_EVAL_LEDOn(LED_Red);
	//	if (urb_status == URB_NOTREADY) STM_EVAL_LEDOn(LED_Blue);
	//	if (urb_status == URB_STALL) STM_EVAL_LEDOn(LED_Green);
	//	return 0;


	//	{
	//
	//		switch(State){
	//		case 0:
	//			USBH_BulkReceiveData( &USB_OTG_Core_dev, MIDI_Machine.buff, 64, MIDI_Machine.hc_num_in);
	//			State = 1;
	//			//if all data is zero, no valid data received.
	//						if( MIDI_Machine.buff[0] == 0 && MIDI_Machine.buff[1] == 0 && MIDI_Machine.buff[2] == 0 && MIDI_Machine.buff[3] == 0 ) {
	//							return 0;
	//						}
	//						outBuf[0] = MIDI_Machine.buff[1];
	//						outBuf[1] = MIDI_Machine.buff[2];
	//						outBuf[2] = MIDI_Machine.buff[3];
	//						return MIDI_lookupMsgSize(MIDI_Machine.buff[1]);
	//			return 0;
	//		case 1:
	//			switch(HCD_GetURB_State(&USB_OTG_Core_dev , MIDI_Machine.hc_num_in)){
	//			case URB_DONE:
	//
	//				State = 0;
	//				return 0;
	//			case URB_STALL:
	////				if(USBH_OK == USBH_ClrFeature(BT_State.pdev, BT_State.phost, ACLI_EP, BT_State.acli_hcn))
	////					BT_State.acli_state = 0;
	//				break;
	//			default: return 0;
	//			}
	//			break;
	//			default: return -1;
	//		}
	//		return 0;
	//	}
}



/*****************************END OF FILE****/
