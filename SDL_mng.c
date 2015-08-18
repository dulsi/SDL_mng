/*
   SDL_mng:  A library to load MNG files
   Copyright (C) 2011, Thomas Kircher
   
   PNG read code inspired by the libPNG and SDL_image libraries

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

#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_endian.h>
#include "SDL_mng.h"

#include <png.h>

/* Define png_jmpbuf for compatibility with older versions */
#ifndef png_jmpbuf
 #define png_jmpbuf(png_ptr) ((png_ptr)->png_jmpbuf)
#endif

/* Some select chunk IDs, from libmng.h */
#define MNG_UINT_MHDR 0x4d484452L
#define MNG_UINT_BACK 0x4241434bL
#define MNG_UINT_PLTE 0x504c5445L
#define MNG_UINT_tRNS 0x74524e53L
#define MNG_UINT_IHDR 0x49484452L
#define MNG_UINT_IDAT 0x49444154L
#define MNG_UINT_IEND 0x49454e44L
#define MNG_UINT_MEND 0x4d454e44L
#define MNG_UINT_FRAM 0x4652414dL
#define MNG_UINT_LOOP 0x4c4f4f50L
#define MNG_UINT_ENDL 0x454e444cL
#define MNG_UINT_TERM 0x5445524dL

/* Chunk structure */
typedef struct
{
   unsigned int chunk_ID;
   unsigned int chunk_size;
   char *chunk_data;
   unsigned int chunk_CRC;
}
chunk_t;

/* Internal MNG functions */
unsigned char MNG_read_byte(SDL_RWops *src);
unsigned int MNG_read_uint32(SDL_RWops *src);
chunk_t MNG_read_chunk(SDL_RWops *src);
MNG_Image *MNG_iterate_chunks(SDL_RWops *src);

typedef struct MNG_Frame
{
   unsigned int delay;
   SDL_Surface      *frame;
   struct MNG_Frame *next;
}
MNG_Frame;

/* Read one MNG frame and return it as an SDL_Surface */
SDL_Surface* MNG_read_frame(SDL_RWops *);

/* Return whether or not the file claims to be an MNG */
int IMG_isMNG(SDL_RWops *src)
{
   unsigned char buf[8];

   int start = SDL_RWtell(src);
   if ( SDL_RWread(src, buf, 1, 8) != 8 )
   {
       SDL_RWseek(src, start, RW_SEEK_SET);
       return -1;
   }

   SDL_RWseek(src, start, RW_SEEK_SET);
   return( !memcmp(buf, "\212MNG\r\n\032\n", 8) );
}

MNG_Image *IMG_LoadMNG(const char *file)
{
   SDL_RWops *src = NULL;

   src = SDL_RWFromFile(file, "rb");
   return IMG_LoadMNG_RW(src);
}

MNG_Image *IMG_LoadMNG_RW(SDL_RWops *src)
{
    if (src == NULL)
        return NULL;

    /* See whether or not this data source can handle seeking */
    if (SDL_RWseek(src, 0, SEEK_CUR) < 0)
    {
            SDL_SetError("Can't seek in this data source");
            SDL_RWclose(src);
            return NULL;
    }

    /* Verify MNG signature */
    if (!IMG_isMNG(src))
    {
            SDL_SetError("File is not an MNG file");
            SDL_RWclose(src);
            return NULL;
    }
   SDL_RWseek(src, 8, RW_SEEK_CUR);
   MNG_Image *result = MNG_iterate_chunks(src);
   SDL_RWclose(src);
   return result;
}

/* Read a byte from the src stream */
unsigned char MNG_read_byte(SDL_RWops *src)
{
   signed int ch;

   SDL_RWread(src, &ch, 1, 1);

   return((unsigned char)ch);
}

/* Read a 4-byte uint32 from the src stream */
unsigned int MNG_read_uint32(SDL_RWops *src)
{
   unsigned char buffer[4];
   unsigned int value;

   buffer[0] = MNG_read_byte(src);
   buffer[1] = MNG_read_byte(src);
   buffer[2] = MNG_read_byte(src);
   buffer[3] = MNG_read_byte(src);

   value  = buffer[0] << 24;
   value |= buffer[1] << 16;
   value |= buffer[2] << 8;
   value |= buffer[3];

   return value;
}

/* Read an MNG chunk */
chunk_t MNG_read_chunk(SDL_RWops *src)
{
   chunk_t this_chunk;

   this_chunk.chunk_size = MNG_read_uint32(src);
   this_chunk.chunk_ID = MNG_read_uint32(src);

   this_chunk.chunk_data = (char *)calloc(sizeof(char), this_chunk.chunk_size);

   SDL_RWread(src, this_chunk.chunk_data, 1, this_chunk.chunk_size);
   this_chunk.chunk_CRC = MNG_read_uint32(src);

   return this_chunk;
}

/* Read MHDR chunk data */
MHDR_chunk read_MHDR(SDL_RWops *src)
{
   MHDR_chunk mng_header;

   mng_header.Frame_width = MNG_read_uint32(src);
   mng_header.Frame_height = MNG_read_uint32(src);
   mng_header.Ticks_per_second = MNG_read_uint32(src);
   mng_header.Nominal_layer_count = MNG_read_uint32(src);
   mng_header.Nominal_frame_count = MNG_read_uint32(src);
   mng_header.Nominal_play_time = MNG_read_uint32(src);
   mng_header.Simplicity_profile = MNG_read_uint32(src);

   /* skip CRC bits */
   MNG_read_uint32(src);

   return mng_header;
}

/* Read FRAM chunk data */
unsigned int read_FRAM(chunk_t *c)
{
    unsigned int value;
    value  = ((unsigned char)c->chunk_data[6]) << 24; value |= ((unsigned char)c->chunk_data[7]) << 16;
    value |= ((unsigned char)c->chunk_data[8]) << 8;  value |= ((unsigned char)c->chunk_data[9]);
    return value;
/*    MNG_read_byte(src); // Framing mode
    MNG_read_byte(src); // Seperator
    MNG_read_byte(src); // Change interframe delay
    MNG_read_byte(src); // Change timeout and termination
    MNG_read_byte(src); // Change layer clipping boundaries
    MNG_read_byte(src); // Change sync id list
    return MNG_read_uint32(src); // Interframe delay*/
}

/* Iterate through the MNG chunks */
MNG_Image *MNG_iterate_chunks(SDL_RWops *src)
{
   chunk_t current_chunk;

   unsigned int byte_count = 0;
   unsigned int frame_count = 0;
   unsigned int frame_delay = 0;
   unsigned int i;

   MNG_Frame *start_frame = NULL;
   MNG_Frame *current_frame = NULL;

   MNG_Image *image = (MNG_Image *)malloc(sizeof(MNG_Image));

   do {
      current_chunk = MNG_read_chunk(src);
      byte_count += current_chunk.chunk_size + 12;

      switch(current_chunk.chunk_ID)
      {
         /* Read MHDR chunk, and store in image struct */
         case MNG_UINT_MHDR:
            SDL_RWseek(src, -(current_chunk.chunk_size + 4), SEEK_CUR);
            image->mhdr = read_MHDR(src);
            frame_delay = 1000 / image->mhdr.Ticks_per_second;
            break;

         /* Reset our byte count */
         case MNG_UINT_IHDR:
            byte_count = current_chunk.chunk_size + 12;
            break;

         /* We've reached the end of a PNG - seek to IHDR and read */
         case MNG_UINT_IEND:
            SDL_RWseek(src, -byte_count, SEEK_CUR);

            /* We don't know how many frames there will be, really. */
            if(start_frame == NULL)
            {
               current_frame = (MNG_Frame *)malloc(sizeof(MNG_Frame));
               start_frame = current_frame;
            }
            else
            {
               current_frame->next = (MNG_Frame *)malloc(sizeof(MNG_Frame));
               current_frame = current_frame->next;
            }

            current_frame->frame = MNG_read_frame(src);
            current_frame->delay = frame_delay;
            frame_count++;
            break;
         case MNG_UINT_FRAM:
            frame_delay = read_FRAM(&current_chunk) * 1000 / image->mhdr.Ticks_per_second;
            break;
         default:
            break;
      }
      free(current_chunk.chunk_data);
   }
   while(current_chunk.chunk_ID != MNG_UINT_MEND);

   /* Now that we're done, move the frames into our image struct */
   image->frame_count = frame_count;
   image->frame_delay = (unsigned int *)calloc(sizeof(unsigned int), frame_count);
   image->frame = (SDL_Surface **)calloc(sizeof(SDL_Surface *), frame_count);

   current_frame = start_frame;

   for(i = 0; i < frame_count; i++)
   {
      image->frame_delay[i] = current_frame->delay;
      image->frame[i] = current_frame->frame;
      current_frame = current_frame->next;
      free(start_frame);
      start_frame = current_frame;
   }
   return image;
}

/* png_read_data callback; return <size> bytes from wherever */
static void png_read_data(png_structp ctx, png_bytep area, png_size_t size)
{
   SDL_RWops *src;

   src = (SDL_RWops *)png_get_io_ptr(ctx);
   SDL_RWread(src, area, size, 1);
}

int IMG_FreeMNG(MNG_Image *img)
{
   unsigned int i;

   if(img == NULL)
      return 0;

   for(i = 0; i < img->frame_count; i++)
      SDL_FreeSurface(img->frame[i]);

   free(img->frame_delay);
   free(img->frame);
   free(img);

   return 1;
}

/* Read a PNG frame from the MNG file */
SDL_Surface *MNG_read_frame(SDL_RWops *src)
{
   SDL_Surface *volatile surface;
   Uint32 Rmask, Gmask, Bmask, Amask;

   png_structp png_ptr;
   png_infop info_ptr;
   png_uint_32 width, height;
   int bit_depth, color_type, interlace_type;
   png_bytep *volatile row_pointers;
   int row, i;

   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

   if(png_ptr == NULL)
   {
      SDL_SetError("Couldn't allocate memory for PNG file");
      return NULL;
   }

   info_ptr = png_create_info_struct(png_ptr);

   if(info_ptr == NULL)
   {
      SDL_SetError("Couldn't create image information for PNG file");
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      return NULL;
   }

   if(setjmp(png_jmpbuf(png_ptr)))
   {
      SDL_SetError("Error reading the PNG file.");
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      return NULL;
   }

   png_set_read_fn(png_ptr, src, png_read_data);
   png_set_sig_bytes(png_ptr, 8);
   png_read_info(png_ptr, info_ptr);

   png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
      &color_type, &interlace_type, NULL, NULL);

   png_set_strip_16(png_ptr) ;
   png_set_packing(png_ptr);

   if(color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_palette_to_rgb(png_ptr);

   if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
      png_set_tRNS_to_alpha(png_ptr);

   png_read_update_info(png_ptr, info_ptr);

   png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
      &color_type, &interlace_type, NULL, NULL);

   if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
   {
      Rmask = 0x000000FF;
      Gmask = 0x0000FF00;
      Bmask = 0x00FF0000;
      Amask = 0xFF000000;
   }
   else
   {
      Rmask = 0xFF000000;
      Gmask = 0x00FF0000;
      Bmask = 0x0000FF00;
      Amask = 0x000000FF;
   }

   surface = SDL_AllocSurface(SDL_SWSURFACE, width, height,
                32, Rmask, Gmask, Bmask, Amask);

   if(surface == NULL)
   {
      SDL_SetError("Out of memory");
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      return NULL;
   }

   row_pointers = (png_bytep*) malloc(sizeof(png_bytep)*height);

   if(row_pointers == NULL)
   {
      SDL_SetError("Out of memory");
      SDL_FreeSurface(surface);
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      return NULL;
   }

   for(row = 0; row < height; row++)
      row_pointers[row] = NULL;

   for(row = 0; row < (int)height; row++)
      row_pointers[row] = (png_bytep)(Uint8 *)surface->pixels + row*surface->pitch;

   png_read_image(png_ptr, row_pointers);
   png_read_end(png_ptr, info_ptr);

   png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
   if ( row_pointers )
   {
      free(row_pointers);
   }

   return(surface); 
}

void IMG_SetAnimationState(MNG_AnimationState *state, int frame, int ticks)
{
   state->frame = frame;
   state->ticks = ticks;
}

unsigned long IMG_TimeToNextFrame(MNG_AnimationState *state, int ticks)
{
   return state->animation->frame_delay[state->frame] - (ticks - state->ticks);
}

SDL_Surface *IMG_TimeUpdate(MNG_AnimationState *state, int ticks)
{
   if ((state->frame == -1) || (state->animation->frame_delay[state->frame] <= (ticks - state->ticks)))
   {
      state->frame++;
      state->frame = state->frame % state->animation->frame_count;
      state->ticks = ticks;
      return state->animation->frame[state->frame];
   }
   else
      return NULL;
}
