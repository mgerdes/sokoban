all: sokoban

sokoban: level_reader.c warehouse.c main.c maths.c maths.h sokoban.h
	gcc level_reader.c warehouse.c maths.c main.c -lGL -lGLEW -lglfw -lm -o sokoban
