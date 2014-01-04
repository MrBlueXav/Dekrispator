/**
 ******************************************************************************
 * @file    main.c
 * @author	Xavier Halgand
 * @version
 * @date
 * @brief   Dekrispator main file
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/*---------------------------------------------------------------------------*/

__ALIGN_BEGIN USB_OTG_CORE_HANDLE		USB_OTG_Core_dev __ALIGN_END  ;

__ALIGN_BEGIN USBH_HOST					USB_Host __ALIGN_END ;

bool	demoMode = true;
bool	freeze = false;

/*----------------------------------------------------------------------------*/

void ButtonPressed_action(void)
{
	if (freeze == false)
		{
			freeze = true;
			STM_EVAL_LEDOn(LED_Red);
		}
	else
		{
			freeze = false;
			STM_EVAL_LEDOff(LED_Red);
		}
}
/*----------------------------------------------------------------------------*/
void ButtonReleased_action(void)
{
	// nothing to do
}

/*====================================================================================================*/

int main(void)
{

	/* Initialize Discovery board LEDS */
	STM_EVAL_LEDInit(LED_Green);
	STM_EVAL_LEDInit(LED_Orange);
	STM_EVAL_LEDInit(LED_Red);
	STM_EVAL_LEDInit(LED_Blue);

	/* Initialize User Button */
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

	/* Setup SysTick Timer for x msec interrupts.----------------------------*/
	setSysTick();

	randomGen_init();/* Initialize the on-board random number generator ! */
	Synth_Init();
	audio_init();

	/* Choose demo or user mode --------------------------*/
	if(STM_EVAL_PBGetState(BUTTON_USER)) // press or not user button before and during startup to choose user or demo mode
	{	// normal user mode, with USB (button pressed)
		demoMode = false;
		/* Init Host Library */
		USBH_Init(&USB_OTG_Core_dev,
				USB_OTG_FS_CORE_ID,
				&USB_Host,
				&MIDI_cb,
				&USR_Callbacks);

		while (1)
		{
			/* Host Task handler */
			USBH_Process(&USB_OTG_Core_dev , &USB_Host);

			USB_OTG_BSP_mDelay(1);// how many ms ?????

		}
	}
	else
	{	// demo mode, no USB, no interaction, automatic
		demoMode = true;
		while (1);
	}
}


/************************  *****END OF FILE****/
