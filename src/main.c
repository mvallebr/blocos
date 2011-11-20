#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blscreen.h"
#include "blsprite.h"
#include "main.h"
#include "SFont.h"
#include <dirent.h>

//Variáveis globais para esse arquivo .c
blscreen *s;  
SDL_Surface *imagens[NUM_IMAGENS];
Mix_Chunk   *sons[NUM_SONS];
char *nome_imgs[] = {"fundo0001.png",   "barreira0001.png",
		     "spritep0001.png", "sprite0001.png",  "spriteg0001.png", 
		     "bloco0001.png",   "bloco0002.png",   "bloco0003.png", 
		     "bloco0004.png",   "bola0001.png",    "bolasp0001.png",
		     "bonus0001.png",   "bonus0002.png",   "bonus0003.png", 
		     "bonus0004.png",   "bonus0005.png",   "bonus0006.png"
};  
char *nome_sons[] = {"colnivel.wav",   "colbloco.wav",
		     "coljogad.wav", "ganhou.wav",  "perdeu.wav"		     
};
blsprite *blocos[NUM_BLOCOS_X*NUM_BLOCOS_Y]; //ponteiros para os blocos do jogo
blsprite *jogador;             //Sprite controlado pelo jogador
blsprite *bolas[MAX_BOLAS];    //Ponteiros para as bolas
int estado_da_bola[MAX_BOLAS];//Estado atual de cada bola
int num_bolas=0;              //Número de bolas no jogo    
char caminho_niveis[1024]; //Guarda caminho completo dos arquivos de nível

int main(int argc, char *argv[]) {
  //Declaração de variáveis
  int i=0, j=0;
  int sair=0; //Flag que indica se o programa deve sair do loop principal
  int modo=MODO_MENU, desenhar=1;
  SDL_Event event;
  SDL_Surface *fonte;
  SDL_Rect rect;
  DIR *dirniveis;
  char nome_arquivo[1024];   //Guarda caminho completo de cada arquivo  
  char niveis[MAX_NUM_NIVEIS][1024];
  int num_niveis=0, primeiro_nivel=0, niveis_mostrados=0, niveis_offsety=0;
  int num_vezes_vel_x_igual=0, num_vezes_vel_y_igual=0;
  struct dirent *arquivos;  
  //Variáveis de controle
  int velocidade_jogador=0;
  Uint32 tempo_atual=0;
  //---------------------------------------------
  //Inicializa audio
  Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
		MIX_DEFAULT_CHANNELS,
		2048);
/*  Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
		MIX_DEFAULT_CHANNELS,
		256);*/
  //Seta o caminho dos níveis
  strcpy(caminho_niveis, CAMINHO_DO_JOGO);
  strcat(caminho_niveis, "niveis/");
  //Constroi a lista de níveis
  dirniveis = opendir(caminho_niveis);
/*
  if (dirniveis==NULL) {
    fprintf(stderr, "Caminho inválido: %s\n", caminho_niveis);
    return -1;
  }
  do {
    arquivos = readdir(dirniveis);
    if (arquivos==NULL) break;   //Fim da lista de arquivos no diretório
    //if (arquivos->d_type!=8) continue;        //Não é arquivo, pule
    if (strlen(arquivos->d_name)<5) continue; //Não tem extensão, pule
    //A extensão não é .blc, pule
    if (strcmp((arquivos->d_name)+strlen(arquivos->d_name)-4, ".blc")) continue;
    //Arquivo deve ser um nível. Adicione à lista
    strcpy(niveis[num_niveis], arquivos->d_name);
    //    niveis[num_niveis][strlen(arquivos->d_name)-4]='\0';
    printf("Nível %d (%s) adicionado à lista. \n", num_niveis, niveis[num_niveis]);
    num_niveis++;
  } while(num_niveis<MAX_NUM_NIVEIS);
*/
  strcpy(niveis[0], "../niveis/nivel1.blc");
  strcpy(niveis[1], "../niveis/nivel2.blc");
  strcpy(niveis[2], "../niveis/nivel3.blc");
  strcpy(niveis[3], "../niveis/nivel4.blc");
  strcpy(niveis[4], "../niveis/nivel5.blc");
num_niveis=5;
  closedir(dirniveis);
  //-------------------------------------------------------
  //Aloca memória e inicializa
  s = blscreen_new();
  blscreen_init(s);
  //Inicializa fonte
  fonte = IMG_Load(ARQUIVO_DE_FONTE);
  InitFont(fonte);
  //Carrega imagens
  for (i=0; i<NUM_IMAGENS; i++) {
    strcpy(nome_arquivo, CAMINHO_DO_JOGO);
    strcat(nome_arquivo, "imgs/");
    strcat(nome_arquivo, nome_imgs[i]);
    printf("Carregando imagem do arquivo: '%s'\n", nome_arquivo);
    imagens[i] = IMG_Load(nome_arquivo);
    if (!imagens[i]) {
      fprintf(stderr, "Não pude carregar imagem: '%s'\n", nome_arquivo);
      return -2;
    }
  }  
  //Carrega sons
  for (i=0; i<NUM_SONS; i++) {
    strcpy(nome_arquivo, CAMINHO_DO_JOGO);
    strcat(nome_arquivo, "sons/");
    strcat(nome_arquivo, nome_sons[i]);
    printf("Carregando som do arquivo: '%s'\n", nome_arquivo);
    sons[i] = Mix_LoadWAV(nome_arquivo);
    if (!sons[i]) {
      fprintf(stderr, "Não pude carregar som: '%s'\n", nome_arquivo);
      return -3;
    }
  }  
  //inicializa jogador
  jogador = blsprite_new();
  blsprite_init(jogador);
  jogador->imagem = imagens[SPR_JOGADOR_NATURAL];
  jogador->tipo = SPR_JOGADOR_NATURAL;
  jogador->y    = ALTURA_TELA-(3*20);
  jogador->x    = (LARGURA_TELA - jogador->imagem->w)/2;
  //-------------------------------------------------------
  //Loop principal do programa
  do {
    while(tempo_atual+TEMPO_FRAME>SDL_GetTicks());//Espera tempo para próximo frame
    tempo_atual = SDL_GetTicks(); //Atualiza tempo atual
    // Flag desenhar ativado, redesenhe a tela inteira
    if (desenhar) { 
      desenhar = 0;
      //Primeiro desenha o fundo e tudo o que é fixo.
      rect.x = 0;
      rect.y = 0;
      rect.w = imagens[SPR_FUNDO]->w;
      rect.h = imagens[SPR_FUNDO]->h;
      SDL_BlitSurface(imagens[SPR_FUNDO], NULL, s->sdl_surface, &rect); 

      switch(modo) {
      case MODO_MENU:   //-------------------------------------------------------
	PutString(s->sdl_surface, 200, 200, "ENTER - Escolher nivel e jogar");
	PutString(s->sdl_surface, 200, 280, "ESC   - Sair");
	break;
      case MODO_NIVEL:  //-------------------------------------------------------
	//Calcula quantos nomes devem ser exibidos na tela
	niveis_mostrados = num_niveis - primeiro_nivel;
	niveis_mostrados = (niveis_mostrados<=MAX_NIVEIS_MOSTRADOS) 
	  ? niveis_mostrados
	  : MAX_NIVEIS_MOSTRADOS;
	//Exibe a partir do primeiro	
	niveis_offsety = OFFSET_Y + 20;
	//O nível a ser carregado é o primeiro nível
	PutString(s->sdl_surface, 80, niveis_offsety, "Nivel selecionado  --->");	
	for (i=primeiro_nivel; i<primeiro_nivel+niveis_mostrados; i++) {
	  PutString(s->sdl_surface, 280, niveis_offsety, niveis[i]);
	  niveis_offsety+=30;
	}
	break;
      case MODO_JOGO:  //-------------------------------------------------------
	for(i=0; i<NUM_BLOCOS_X*NUM_BLOCOS_Y; i++) 
	  if (blocos[i]) {
	    blsprite_copia_fundo(blocos[i], s);
	    blsprite_desenha(blocos[i], s);
	  }	
	//Desenha jogador
	blsprite_copia_fundo(jogador, s);
	blsprite_desenha(jogador, s);
	//Cria a primeira bola no jogo, caso não exista
	if (num_bolas<=0)
	  main_nova_bola();      
	//Desenha bolas
	for (i=0; i<num_bolas; i++){
	  blsprite_copia_fundo(bolas[i], s);
	  blsprite_desenha(bolas[i], s);
	  blsprite_update(bolas[i], s);
	}
 	break;
      default: //Nunca deveria ocorrer
	sair=1;
	break;
      }
      SDL_UpdateRect(s->sdl_surface, 0, 0, LARGURA_TELA, ALTURA_TELA);
    }
    //-------------------------------------------------------
    //Atualiza a tela
    if ( SDL_MUSTLOCK(s->sdl_surface) ) {
      SDL_UnlockSurface(s->sdl_surface);
    }
    //    SDL_UpdateRect(s->sdl_surface, 0, 0, LARGURA_TELA, ALTURA_TELA);
    //-------------------------------------------------------
    //Recebe entrada de teclado
    switch(modo) {
    case MODO_MENU:   //-------------------------------------------------------
      //case menu
      while( SDL_PollEvent( &(event) ) ){	
	switch (event.type) {
	case SDL_KEYDOWN:      
	  switch(((SDL_KeyboardEvent *)(&(event)))->keysym.sym){	
	  case SDLK_ESCAPE:
	    sair=1; //Sai do jogo
	    break;
	  case SDLK_RETURN:
	    modo=MODO_NIVEL; //Escolhe o nível
	    desenhar=1;	    
	    break;
	  default:
	    break;
	  }
	  break;
	case SDL_QUIT:
	  sair=1;
	  break;
	}
      }
      //end case menu
      break;      
      // case niveis     
    case MODO_NIVEL:  //-------------------------------------------------------
      while( SDL_PollEvent( &(event) ) ){	
	switch (event.type) {
	case SDL_KEYDOWN:      
	  switch(((SDL_KeyboardEvent *)(&(event)))->keysym.sym){
	  case SDLK_DOWN:
	    if (primeiro_nivel<num_niveis-1) primeiro_nivel++;
	    desenhar=1;
	    break;
	  case SDLK_UP:
	    if (primeiro_nivel>0) primeiro_nivel--;
	    desenhar=1;
	    break;
	  case SDLK_ESCAPE:
	    modo=MODO_MENU; //Volta para o menu
	    desenhar=1;
	    break;
	  case SDLK_RETURN:
	    //Carrega o primeiro_nivel e entra no jogo
	    if (main_carrega_nivel(niveis[primeiro_nivel]) ) {
	      modo=MODO_JOGO; 
	      desenhar=1;
	    }
	    break;
	  default:
	    break;
	  }
	  break;
	case SDL_QUIT:
	  sair=1;
	  break;
	}
      }
      //end case niveis
      break;
      //case jogo
    case MODO_JOGO:  //-------------------------------------------------------
      while( SDL_PollEvent( &(event) ) ){	
	switch (event.type) {
	case SDL_KEYDOWN:      
	  switch(((SDL_KeyboardEvent *)(&(event)))->keysym.sym){
	  case SDLK_LEFT:
	    velocidade_jogador-=VEL_JOGADOR;
	    break;
	  case SDLK_RIGHT:
	    velocidade_jogador+=VEL_JOGADOR;
	    break;
	  case SDLK_SPACE:
	    //Joga todas as bolas paradas
	    for (i=0; i<num_bolas; i++) {
	      estado_da_bola[i] = BOLA_MOVIMENTO;
	      if ( bolas[i]->vel_y==0)
		bolas[i]->vel_y=-VELOCIDADE_PADRAO;
	      bolas[i]->vel_x=-3;
	    }
	    break;
	  case SDLK_ESCAPE:
	    //Volta para a escolha de níveis
	    modo=MODO_NIVEL;
	    main_descarrega_nivel();
	    desenhar=1;
	    break;
	  default:
	    break;
	  }
	  break;
	case SDL_KEYUP:
	  switch(((SDL_KeyboardEvent *)(&(event)))->keysym.sym){
	  case SDLK_LEFT:
	    velocidade_jogador+=VEL_JOGADOR;
	    break;
	  case SDLK_RIGHT:
	    velocidade_jogador-=VEL_JOGADOR;
	    break;
	  default:
	    break;
	  }
	  break;
	case SDL_QUIT:
	  sair=1;
	  break;
	}
      }
      //end case jogo
      break;
      //default case
    default: //Nunca deveria ocorrer
      sair=1;
      break;
      //end default case
    }

    if (modo==MODO_JOGO) {
      //Certifica que o jogador está na região permitida
      blsprite_move(jogador, velocidade_jogador, 0);
      if (jogador->x<OFFSET_X) jogador->x=OFFSET_X;
      if (jogador->x > LARGURA_TELA - OFFSET_X - jogador->imagem->w)
	jogador->x = LARGURA_TELA - OFFSET_X - jogador->imagem->w;
      //Apaga tudo
      blsprite_apaga(jogador, s);
      for (i=num_bolas-1; i>=0; i--)
	blsprite_apaga(bolas[i], s);
      //Processa cada bola
      for (i=0; i<num_bolas; i++) {
	if (estado_da_bola[i]==BOLA_MOVIMENTO) {//Bola deve ser processada
	  //Detecta colisão com cada bloco
	  for (j=0; j<NUM_BLOCOS_X*NUM_BLOCOS_Y; j++) 
	    if (blocos[j]!=NULL){
	      if (blsprite_bounce_rect(bolas[i], 
				       blocos[j]->x, blocos[j]->y,
				       blocos[j]->imagem->w,
				       blocos[j]->imagem->h, 0)) {
		Mix_PlayChannel(-1,sons[SOM_COLISAO_BLOCO],0);
		//Detectou com o bloco. Vejamos o que deve acontecer com o bloco:
		switch (blocos[j]->tipo) {
		case SPR_BLOCO_0:
		  //Bloco deve ser destruido e bônus lançado, caso haja algum.
		  //Ao destruir o sprite, tomar cuidado para que o backimg da bola
		  //não fique errado. apague a bola primeiro, apague o sprite e então
		  //atualize o backimg da bola. Lembre-se que a ordem ao copiar os sprites
		  //para a tela e ao copiar parte da tela para os sprites é muito
		  //importante.
		  blsprite_apaga(bolas[i], s);
		  blsprite_apaga(blocos[j], s);
		  blsprite_copia_fundo(bolas[i], s);
		  blsprite_update(blocos[j], s);
		  blsprite_finish(blocos[j]);
		  blsprite_destroy(blocos[j]);
		  blocos[j]=NULL;
		  break;
		case SPR_BLOCO_1:
		  //Transforme-se em bloco 0
		  blsprite_apaga(bolas[i], s);
		  blsprite_apaga(blocos[j], s);
		  blsprite_copia_fundo(bolas[i], s);
		  blocos[j]->tipo = SPR_BLOCO_0;
		  blocos[j]->imagem = imagens[SPR_BLOCO_0];
		  blsprite_copia_fundo(blocos[j], s);
		  blsprite_desenha(blocos[j], s);
		  blsprite_update(blocos[j], s);
		break;
		case SPR_BLOCO_2:
		  //Transforme-se em bloco 1
		  blsprite_apaga(bolas[i], s);
		  blsprite_apaga(blocos[j], s);
		  blsprite_copia_fundo(bolas[i], s);
		  blocos[j]->tipo = SPR_BLOCO_1;
		  blocos[j]->imagem = imagens[SPR_BLOCO_1];
		  blsprite_copia_fundo(blocos[j], s);
		  blsprite_desenha(blocos[j], s);
 		  blsprite_update(blocos[j], s);
		  break;
		case SPR_BLOCO_3: //Bloco indestrutível, não faz nada
		default:
		  break;
		}
	      }
	    }
	  blsprite_aplica_forca(bolas[i]);
	  //Detecta colisão com bloco do jogador 
	  if (blsprite_bounce_rect(bolas[i], jogador->x, jogador->y,
				   jogador->imagem->w, jogador->imagem->h, 1) ) {
	    Mix_PlayChannel(-1,sons[SOM_COLISAO_JOGADOR],0);
	  }
	  //Detecta situação de cascata
	  if (bolas[i]->x==((blsprite *)bolas[i])->x_antigo)
	    num_vezes_vel_x_igual++;
	  if (((blsprite *)bolas[i])->y==((blsprite *)bolas[i])->y_antigo)
	    num_vezes_vel_y_igual++;
	  if (num_vezes_vel_x_igual>300) {
	    num_vezes_vel_x_igual=0;
	    ((blsprite *)bolas[i])->x-=1;
	  }
	  if (num_vezes_vel_y_igual>300) {
	    num_vezes_vel_y_igual=0;
	    ((blsprite *)bolas[i])->y-=1;
	  }

	  //Move de acordo com a velocidade
	  blsprite_move((blsprite *)bolas[i], ((blsprite *)bolas[i])->vel_x,
			((blsprite *)bolas[i])->vel_y);  
	  //Verifica se colidiu aos lados
	  if ( (((blsprite *)bolas[i])->x < OFFSET_X) ){	
	    Mix_PlayChannel(-1,sons[SOM_COLISAO_NIVEL],0);    
	    ((blsprite *)bolas[i])->vel_x = -((blsprite *)bolas[i])->vel_x;
	    ((blsprite *)bolas[i])->x = OFFSET_X;
	  }
	  if ((((blsprite *)bolas[i])->x + ((blsprite *)bolas[i])->imagem->w > 
	       LARGURA_TELA - OFFSET_X) ) {
	    Mix_PlayChannel(-1,sons[SOM_COLISAO_NIVEL],0);    
	    ((blsprite *)bolas[i])->x = LARGURA_TELA - OFFSET_X 
	      -((blsprite *)bolas[i])->imagem->w;
	    ((blsprite *)bolas[i])->vel_x = -((blsprite *)bolas[i])->vel_x;
	  }
	  //Verifica se colidiu acima
	  if ( (((blsprite *)bolas[i])->y < OFFSET_Y) )
	    if (((blsprite *)bolas[i])->vel_y<0) {
	      Mix_PlayChannel(-1,sons[SOM_COLISAO_NIVEL],0);    
	      ((blsprite *)bolas[i])->vel_y = -((blsprite *)bolas[i])->vel_y;
	    }
	  //Verifica se colidiu abaixo
	  if ( (((blsprite *)bolas[i])->y + ((blsprite *)bolas[i])->imagem->h >= 
		ALTURA_TELA) ) {
	    Mix_PlayChannel(-1,sons[SOM_PERDEU],0);    
	    main_apaga_bola(i);
	    main_nova_bola();
	  }

	} else { //BOLA_PARADA - Move bola para junto do jogador
	  ((blsprite *)bolas[i])->x    = jogador->x + 
	    (jogador->imagem->w - ((blsprite *)bolas[i])->imagem->w)/2;
	}
	blsprite_copia_fundo(bolas[i], s);
	blsprite_desenha(bolas[i], s);
	blsprite_update(bolas[i], s);
      }
      blsprite_copia_fundo(jogador, s);
      blsprite_desenha(jogador, s);
      blsprite_update(jogador, s);
    }    
  } while (!sair);
  //-------------------------------------------------------
  //Termina, libera a memória e sai do programa normalmente
  blsprite_finish(jogador);
  blsprite_destroy(jogador);
  blscreen_finish(s);
  blscreen_destroy(s);
  Mix_CloseAudio();
  return 0;
}

int main_carrega_nivel(char *nome) {
  char nome_completo[1024];
  FILE *f;
  int i, indice, num_lidos;
  
  strcpy(nome_completo, caminho_niveis);
  strcat(nome_completo, nome);
  printf("Carregando nível %s\n", nome_completo);

  f = fopen(nome_completo, "r");
  if (!f) {
    fprintf(stderr, "Não pude carregar nível!\n");
    return 0;
  }

  for(i=0; i<NUM_BLOCOS_X*NUM_BLOCOS_Y; i++) {
    if (feof(f)) {//Não deveria ter chegado ao fim do arquivo ainda
      fprintf(stderr, "Arquivo %s está no formato incorreto!!\n", nome_completo);
      return 0; 
    }
    num_lidos = fscanf(f, "%d ", &indice);
    if (num_lidos<1) {//Não tem o número certo de índices de blocos 
      fprintf(stderr, "Arquivo %s está no formato incorreto!!\n", nome_completo);
      return 0; 
    }
    if (indice < 0) {
      blocos[i] = NULL; //Sem bloco
      continue;
    }
    if (indice >= NUM_TIPOS_BLOCOS) {
      fprintf(stderr, "Arquivo %s está incorreto! Índice de bloco>3\n", nome_completo);
      return 0;
    }
    //    printf("Criando bloco %d id=%d pos=(%d %d)\n", i, indice, OFFSET_X + 40*(i%16), OFFSET_Y +20*(i/14));
    blocos[i] = blsprite_new();
    blsprite_init(blocos[i]);
    blocos[i]->tipo   = ID_PRIMEIRO_BLOCO+indice;
    blocos[i]->imagem = imagens[ID_PRIMEIRO_BLOCO+indice];
    blocos[i]->x = OFFSET_X + 40*(i%NUM_BLOCOS_X);
    blocos[i]->y = OFFSET_Y + 20*(i/NUM_BLOCOS_X);    
  }

  fclose(f);
  return 1;
}


int main_nova_bola() { //Cria nova bola e põe no jogo
  if (num_bolas>=MAX_BOLAS) return 0;
  jogador->x = (LARGURA_TELA-jogador->imagem->w)/2;
  bolas[num_bolas] = blsprite_new();
  blsprite_init(bolas[num_bolas]);
  estado_da_bola[num_bolas]=BOLA_PARADA;
  bolas[num_bolas]->imagem = imagens[SPR_BOLA_0];
  bolas[num_bolas]->tipo = SPR_BOLA_0;
  bolas[num_bolas]->y    = ALTURA_TELA-(3*20)-jogador->imagem->h;
  bolas[num_bolas]->x    = jogador->x + 
    (jogador->imagem->w - bolas[num_bolas]->imagem->w)/2;
  num_bolas++;
  return 1;
}

int main_apaga_bola(int id) { //Apaga bola correspondente à id
  if (num_bolas<1) return 0;
  blsprite_apaga(bolas[id], s);
  blsprite_update(bolas[id], s);
  blsprite_finish(bolas[id]);
  blsprite_destroy(bolas[id]);
  bolas[id] = bolas[num_bolas-1];
  num_bolas--;
  return 1;
}
int main_descarrega_nivel() {
  int i;
  //Apaga bolas da memória
  while(num_bolas>0) main_apaga_bola(0);
  for(i=0; i<NUM_BLOCOS_X*NUM_BLOCOS_Y; i++) {
    blsprite_finish(blocos[i]);
    blsprite_destroy(blocos[i]);
    blocos[i]=NULL;
  }
  return 1;
}
