comp = gcc -std=c99 -Wall
lib = -lncurses
path = bin
obj = 			\
		$(path)/main.o	\
		$(path)/game.o	\
		$(path)/global.o	\
		$(path)/draw.o	\
		$(path)/net.o
all : $(path)/BattleShip

$(path)/BattleShip : $(obj)
	$(comp) $(obj) -o $(path)/BattleShip $(lib)
$(path)/%.o : %.c
	$(comp) -c $< -o $@
clean :
	rm -rf $(path)/*
