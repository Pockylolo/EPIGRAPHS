CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1 -g -fsanitize=address $(shell pkg-config --cflags sdl2 SDL2_image SDL2_gfx) $(shell pkg-config --cflags gtk+-3.0) -D__NO_INLINE__
LDFLAGS = 
LDLIBS = -lm $(shell pkg-config --libs sdl2 SDL2_image SDL2_gfx)  $(shell pkg-config --libs gtk+-3.0)

SRC = src/list.c src/graph.c src/pixel.c src/graph_display.c src/main_process.c src/interface.c src/transform.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}


all: main

main: ${OBJ}
	${CC} ${CFLAGS} $^ -o $@ ${LDLIBS}
	

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main


-include ${DEP}
