LIBS = -lGL -lGLEW -lglfw -lm
INCLUDES = -Iobjects -Imaths -Iutils -Iobjects -Iimage_reader
SRCS = level_reader.c warehouse.c main.c maths/maths.c utils/gl_utils.c objects/cube.c
TARGET = sokoban

sokoban: $(SRCS)
	gcc $(SRCS) $(LIBS) $(INCLUDES) -o $(TARGET)

clean:
	rm -f $(TARGET)
