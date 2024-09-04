CFLAGS = -Wall -Wextra -Wno-missing-braces -std=c11
CC = gcc

all: tile

tile:
	$(CC) $(CFLAGS) ./src/*.c -I ./include -I ./extern/stb/include -o ./build/tile.exe
