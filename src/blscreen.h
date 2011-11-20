#ifndef __BL_SCREEN_H__
#define __BL_SCREEN_H__

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#define LARGURA_TELA   640
#define ALTURA_TELA    480
#define BITS_COR        32

typedef struct _blscreen {
  SDL_Surface *sdl_surface;
  SDL_Surface *screen_surface;
} blscreen;

blscreen *blscreen_new(void);
void blscreen_init( blscreen *s);
void blscreen_finish( blscreen *s);
void blscreen_destroy( blscreen *s);

#endif //__BL_SCREEN_H__
