FILES= core.c core.h \
	sprite.c sprite.h \
	player.c player.h \
	map.c map.h \
	utils.c utils.h \
	character.c character.h \
	text.c text.h \
	texture.c texture.h \
	input.c input.h \
	tinytest.c

LIBS= -lSDL2 -lSDL2_image -lSDL2_ttf
FLAGS= -Wall -Wextra -pedantic -g -pg
NAME= tinytest

make: $(FILES)
	gcc $(FILES) $(LIBS) $(FLAGS) -o $(NAME)


