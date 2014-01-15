/*
 * minBLEP_osc.h
 *
 *  Created on: 11 janv. 2014
 *      Author:
 */

#ifndef MINBLEP_OSC_H_
#define MINBLEP_OSC_H_




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


#include <stdbool.h>
#include "Wave.h"
#include "MinBLEP.h"

#define MINBLEP_ZEROCROSSINGS 32
#define MINBLEP_OVERSAMPLING  256

#define VCO_SHAPE_SINE     0
#define VCO_SHAPE_TRIANGLE 1
#define VCO_SHAPE_SAW      2
#define VCO_SHAPE_SQUARE   3
#define VCO_SHAPE_IMPULSE  4


       CVCO(int sampleRate, CWave *wave, float frequency, CMinBLEP *minBLEP);
      CVCO();
  void Generate(float *modIn, float *sigOut, int samples);
  void Generate(float *modIn, float *sigOut, int samples, CWave *wave2, float *mixIn, float mixLevel);
  void Generate(float *sigOut, int samples);
  void GenerateSH(float *sigOut, int samples);
  void GenerateSG(float *sigOut, int samples);
  void Generate(float *pulseWidthIn, float *syncIn, float *fmIn, float *fmInLinear, float fmLinearScale, float *sigOut, float *syncOut, int samples);
  void GenerateBLEP(float *pwIn, float *syncIn, float *fmIn, float *sigOut, float *syncOut, int samples);
  void SetFrequency(float value);
  void SetFrequency(float value, float semitone);
  void SetFrequency(float value, float semitone, float detune);
  void SetWave(CWave *wave);
  void SetVoltage(float value);
  void SetShape(int shape);
  void AddBLEP(float offset, float amp);
  void SetPhase(float value);
  void SetSampleRate(int sampleRate);


  int       SampleRate, Shape, Resolution;
  int       BufferInit, BufferOffset, BufferCount;
  float    *Buffer;
  float     Phase, Value, Frequency, Voltage, Log440, InvLog2, Sample1, Sample2, Integrator1, Integrator2, Leaky;
  CWave    *Wave, *SinTable;
  CMinBLEP *MinBLEP, *Impulse;






#endif /* MINBLEP_OSC_H_ */
