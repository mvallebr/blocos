# A variável CC indica qual é o nome do comando do compilador
CC=gcc

# Cada arquivo .c é compilado com as opções em CFLAGS passadas para o
# compilador. -O2 significa que está sendo utilizado o tipo de otimização 2,
# que é velocidade. O programa talvez fique maior mas será o mais rápido 
# possível. -Wall habilita todos os Warnings, ou avisos (W=warning=aviso e 
# all=todos). -g diz ao compilador para guardar informações sobre o código
# de forma que seja possível depurar o programa posteriormente, com uso de 
# um debugger. 
# $$(sdl-config --cflags) diz ao Makefile para executar o comando "sdl-config --cflags",
# que pode ser executado em um shell, e passar a saida desse programa para o compilador.
# -I. diz ao compilador para procurar por arquivos .h no diretório corrente além de procurar
#  nos diretórios padrão.
CFLAGS=-O2 -Wall -g $$(sdl-config --cflags) -I.

# Depois que todo o código foi compilado, temos vários arquivos .o a serem linkados. Para
# linká-los, passaremos flags de ligação ao compilador, que são representados pela variável
# LDFLAGS. $$(sdl-config --libs) executa esse comando e coloca a saida em LDFLAGS, como acima. 
# O programa sdl-config é instalado junto com a libsdl. -lSDL_image e -lSDL_mixer diz ao compilador
# para ligar o executável com as bibliotecas dinâmicas libSDL_image.so e libSDL_mixer.so
LDFLAGS=$$(sdl-config --libs)  -lSDL_image -lSDL_mixer

# Guardaremos o nome do programa em uma variável. Dessa forma, ficará fácil alterar o nome 
# do programa, posteriormente.
NOMEPROG=blocos

#
OBJECTS=main.o blscreen.o blsprite.o SFont.o 

# all é o primeiro alvo. Na frente do alvo, vem os nomes dos arquivos objeto (*.o) que precisam
# existir antes do make executar os comandos de shell listados abaixo. Todos os comando listados
# abaixo do alvo precisam necessariamente estar em linhas que começam por tabulação (TAB).
# O make vai compilar todos os arquivos objeto da variável OBJECTS usando CFLAGS e então, caso 
# consiga compilar todos, vai executar o comando da linha abaixo do alvo all, que liga todos
# os objetos em um executável.
all: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(NOMEPROG) 

#O alvo clean remove todos os arquivos objetos, o executável do programa e arquivos de backup
clean:
	rm -f *~ *.o $(NOMEPROG)
