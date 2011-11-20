#include "blsprite.h"
#include <stdlib.h>

blsprite *blsprite_new(void) { 
  return (blsprite *) malloc(sizeof(blsprite));
}

void blsprite_init(blsprite *b) {
  b->x = b->y = b->x_antigo = b->y_antigo = 0;
  b->tipo = -1;
  b->backimg = NULL;
  b->imagem  = NULL;
  b->vel_x = b->vel_y =0;
  b->forca_x = b->forca_y = 0;
}

void blsprite_finish(blsprite *b){
  
}

void blsprite_destroy(blsprite *b){
  free(b); 
  b=NULL;
}


void blsprite_move(blsprite *b, int x, int y) {
  int largura=0, altura=0;
  largura=b->imagem->w;
  altura =b->imagem->h;
  //Simplesmente atualiza as coordenadas do sprite
  b->x+=x;
  b->y+=y;
  //Verifica se o sprite está fora da tela
  if (b->x<0)   b->x=0;
  if (b->x>LARGURA_TELA-largura) b->x=LARGURA_TELA-largura;
  if (b->y<0)   b->y=0;
  if (b->y>ALTURA_TELA-altura) b->y=ALTURA_TELA-altura;
}


int blsprite_bounce_rect(blsprite *b, int x, int y, int w, int h, int use_forca) {
  //Primeiro trate dos casos onde a interseção não ocorre
  if (b->x + b->imagem->w < x) return 0;
  if (b->y + b->imagem->h < y) return 0;
  if (b->x > x + w) return 0;
  if (b->y > y + h) return 0;
  //Agora trate os casos especiais, que nunca deveriam ocorrer, mas por segurança...
  //Sprite dentro do retângulo
  if ( (b->x >= x) && (b->x+b->imagem->w <= x+w) &&
       (b->y >= y) && (b->y+b->imagem->h <= y+h) ) {
    b->vel_y = -VELOCIDADE_PADRAO;
    b->vel_x = -2*VELOCIDADE_PADRAO * (x+(w/2)-(b->x + (b->imagem->w/2))) / w;
    return 1;
  }
  //Retângulo dentro do sprite
  if ( (b->x <= x) && (b->x+b->imagem->w >= x+w) &&
       (b->y <= y) && (b->y+b->imagem->h >= y+h) ) {
    b->vel_y = -VELOCIDADE_PADRAO;
    b->vel_x = -2*VELOCIDADE_PADRAO * (x+(w/2)-(b->x + (b->imagem->w/2))) / w;
    return 1;
  }
  
  //Agora sim faça o que deve ser feito
  if (!use_forca) {//Apenas mude a força aplicada, não a velocidade
    //Sprite colide com uma borda horizontal do retângulo
    if ( !((b->y > y) && (b->y+(b->imagem->h) < y+h)) ) {
      if (b->y+(b->imagem->h/2)<=y+(h/2)) {
	b->forca_y--; //Força para cima
      } else {
	b->forca_y++; //Força para baixo
      }
    }
    //Sprite colide com uma borda vertical do retângulo    
    if ( !((b->x > x) && (b->x+(b->imagem->w) < x+w)) ) {
      if (b->x+(b->imagem->w/2)<=x+(w/2)) {
	b->forca_x--;  //Força para a esquerda
      } else {
	b->forca_x++;  //Força para a direita
      }
    }
  } else {//Mude velocidades e mude de forma diferente caso tenha batido na quina    
    if ( (b->x + (b->imagem->w/2)> x) && (b->x+(b->imagem->w)/2 < x+w) ) {
      //Não bateu na quina, pegou em uma face horizontal
      if (b->y+(b->imagem->h/2)<=y+(h/2)) {
	if(b->vel_y>0) b->vel_y = -b->vel_y; //Vá para cima
      } else  {
	if(b->vel_y<0) b->vel_y = -b->vel_y; //Vá para baixo
      }
    } else if ( (b->y +(b->imagem->h/2)> y) && (b->y+(b->imagem->h/2) < y+h) ) {
      //Não bateu na quina, pegou em uma face vertical
      if (b->x+(b->imagem->w/2)<=x+(w/2)) {
	if(b->vel_x>0) b->vel_x = -b->vel_x; //Vá para a esquerda
      } else  {
	if(b->vel_x<0) b->vel_x = -b->vel_x; //Vá para direita
      }
    } else { //Bateu na quina
      b->vel_x = -2*VELOCIDADE_PADRAO * (x+(w/2)-(b->x + (b->imagem->w/2))) / w;
      b->vel_y = -2*VELOCIDADE_PADRAO * (y+(h/2)-(b->y + (b->imagem->h/2))) / h; 
    }     
  }

  return 1;
}

void blsprite_apaga(blsprite *b, blscreen *s) {
  SDL_Rect oldrect;
  //Verificações de segurança
  if (b==NULL) return ;
  if ((b->tipo<0) || (b->tipo>=NUM_IMAGENS)) return;
  if (b->backimg==NULL) return;
  //Apaga o sprite
  oldrect.x = b->x_antigo;
  oldrect.y = b->y_antigo;
  oldrect.w = b->backimg->w;
  oldrect.h = b->backimg->h;
  SDL_BlitSurface(b->backimg, NULL, s->sdl_surface, &oldrect);
  //  SDL_BlitSurface(imagens[0], &oldrect, s->sdl_surface, &oldrect);
  //  SDL_UpdateRect(s->sdl_surface, b->x_antigo, b->y_antigo, 
  //  		 b->backimg->w, b->backimg->h);
}

void blsprite_update(blsprite *b, blscreen *s) {
  //Verificações de segurança
  if (b==NULL) return ;
  if ((b->tipo<0) || (b->tipo>=NUM_IMAGENS)) return;
  if (b->backimg==NULL) return;
  SDL_UpdateRect(s->sdl_surface, b->x, b->y, 
		 b->backimg->w, b->backimg->h);  
  SDL_UpdateRect(s->sdl_surface, b->x_antigo, b->y_antigo, 
		 b->backimg->w, b->backimg->h);   
  b->x_antigo=b->x;
  b->y_antigo=b->y;
}

void blsprite_copia_fundo(blsprite *b, blscreen *s) {
  SDL_Rect rect;
  Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0;//0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0;//0xff000000;
#endif

  //Verificações de segurança
  if (b==NULL) return ;
  if ((b->tipo<0) || (b->tipo>=NUM_IMAGENS)) return;
  //Cria o backbuffer se esse não existir
  if (b->backimg==NULL) {
    b->backimg = SDL_CreateRGBSurface(SDL_HWSURFACE, //Flags
				      b->imagem->w, //Largura
				      b->imagem->h, //Altura
				      BITS_COR, //Reolução de cores
				      rmask, gmask, bmask, amask); //Máscara de cores
    b->x_antigo=b->x;
    b->y_antigo=b->y;    
  }

  rect.x = b->x;
  rect.y = b->y;
  rect.w = b->backimg->w;
  rect.h = b->backimg->h;
  SDL_BlitSurface(s->sdl_surface, &rect, b->backimg, NULL);  
}

void blsprite_desenha(blsprite *b, blscreen *s) {
  SDL_Rect rect;
  //Verificações de segurança
  if (b==NULL) return ;
  if ((b->tipo<0) || (b->tipo>=NUM_IMAGENS)) return;
  //Apaga o sprite
  //  oldrect.x = b->x_antigo;
  //  oldrect.y = b->y_antigo;
  //  oldrect.w = b->backimg->w;
  //  oldrect.h = b->backimg->h;
  //  SDL_BlitSurface(b->backimg, NULL, s->sdl_surface, &oldrect);
  rect.x = b->x;
  rect.y = b->y;
  rect.w = b->imagem->w;
  rect.h = b->imagem->h;
  //Copia o fundo para a imagem de fundo
  //  SDL_BlitSurface(s->sdl_surface, &rect, b->backimg, NULL);  
  //Desenha o novo
  SDL_BlitSurface(b->imagem, NULL, s->sdl_surface, &rect);
  //SDL_UpdateRect(s->sdl_surface, b->x_antigo, b->y_antigo, 
  //  	 b->backimg->w, b->backimg->h);
  //    SDL_UpdateRect(s->sdl_surface, b->x, b->y, 
  //  		 b->imagem->w, b->imagem->h);

  //Atualiza valores antigos
  //  b->x_antigo=b->x;
  //  b->y_antigo=b->y;
}

void blsprite_aplica_forca(blsprite *b) {
  if (b->forca_y<0) {
    if(b->vel_y>0) b->vel_y = -b->vel_y; //Vá para cima
  } else if (b->forca_y>0) {
    if(b->vel_y<0) b->vel_y = -b->vel_y; //Vá para baixo
  }
  if (b->forca_x<0) {
    if(b->vel_x>0) b->vel_x = -b->vel_x; //Vá para a esquerda
  } else if (b->forca_x>0){
    if(b->vel_x<0) b->vel_x = -b->vel_x; //Vá para a direita
  }
  b->forca_y=b->forca_x=0;
}

