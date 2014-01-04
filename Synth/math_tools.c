/**
 ******************************************************************************
 * File Name          	: math_tools.c
 * Author				: Xavier Halgand
 * Date               	:
 * Description        	:
 ******************************************************************************
 */

#include "math_tools.h"

/*-----------------------------------------------------------------------------------*/
float approx_exp(float x)
{
	float y; // exp(x) = lim n->inf (1 + x/n)^n, here we've choosen n = 16
	y = 1.f + x/16.f ;
	y *= y;
	y *= y;
	y *= y;
	y *= y;
	return y ;
}
//------------------------------------------------------------------------------------
// input val in 0 ... 127
// output is an exponential between min and max
float	Lin2Exp(uint8_t val, float min, float max)
{
	return min * powf(max / min, val / MIDI_MAX);
}
//------------------------------------------------------------------------------------

float tanhXdX(float x)
{
	float a = x*x;
	// IIRC I got this as Pade-approx for tanh(sqrt(x))/sqrt(x)
	x = ((a + 105)*a + 945) / ((15*a + 420)*a + 945);
	return x;
}

//------------------------------------------------------------------------------------
float fastTanh(float var)
{
	if(var < -1.95f)     return -1.0f;
	else if(var > 1.95f) return  1.0f;
	else return  4.15f*var/(4.29f+var*var);
}
//------------------------------------------------------------------------------------
float 	softClipTwo(float in)
{
	return in * tanhXdX(0.5*in);
}
//------------------------------------------------------------------------------------
float 	fastTan(float x)
{
	float A = -15*x+x*x*x;
	float B = 3*(-5+2*x*x);
	return A/B;
}
