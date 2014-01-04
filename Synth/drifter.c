/**
 ******************************************************************************
 * File Name          : drifter.c
 * Author			  : Xavier Halgand
 * Date               :
 * Description        : random segment waveform generator
 ******************************************************************************
 */

#include "drifter.h"

/*-------------------------------------------------------------------------------------------*/

Drifter_t 	d1 _CCM_ ;
Drifter_t	d2 _CCM_ ;
Drifter_t	d3 _CCM_ ;

/*-------------------------------------------------------------------------------------------*/
void	Drifter_amp_set(uint8_t val)
{
	d1.gain = d2.gain = d3.gain = (val / MIDI_MAX)*(val / MIDI_MAX);

}
void	Drifter_minFreq_set(uint8_t val)
{
	d1.fmin = d2.fmin = d3.fmin = .1f + 50 * val / MIDI_MAX;

}
void	Drifter_maxFreq_set(uint8_t val)
{
	float x;

	x = (1 + 19 * val / MIDI_MAX )* d1.fmin ;
	if (x > 10000)
		d1.fmax = d2.fmax = d3.fmax = 10000;
	else {
		d1.fmax = d2.fmax = d3.fmax = x;
	}
}

/*-------------------------------------------------------------------------------------------*/
void drifter_newSegment(Drifter_t *d) //
{
	d->n = 0;
	d->initial = d->final;
	d->minLength = 0.5f * Fs / d->fmax ;
	d->maxLength = 0.5f * Fs / d->fmin ;
	d->length = frand_a_b(d->minLength,d->maxLength);
	d->final = frand_a_b(-1, 1);
	d->slope = (d->final - d->initial) / d->length ;
}
/*-------------------------------------------------------------------------------------------*/
float drifter_nextSample(Drifter_t *d) //
{
	d->out = d->gain * (d->slope * d->n + d->initial);
	(d->n)++;
	if (d->n >= d->length)
		{
			drifter_newSegment(d);
		}
	return d->out;
}
/*-------------------------------------------------------------------------------------------*/
float d1_drifter_nextSample(void) //
{
	return drifter_nextSample(&d1);
}
/*-------------------------------------------------------------------------------------------*/
float d2_drifter_nextSample(void) //
{
	return drifter_nextSample(&d2);
}
/*---------------------------------------------------------------------------------------------*/
void drifter_prepare(Drifter_t *d)
{
	d->final = 0;
	d->fmax = 4;
	d->fmin = 2;
	d->gain = .01f;

	drifter_newSegment(d);
}
/*---------------------------------------------------------------------------------------------*/
void drifter_init(void)
{
	drifter_prepare(&d1);
	drifter_prepare(&d2);
	drifter_prepare(&d3);
}
/*------------------------------------------END--------------------------------------------*/

