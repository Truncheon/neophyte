FILES= core.c core.h\
	sprite.c sprite.h \
	player.c player.h \
	map.c map.h \
	tinytest.c

LIBS= -lSDL2 -lSDL2_image
FLAGS= -Wall -Wextra -pedantic -g
NAME= tinytest

make: $(FILES)
	gcc $(FILES) $(LIBS) $(FLAGS) -o $(NAME)


