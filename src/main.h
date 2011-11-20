#ifndef __main_h__
#define __main_h__

#define TEMPO_FRAME 20 //Tempo em milisegundos para um frame ser exibido.
#define CAMINHO_DO_JOGO "../" //Pasta onde se encontram os recursos (níveis, imgs, etc.)

#define OFFSET_X 40           //Offset em pixels a partir da esquerda ou da direita
#define OFFSET_Y 40           //Offset em pixels a partir de cima

#define ARQUIVO_DE_FONTE "fonte.bmp"  //Arquivo .bmp que contém a fonte raster
#define MAX_NUM_NIVEIS 400           //Número máximo de níveis listados
#define MAX_NIVEIS_MOSTRADOS 12      //Número máximo de níveis listados ao mesmo tempo

#define MAX_BOLAS 20  //Número máximo de bolas que o jogador pode usar ao mesmo tempo

#define NUM_BLOCOS_X 14              //Número de blocos na horizontal
#define NUM_BLOCOS_Y 16              //Número de blocos na vertical
#define LARGURA_TELA 640             //Largura da tela
#define ALTURA_TELA  480             //Altura da tela
 
#define NUM_TIPOS_BLOCOS  4              //Número de tipos de bloco
#define ID_PRIMEIRO_BLOCO (SPR_BLOCO_0)  //número corresp. ao id do prim. tipo de bloco

#define VEL_JOGADOR 9                    //Velocidade x em pixels do jogador

#define NUM_SONS 5                       //Número total de sons
enum { //Sons disponíveis
  SOM_COLISAO_NIVEL=0,
  SOM_COLISAO_BLOCO,
  SOM_COLISAO_JOGADOR,
  SOM_GANHOU,
  SOM_PERDEU
};

#define NUM_IMAGENS 17                   //Número total de imagens
enum {  //Tipos de sprite
  SPR_FUNDO=0,
  SPR_BARREIRA,
  SPR_JOGADOR_DIMINUIDO,
  SPR_JOGADOR_NATURAL,
  SPR_JOGADOR_AUMENTADO,
  SPR_BLOCO_0,
  SPR_BLOCO_1,
  SPR_BLOCO_2,
  SPR_BLOCO_3,
  SPR_BOLA_0,
  SPR_BOLA_1,
  SPR_BONUS_0,
  SPR_BONUS_1,
  SPR_BONUS_2,
  SPR_BONUS_3,
  SPR_BONUS_4,
  SPR_BONUS_5,
  SPR_BONUS_6
} ;

enum {    //Cada modo de jogo indica a tela onde o usuário está
  MODO_MENU,
  MODO_NIVEL,
  MODO_JOGO
};

enum { //Estado de cada bola
  BOLA_PARADA,
  BOLA_MOVIMENTO
} ;

int main_carrega_nivel(char *nome); //Função que carrega um nível do arquivo pra memória
int main_descarrega_nivel(); //Função que descarrega um nível da memória
int main_nova_bola(); //Cria nova bola e põe no jogo
int main_apaga_bola(int id); //Apaga bola correspondente à id

#endif //__main_h__
