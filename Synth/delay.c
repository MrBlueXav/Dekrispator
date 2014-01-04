/**
 ******************************************************************************
 * File Name		: delay.c
 * Author			: Xavier Halgand
 * Date				:
 * Description		: simple delay/echo effect with low pass filter in the feedback loop
 ******************************************************************************
 */

#include "delay.h"

/*-------------------------------------------------------------------------------------------*/
/* Delay effect variables  */
static float		delayline[DELAYLINE_LEN + 2 ];
static float		*readpos; // output pointer of delay line
static float      	*writepos; // input pointer of delay line
static uint32_t		shift;
static float		coeff_a1 = 0.6f; 	// coeff for the one pole low-pass filter in the feedback loop
										// coeff_a1 is between 0 and 1, 0 : no filtering, 1 : heavy filtering
static float		old_dy; //previous delayed sample
static float		fdb = FEEDB;

/*-------------------------------------------------------------------------------------------*/
void Delay_init(void)
{
	/* initialize pointers positions for delay effect */
	shift = DELAY;
	readpos = delayline;
	writepos = delayline + DELAY;
}
/*-------------------------------------------------------------------------------------------*/
void Delay_clean(void)
{
	for (int i = 0 ; i < DELAYLINE_LEN + 2; i++)
		delayline[i] = 0;
}
/*-------------------------------------------------------------------------------------------*/
void 	Delay_time_inc(uint8_t val)
{
	if (val == MIDI_MAX) {

		float 	*pos;
		if(shift < (DELAYLINE_LEN - DELTA_DELAY))
			shift += DELTA_DELAY;  //
		pos = writepos - shift;
		if (pos >= delayline)
			readpos = pos;
		else
			readpos = pos + DELAYLINE_LEN - 1;
	}
}
/*-------------------------------------------------------------------------------------------*/
void	Delay_time_dec(uint8_t val)
{
	if (val == MIDI_MAX) {

		float 	*pos;
		if(shift > (MIN_DELAY + DELTA_DELAY))  shift -= DELTA_DELAY;  //
		pos = writepos - shift;
		if (pos >= delayline)
			readpos = pos;
		else
			readpos = pos + DELAYLINE_LEN - 1;
	}
}
/*-------------------------------------------------------------------------------------------*/
void Delay_time_set(uint8_t val)
{
	float 	*pos;
	shift = (uint32_t) lrintf((DELAYLINE_LEN - 1 - MIN_DELAY)/MIDI_MAX * val + MIN_DELAY);
	pos = writepos - shift;
	if (pos >= delayline)
		readpos = pos;
	else
		readpos = pos + DELAYLINE_LEN - 1;

}
/*-------------------------------------------------------------------------------------------*/
void Delay_feedback_inc(void)
{
	/* increment feedback delay */

	fdb *= 1.05f ;//
}
/*-------------------------------------------------------------------------------------------*/
void Delay_feedback_dec(void)
{
	/* decrement feedback delay */

	fdb *= 0.95f ;//
}
/*-------------------------------------------------------------------------------------------*/
void DelayFeedback_set(uint8_t val)
{
	fdb = val / MIDI_MAX;
}
/*-------------------------------------------------------------------------------------------*/
float Delay_compute (float x)
{
	float y, dy;

	/**** insert delay effect  ****/
	// (*readpos) : delayed sample read at the output of the delay line
	dy = (1.f - coeff_a1)*(*readpos) + coeff_a1 * old_dy; // apply lowpass filter in the loop
	old_dy = dy;
	y = x + fdb*dy;

	y = (y > 1.0f) ? 1.0f : y ; //clip too loud samples
	y = (y < -1.0f) ? -1.0f : y ;
	*writepos = y; // write new computed sample at the input of the delay line

	/* update the delay line pointers : */
	writepos++;
	readpos++;

	if ((writepos - delayline) >= DELAYLINE_LEN)
		writepos = delayline; // wrap pointer

	if ((readpos - delayline) >= DELAYLINE_LEN)
		readpos = delayline;  // wrap pointer

	return y;

}
