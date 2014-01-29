/**
 ******************************************************************************
 * @file     phaser.c
 * @author  Xavier Halgand, thanks to Ross Bencina and music-dsp.org guys !
 * @version
 * @date    december 2012
 * @brief

 ******************************************************************************
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307, USA.
 */



#include "phaser.h"

#define MAX_RATE		7.f // in Hz
#define MIN_RATE		0.02f // in Hz

/*This defines the phaser stages
that is the number of allpass filters
 */
#define PH_STAGES 6

/*---------------------------------------------------------------------*/

static float 	old[PH_STAGES];
static float 	f_min, f_max;
static float 	swrate;
static float 	wet ;
static float	 _dmin, _dmax; //range
static float	 _fb; //feedback
static float 	_lfoPhase;
static float 	_lfoInc;
static float	_a1;
static float 	_zm1;

/*---------------------------------------------------------------------*/
void PhaserInit(void)
{
	f_min = 200.f;
	f_max = 1700.f;
	swrate = 0.1f;
	_fb = 0.7f;
	//dry = 0.7f;
	wet = 0.3f;

	_dmin = 2 * f_min / SAMPLERATE;
	_dmax = 2 * f_max / SAMPLERATE;
	_lfoInc = _2PI * swrate / SAMPLERATE;
}
/*---------------------------------------------------------------------*/
void Phaser_Rate_set(uint8_t val)
{
	swrate = (MAX_RATE - MIN_RATE) / MIDI_MAX * val + MIN_RATE;
	_lfoInc = _2PI * swrate / SAMPLERATE;
}
/*---------------------------------------------------------------------*/
void Phaser_Feedback_set(uint8_t val)
{
	_fb = 0.999f * val / MIDI_MAX;
}
/*---------------------------------------------------------------------*/
void Phaser_Wet_set(uint8_t val)
{
	wet = val / MIDI_MAX;
}
/*---------------------------------------------------------------------*/
void PhaserRate(float rate)
{
	swrate = rate;
	_lfoInc = _2PI * swrate / SAMPLERATE;
}
/*---------------------------------------------------------------------*/
void PhaserFeedback(float fdb)
{
	_fb = fdb;
}
/*---------------------------------------------------------------------*/
static float allpass(float yin, int ind)
{
	float yout;

	yout = - yin * _a1 + old[ind];
	old[ind] = yout * _a1 + yin;
	return yout;
}

/*---------------------------------------------------------------------*/
float Phaser_compute(float xin)
{
	float yout;
	int i;

	//calculate and update phaser sweep lfo...

	//float d  = _dmin + (_dmax - _dmin) * ((sinf(_lfoPhase) + 1.f)*0.5f);

	float d  = _dmin + (_dmax - _dmin) * ((sinetable[lrintf(ALPHA * _lfoPhase)] + 1.f)*0.5f);


	_lfoPhase += _lfoInc;
	if( _lfoPhase >= _2PI ) 	_lfoPhase -= _2PI;

	//update filter coeffs
	_a1 = (1.f - d) / (1.f + d);

	//calculate output

	yout = allpass(xin + _zm1 * _fb, 0);

	for(i = 1; i < PH_STAGES; i++)
	{
		yout = allpass(yout, i);
	}
	_zm1 = yout;

	yout = (1 - wet) * xin + wet * yout;

	return yout;
}

