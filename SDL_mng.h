/*
    SDL_mng:  A library to load MNG files
    Copyright (C) 2011, Thomas Kircher

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __SDL_mng_h__
#define __SDL_mng_h__

#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
   unsigned int Frame_width;
   unsigned int Frame_height;
   unsigned int Ticks_per_second;
   unsigned int Nominal_layer_count;
   unsigned int Nominal_frame_count;
   unsigned int Nominal_play_time;
   unsigned int Simplicity_profile;
}
MHDR_chunk;

/* MNG_Image type */
typedef struct
{
   MHDR_chunk    mhdr;
   unsigned int  frame_count;
   SDL_Surface **frame;
	  unsigned int  *frame_delay;
}
MNG_Image;

/* Check for MNG signature */
int IMG_isMNG(SDL_RWops *src);

/* Read and return an MNG image */
MNG_Image *IMG_LoadMNG(const char *file);
MNG_Image *IMG_LoadMNG_RW(SDL_RWops *src);

int IMG_FreeMNG(MNG_Image *img);

typedef struct
{
	MNG_Image *animation;
	int frame;
	unsigned long ticks;
	SDL_Rect dst;
}
MNG_AnimationState;

void IMG_SetAnimationState(MNG_AnimationState *state, int frame, int ticks);

unsigned long IMG_TimeToNextFrame(MNG_AnimationState *state, int ticks);

SDL_Surface *IMG_TimeUpdate(MNG_AnimationState *state, int ticks);

#ifdef __cplusplus
}
#endif

#endif /* __SDL_mng_h__ */
