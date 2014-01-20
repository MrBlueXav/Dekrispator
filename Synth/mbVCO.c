/*
 * minBLEP_osc.c
 *
 *  Created on: 11 janv. 2014
 *      Author:
 */


/*  ExperimentalScene DarkWave Studio
    Copyright (C) 2004-2011 Daniel Werner
    http://www.experimentalscene.com/

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "mbVCO.h"
#include "Core.h"
#include "PI.h"
#include "Util.h"
#include <math.h>
#include <stdlib.h>

float	Log440, InvLog2;

float fastpow2(float x) { return pow(2.0, x); }

void mbVCO_Init(mbVCO_t *osc, int sampleRate, CWave *wave, float frequency, CMinBLEP *minBlep)
{
	osc->Resolution = 4;
	osc->SampleRate = sampleRate;
	SetWave(wave);
	Log440       = log(440.0);
	InvLog2      = 1.0f / logf(2.0f);
	SetFrequency(frequency);
	osc->Phase        = 0.0f;
	osc->Value        = 0.0f;
	osc->Sample1      = osc->Sample2 = 0.0f;
	osc->MinBLEP      = minBlep;
	//osc->Impulse      = impulse;
	osc->Buffer       = 0;
	osc->BufferOffset = 0;
	osc->BufferInit   = 0;
	osc->BufferCount  = 0;
	if(osc->MinBLEP != 0)
	{
		osc->BufferCount = (MinBLEP->ZeroCrossings * 2) + 1;
		osc->Buffer = new float[BufferCount];
	}
}


void mbVCO_Generate(mbVCO_t *osc, float *sigOut, int samples)
{
	float phaseStep, phase;

	phase = osc->Phase;
	phaseStep = osc->Frequency / ((float)osc->SampleRate);
	while(samples--)
	{
		*(sigOut++) = Wave->GetSample(phase);
		phase += phaseStep;
		if(phase >= 1.0f)
			phase -= 1.0f;
	}
	osc->Phase = phase;
}

void mbVCO_GenerateSH(mbVCO_t *osc, float *sigOut, int samples)
{
	float phaseStep, phase;

	phase = osc->Phase;
	phaseStep = osc->Frequency / ((float)osc->SampleRate);
	while(samples--)
	{
		*(sigOut++) = osc->Sample1;
		phase += phaseStep;
		if(phase >= 1.0f)
		{
			osc->Sample1 = (((float)rand() / (float)RAND_MAX) - 0.5f) * 2.0f;
			phase -= 1.0f;
		}
	}
	osc->Phase = phase;
}

void mbVCO_GenerateSG(mbVCO_t *osc, float *sigOut, int samples)
{
	float phaseStep, phase;

	phase = osc->Phase;
	phaseStep = osc->Frequency / ((float)osc->SampleRate);
	while(samples--)
	{
		*(sigOut++) = osc->Sample1 + ((osc->Sample2 - osc->Sample1) * phase);
		phase += phaseStep;
		if(phase >= 1.0f)
		{
			phase -= 1.0f;
			osc->Sample1 = osc->Sample2;
			osc->Sample2 = (((float)rand() / (float)RAND_MAX) - 0.5f) * 2.0f;
		}
	}
	osc->Phase = phase;
}



// BandLimited Waveform Generation Using MinBLEPs

void mbVCO_GenerateBLEP(mbVCO_t *osc, float *pwIn, float *syncIn, float *fmIn, float *sigOut, float *syncOut, int samples)
{
	float fs, phase, value, out;
	float baseVoltage, invSampleRate;
	float pw, freq, resetPhase;
	bool  reset;
	int   calcCounter;

	baseVoltage    = osc->Voltage;
	invSampleRate  = (1.0f / ((float)osc->SampleRate));
	phase          = osc->Phase;
	value          = osc->Value;
	freq           = osc->Frequency;
	pw             = 0.5f;
	fs             = freq * invSampleRate;
	calcCounter    = 0;

	while(samples--)
	{
		if(fmIn != 0)
		{
			if(calcCounter == 0)
			{
				freq = 440.0f * fastpow2((baseVoltage + *(fmIn++)) - 5.0f) ;

				if(freq < 1.0f)
					freq = 1.0f;
				if(freq > 0.25f * (float)osc->SampleRate)
					freq = 0.25f * (float)osc->SampleRate;
				fs = freq * invSampleRate;
				calcCounter = osc->Resolution - 1;
			}
			else
				calcCounter--;
		}

		if(pwIn != 0)
		{
			pw = (1.0f + *(pwIn++)) * 0.5f;
			if(pw < 0.0f)
				pw = 0.0f;
			else if(pw > 1.0f)
				pw = 1.0f;
		}

		phase += fs;

		if(syncIn != 0)
			if(*(syncIn++) == 0.0f)
			{
				reset      = true;
				resetPhase = 0.0f;
			}
			else
				reset = false;
		else
			reset = false;

		if(!reset)
			if(phase >= 1.0f)
			{
				reset = true;
				resetPhase = phase - 1.0f;
			}

		if(reset)
		{
			if(syncOut != 0)
				*(syncOut++) = 0.0f;
			AddBLEP(resetPhase / fs, osc->Shape == VCO_SHAPE_SAW ? phase : 1.0f);
			phase = resetPhase;
			value = 0.0f;
		}
		else
			if(syncOut != 0)
				*(syncOut++) = 1.0f;


		if(osc->Shape == VCO_SHAPE_SQUARE || osc->Shape == VCO_SHAPE_TRIANGLE)
		{
			if((value == 0.0f) && (phase > pw))
			{
				value = 1.0f;
				AddBLEP((phase - pw) / fs, -1.0f);
			}
			out = value;
		}
		else
			out = phase;


		if(osc->BufferInit != 0)
		{
			out += osc->Buffer[osc->BufferOffset];
			osc->BufferInit--;
			if(++osc->BufferOffset >= osc->BufferCount)
				osc->BufferOffset = 0;
		}

		if(osc->Shape == VCO_SHAPE_IMPULSE)
			*(sigOut++) = out;
		else
			*(sigOut++) = (2.0f * out) - 1.0f;
	}
	osc->Phase = phase;
	osc->Value = value;
}

void mbVCO_Generate(mbVCO_t *osc, float *modIn, float *sigOut, int samples)
{
	float phase, invSampleRate440, baseVoltage;

	baseVoltage = osc->Voltage;
	phase = osc->Phase;
	invSampleRate440 = (440.0f / ((float)osc->SampleRate));
	while(samples--)
	{
		*(sigOut++) = Wave->GetSample(phase);
		phase += fastpow2((*(modIn++) + baseVoltage) - 5.0f) * invSampleRate440;
		if(phase >= 1.0f)
			phase -= 1.0f;
	}
	osc->Phase = phase;
}

void mbVCO_Generate(mbVCO_t *osc, float *modIn, float *sigOut, int samples, CWave *wave2, float *mixIn, float mixLevel)
{
	float phase, invSampleRate440, baseVoltage, mix, fs;

	baseVoltage = osc->Voltage;
	phase = osc->Phase;
	invSampleRate440 = (440.0 / ((float)osc->SampleRate));
	fs = fastpow2(baseVoltage - 5.0) * invSampleRate440;
	mix = mixLevel;

	while(samples--)
	{
		if(mixIn)
			mix = *(mixIn++) * mixLevel;
		*(sigOut++) = ((1.0 - mix) * Wave->GetSample(phase)) + (mix * wave2->GetSample(phase));
		if(modIn != 0)
			fs = fastpow2((*(modIn++) + baseVoltage) - 5.0) * invSampleRate440;
		phase += fs;
		if(phase >= 1.0)
			phase -= 1.0;
	}
	osc->Phase = phase;
}

void mbVCO_SetWave(mbVCO_t *osc, CWave *wave)
{
	osc->Wave = wave;
}

void mbVCO_SetFrequency(mbVCO_t *osc, float value)
{
	osc->Frequency = value;
	osc->Voltage = ((log(value) - Log440) * InvLog2) + 5.0f;
}

void mbVCO_SetVoltage(mbVCO_t *osc, float value)
{
	osc->Voltage = value;
	osc->Frequency = fastpow2(value - 5.0f) * 440.0f;
}

void mbVCO_SetFrequency(mbVCO_t *osc, float frequency, float semitone)
{
	int   semitones;
	float ratio;

	semitones = (int)((semitone - 0.5f) * (8.0f * 12.0f));
	ratio = fastpow2(fabs((float)semitones) / 12.0f);

	if(semitones < 0)
		SetFrequency(frequency / ratio);
	else
		SetFrequency(frequency * ratio);
}

void mbVCO_SetFrequency(mbVCO_t *osc, float frequency, float semitone, float detune)
{
	int   semitones;
	float realSemitones;
	float ratio;

	semitones = (int)((semitone - 0.5f) * (8.0f * 12.0f));
	realSemitones = ((float)semitones) + detune;
	ratio = fastpow2(fabs(realSemitones) / 12.0);

	if(realSemitones < 0.0f)
		SetFrequency(frequency / ratio);
	else
		SetFrequency(frequency * ratio);
}

void mbVCO_SetShape(mbVCO_t *osc, int shape)
{
	if(osc->Shape != shape)
	{
		if(shape == VCO_SHAPE_TRIANGLE)
			osc->Leaky = 0.995f;
		else
			osc->Leaky = 0.999f;

		if(shape == VCO_SHAPE_SAW)
			osc->Integrator1 = -0.5f;
		else
			osc->Integrator1 = 0.0f;

		osc->Integrator2 = 0.0f;
		osc->Phase       = 0.0f;
		osc->Shape       = shape;
	}
}

void mbVCO_AddBLEP(mbVCO_t *osc, float offset, float amp)
{
	int       i, os;
	float    *lpOut, *lpIn, frac, *lpOutEnd, *lpInEnd, f;
	CMinBLEP *minBLEP;



	if(osc->Shape == VCO_SHAPE_TRIANGLE)
		minBLEP = Core->DSP->MinBLAR;
	else
		minBLEP = MinBLEP;


	os       = minBLEP->OverSampling;
	lpOut    = osc->Buffer + osc->BufferOffset;
	lpIn     = minBLEP->Wave->Data + (int)(os * offset);
	frac     = fmod(os * offset, 1.0);
	lpOutEnd = osc->Buffer + osc->BufferCount;
	lpInEnd  = minBLEP->Wave->Data + (minBLEP->Wave->Samples - 1);

	// add
	for (i = 0; i < osc->BufferInit && lpIn < lpInEnd; i++, lpIn += os, lpOut++)
	{
		if(lpOut >= lpOutEnd)
			lpOut = osc->Buffer;
		f = lpIn[0] + ((lpIn[1] - lpIn[0]) * frac);
		*lpOut += amp * (1.0f - f);
	}

	// copy
	for (; lpIn < lpInEnd; lpIn += os, lpOut++)
	{
		if(lpOut >= lpOutEnd)
			lpOut = osc->Buffer;
		f = lpIn[0] + ((lpIn[1] - lpIn[0]) * frac);
		*lpOut = amp * (1.0f - f);
		osc->BufferInit++;
	}
}

void mbVCO_SetPhase(mbVCO_t *osc, float value)
{
	osc->Phase = value;
}

void mbVCO_SetSampleRate(mbVCO_t *osc, int sampleRate)
{
	osc->SampleRate = sampleRate;
}

