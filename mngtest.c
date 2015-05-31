#include <SDL.h>
#include "SDL_mng.h"

SDL_Surface *init_SDL(void)
{
   SDL_Surface *display;
   int video_flags;

   if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
   {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }

   if(SDL_GetVideoInfo()->hw_available)
      video_flags = SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF;
   else
      video_flags = SDL_SWSURFACE;

   SDL_WM_SetCaption("MNG Test", "MNG Test");

   if((display = SDL_SetVideoMode(400, 300, 0, video_flags)) == NULL)
      exit(1);

   return display;
}

int display_loop(SDL_Surface *display, MNG_Image *image)
{
   SDL_Event event;
   SDL_Surface *background;
   SDL_Rect src_rect, dst_rect;

   unsigned long old_time, new_time, delay;
   unsigned long elapsed_t = 0, frame = 0;

   delay = 1000 / image->mhdr.Ticks_per_second;

   if(delay < 100)
      delay = 100;

#ifdef DEBUG
   printf("frames: %i, fps: %i", image->frame_count, image->mhdr.Ticks_per_second);
#endif

   background = SDL_CreateRGBSurface(SDL_SWSURFACE, display->w, display->h,
                   32, 0, 0, 0, 0);

   SDL_FillRect(background, NULL, SDL_MapRGB(background->format, 128, 128, 128));

   src_rect.w = image->frame[0]->w;
   src_rect.h = image->frame[0]->h;
   src_rect.x = 0;
   src_rect.y = 0;

   dst_rect.w = image->frame[0]->w;
   dst_rect.h = image->frame[0]->h;
   dst_rect.x = (display->w - src_rect.w) / 2;
   dst_rect.y = (display->h - src_rect.h) / 2;

   new_time = SDL_GetTicks();

   while(1)
   {
      old_time = new_time;
      new_time = SDL_GetTicks();

      elapsed_t += new_time - old_time;

      if(elapsed_t >= delay)
      {
         elapsed_t -= delay;
         if(++frame >= image->frame_count)
            frame = 0;
      }

      SDL_BlitSurface(background, NULL, display, NULL);
      SDL_BlitSurface(image->frame[frame], NULL, display, &dst_rect);
      SDL_Flip(display);

      while(SDL_PollEvent(&event))
      {
         switch(event.type)
         {
            case SDL_KEYDOWN:
               return 0;
               break;
         }
      }
   }
}

int main(int argc, char **argv)
{
   SDL_Surface *display;
   MNG_Image *image;

   if(argc > 1)
      image = IMG_LoadMNG(argv[1]);
   else
      return 1;

   if(image != NULL)
   {
      display = init_SDL();

      return(display_loop(display, image));
   }
   return 1;
}
