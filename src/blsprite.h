#ifndef __blsprite_h__
#define __blsprite_h__

#include "main.h"
#include "blscreen.h"

#define VELOCIDADE_PADRAO 4

typedef struct _blsprite {
  //Imagem desse sprite
  SDL_Surface *imagem;    //Imagem do sprite
  //Backbuffer
  SDL_Surface *backimg;   //Buffer de mem�ria para guardar o fundo
  int tipo;               //Tipo de sprite
  int x, y;               //Posi��o atual
  int x_antigo, y_antigo; //Posi��o no desenho anterior
  int vel_x, vel_y;       //Velocidades em x e em y, em pixels
  int forca_x, forca_y;
} blsprite;

blsprite *blsprite_new(void);
void blsprite_init(blsprite *b);
void blsprite_finish(blsprite *b);
void blsprite_destroy(blsprite *b);
void blsprite_desenha(blsprite *b, blscreen *s);  //Desenha o sprite na tela
void blsprite_apaga(blsprite *b, blscreen *s);  //Apaga o sprite da tela
void blsprite_update(blsprite *b, blscreen *s);  //Atualiza a tela na regi�o do sprite
void blsprite_move(blsprite *b, int x, int y); //Movimenta o sprite em x e y
//Ajusta velocidade do sprite de acordo com a intersec��o com o ret�ngulo dado
//Retorna 1 caso haja colis�o e 0 se n�o houver
int blsprite_bounce_rect(blsprite *b, int x, int y, int w, int h, int use_forca); 
//Copia imagem de fundo para backimg do sprite
void blsprite_copia_fundo(blsprite *b, blscreen *s);
//Aplica a for�a acumulada
void blsprite_aplica_forca(blsprite *b);
#endif //__blsprite_h__
