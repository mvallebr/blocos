#include <stdlib.h>
#include "blscreen.h"


blscreen *blscreen_new(void) {
  return (blscreen *) malloc(sizeof(blscreen));
}

void blscreen_init( blscreen *s) {
  SDL_Init(SDL_INIT_EVERYTHING);
  s->sdl_surface = SDL_SetVideoMode(LARGURA_TELA, ALTURA_TELA, 
				    BITS_COR,
				    /*SDL_SRCALPHA | */SDL_ANYFORMAT
				    //);//
				| SDL_FULLSCREEN);  
  if (!(s->sdl_surface)) {
    fprintf(stderr, "Error initializing library: %s\n", SDL_GetError());
    exit(1);
  }
}

void blscreen_finish( blscreen *s) {
  SDL_FreeSurface(s->sdl_surface);
  SDL_Quit();
}

void blscreen_destroy( blscreen *s) {
  free(s);
}

