#include <SDL.h>
#include "SDL_mng.h"

SDL_Renderer *init_SDL(void)
{
   SDL_Renderer *display;
   int video_flags;

   if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER) < 0)
   {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }

   SDL_Window *sdlWindow = SDL_CreateWindow("MNG Test",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             400, 300,
                             0);
   if(sdlWindow == NULL)
      exit(1);

   display = SDL_CreateRenderer(sdlWindow, -1, 0);
   if(display == NULL)
      exit(1);

   return display;
}

Uint32 timerCallback(Uint32 interval, void *param)
{
 SDL_Event event;
 SDL_UserEvent userevent;

 userevent.type = SDL_USEREVENT;
 userevent.code = 0;
 userevent.data1 = NULL;
 userevent.data2 = NULL;

 event.type = SDL_USEREVENT;
 event.user = userevent;

 SDL_PushEvent(&event);
 return 0;
}

int display_loop(SDL_Renderer *display, MNG_Image *image)
{
   SDL_Event event;
   SDL_Texture *sdlTexture;
   SDL_Surface *background;
   SDL_Surface *displayFake;
   SDL_Rect src_rect;
   SDL_TimerID timer;

   unsigned long old_time, new_time, delay;
   unsigned long elapsed_t = 0, frame = 0;

   delay = 1000 / image->mhdr.Ticks_per_second;

   if(delay < 100)
      delay = 100;

#ifdef DEBUG
   printf("frames: %i, fps: %i", image->frame_count, image->mhdr.Ticks_per_second);
#endif

   sdlTexture = SDL_CreateTexture(display,
                               SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING,
                               400, 300);
   displayFake = SDL_CreateRGBSurface(0, 400, 300, 32,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);
   background = SDL_CreateRGBSurface(0, 400, 300, 32,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);

   SDL_FillRect(background, NULL, SDL_MapRGB(background->format, 128, 128, 128));

   src_rect.w = image->frame[0]->w;
   src_rect.h = image->frame[0]->h;
   src_rect.x = 0;
   src_rect.y = 0;

   MNG_AnimationState animation;
   animation.animation = image;

   animation.dst.w = image->frame[0]->w;
   animation.dst.h = image->frame[0]->h;
   animation.dst.x = (400 - src_rect.w) / 2;
   animation.dst.y = (300 - src_rect.h) / 2;
   IMG_SetAnimationState(&animation, -1, 0);
   old_time = SDL_GetTicks();

   while(1)
   {
      unsigned long ticks = SDL_GetTicks();
      SDL_Surface *nextImg = IMG_TimeUpdate(&animation, ticks);
      if (nextImg)
      {
         SDL_BlitSurface(background, NULL, displayFake, NULL);
         SDL_BlitSurface(nextImg, NULL, displayFake, &animation.dst);
         SDL_UpdateTexture(sdlTexture, NULL, displayFake->pixels, 400 * sizeof (Uint32));
         SDL_RenderClear(display);
         SDL_RenderCopy(display, sdlTexture, NULL, NULL);
         SDL_RenderPresent(display);
      }
      unsigned long animationDelay = IMG_TimeToNextFrame(&animation, ticks);
      timer = SDL_AddTimer(animationDelay, timerCallback, NULL);

      SDL_WaitEvent(&event);
      SDL_RemoveTimer(timer);
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
   SDL_Renderer *display;
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
