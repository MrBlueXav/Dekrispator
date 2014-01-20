/* blepvco - minBLEP-based, hard-sync-capable LADSPA VCOs.
 *
 * Copyright (C) 2004-2005 Sean Bolton.
 *
 * Much of the LADSPA framework used here comes from VCO-plugins
 * 0.3.0, copyright (c) 2003-2004 Fons Adriaensen.
 *
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

//-----------------------------------------------------------------------------------
// Common definitions


#include "blepvco.h"

#define NMODS 3
#define VERSION "0.1.0"


static const char* maker = "Sean Bolton <musound AT jps DOT net>";
static const char* copyright = "(C) 2005 Sean Bolton, GNU General Public License Version 2 applies";


static void pconnect (LADSPA_Handle H, unsigned long port, LADSPA_Data *data)
{
  ((LadspaPlugin *)H)->setport (port, data);
}

static void activate (LADSPA_Handle H)
{
  ((LadspaPlugin *)H)->active (true);
}

static void runplugin (LADSPA_Handle H, unsigned long k)
{
  ((LadspaPlugin *)H)->runproc (k, false);
}

//static void runadding (LADSPA_Handle H, unsigned long k)
//{
//  ((LadspaPlugin *)H)->runproc (k, true);
//}

//static void setadding (LADSPA_Handle H, LADSPA_Data gain)
//{
//  ((LadspaPlugin *)H)->setgain (gain);
//}

static void deactivate (LADSPA_Handle H)
{
  ((LadspaPlugin *)H)->active (false);
}

static void cleanup (LADSPA_Handle H)
{
  delete (LadspaPlugin *) H;
}


//-----------------------------------------------------------------------------------
// Plugin definitions


static const char* name_blepsaw = "Sync-Saw-VCO  --  Hard-sync-capable anti-aliased oscillator";
static const char* label_blepsaw  = "Sync-Saw-VCO";

static LADSPA_Handle instant_blepsaw (const struct _LADSPA_Descriptor *desc, unsigned long rate)
{
  return new Ladspa_VCO_blepsaw (rate);
}
 
static const char * const pname_blepsaw  [Ladspa_VCO_blepsaw::NPORT] = 
{
  "Output",
  "Sync Out",
  "Frequency",
  "Exp FM",
  "Lin FM", 
  "Sync In",
  "Octave",
  "Tune",
  "Exp FM gain",
  "Lin FM gain",
  "LP filter"
};

static const LADSPA_PortDescriptor pdesc_blepsaw [Ladspa_VCO_blepsaw::NPORT] = 
{
  LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO,
  LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL
};

static const LADSPA_PortRangeHint phint_blepsaw [Ladspa_VCO_blepsaw::NPORT] = 
{
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0 | LADSPA_HINT_INTEGER, -4, 4 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, 0, 1 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, 0, 4 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, 0, 4 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE, 0, 1 }
};


static const char* name_bleprect = "Sync-Rect-VCO  --  Hard-sync-capable anti-aliased oscillator";
static const char* label_bleprect  = "Sync-Rect-VCO";

static LADSPA_Handle instant_bleprect (const struct _LADSPA_Descriptor *desc, unsigned long rate)
{
  return new Ladspa_VCO_bleprect (rate);
}
 
static const char * const pname_bleprect  [Ladspa_VCO_bleprect::NPORT] = 
{
  "Output",
  "Sync Out",
  "Frequency",
  "Exp FM",
  "Lin FM", 
  "Wave Mod",
  "Sync In",
  "Octave",
  "Tune",
  "Exp FM gain",
  "Lin FM gain",
  "Waveform",
  "WMod gain",
  "LP filter"
};

static const LADSPA_PortDescriptor pdesc_bleprect [Ladspa_VCO_bleprect::NPORT] = 
{
  LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO,
  LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL
};

static const LADSPA_PortRangeHint phint_bleprect [Ladspa_VCO_bleprect::NPORT] = 
{
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0 | LADSPA_HINT_INTEGER, -4, 4 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, 0, 1 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, 0, 4 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, 0, 4 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, -1, 1 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, 0, 4 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE, 0, 1 }
};


static const char* name_bleptri = "Sync-Tri-VCO  --  Hard-sync-capable anti-aliased oscillator";
static const char* label_bleptri  = "Sync-Tri-VCO";

static LADSPA_Handle instant_bleptri (const struct _LADSPA_Descriptor *desc, unsigned long rate)
{
  return new Ladspa_VCO_bleptri (rate);
}
 
static const char * const pname_bleptri  [Ladspa_VCO_bleptri::NPORT] = 
{
  "Output",
  "Sync Out",
  "Frequency",
  "Exp FM",
  "Lin FM", 
  "Wave Mod",
  "Sync In",
  "Octave",
  "Tune",
  "Exp FM gain",
  "Lin FM gain",
  "Waveform",
  "WMod gain",
  "LP filter"
};

static const LADSPA_PortDescriptor pdesc_bleptri [Ladspa_VCO_bleptri::NPORT] = 
{
  LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO,
  LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_AUDIO,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL,
  LADSPA_PORT_INPUT  | LADSPA_PORT_CONTROL
};

static const LADSPA_PortRangeHint phint_bleptri [Ladspa_VCO_bleptri::NPORT] = 
{
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0 | LADSPA_HINT_INTEGER, -4, 4 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, 0, 1 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, 0, 4 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, 0, 4 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, -1, 1 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0, 0, 4 },
  { LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE, 0, 1 }
};


static const LADSPA_Descriptor moddescr [NMODS] =
{
/* Some LADSPA real-time hosts complain if a plugin does not have
 * LADSPA_PROPERTY_HARD_RT_CAPABLE, but it would not be technically
 * correct to set it here, since the plugin execution time
 * increases with oscillator frequency, and when hard-sync is used,
 * with sync frequency as well. */
  {
    2184,
    label_blepsaw,
    0 /* LADSPA_PROPERTY_HARD_RT_CAPABLE */,
    name_blepsaw,
    maker,
    copyright,
    Ladspa_VCO_blepsaw::NPORT,
    pdesc_blepsaw,
    pname_blepsaw,
    phint_blepsaw,
    0,
    instant_blepsaw,
    pconnect,
    activate,
    runplugin,
    0,
    0,
    deactivate,
    cleanup
  },
  {
    2185,
    label_bleprect,
    0 /* LADSPA_PROPERTY_HARD_RT_CAPABLE */,
    name_bleprect,
    maker,
    copyright,
    Ladspa_VCO_bleprect::NPORT,
    pdesc_bleprect,
    pname_bleprect,
    phint_bleprect,
    0,
    instant_bleprect,
    pconnect,
    activate,
    runplugin,
    0,
    0,
    deactivate,
    cleanup
  },
  {
    2186,
    label_bleptri,
    0 /* LADSPA_PROPERTY_HARD_RT_CAPABLE */,
    name_bleptri,
    maker,
    copyright,
    Ladspa_VCO_bleptri::NPORT,
    pdesc_bleptri,
    pname_bleptri,
    phint_bleptri,
    0,
    instant_bleptri,
    pconnect,
    activate,
    runplugin,
    0,
    0,
    deactivate,
    cleanup
  }
};

extern "C" const LADSPA_Descriptor *ladspa_descriptor (unsigned long i)
{
  if (i >= NMODS) return 0;
  return moddescr + i;
}

//-----------------------------------------------------------------------------------

