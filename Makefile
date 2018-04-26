ifndef V
	QUIET_CC       = @echo '  ' CC '       ' $@;
	QUIET_BUILT_IN = @echo '  ' BUILTIN '  ' $@;
	QUIET_CLEAN    = @echo '  ' CLEAN '    ' $<;
endif

CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Werror
RM = rm -f

lib = -lncurses
path = bin
obj = \
		$(path)/main.o   \
		$(path)/game.o   \
		$(path)/global.o \
		$(path)/draw.o   \
		$(path)/net.o
all : check_path $(path)/BattleShip

$(path)/BattleShip : $(obj)
	$(QUIET_BUILT_IN)$(CC) $(CFLAGS) $(obj) -o $(path)/BattleShip $(lib)
$(path)/%.o : %.c
	$(QUIET_CC)$(CC) $(CFLAGS) -c $< -o $@

clean : $(path)
	$(QUIET_CLEAN)$(RM) -r $<

check_path :
	@ if [ ! -d $(path) ]; then mkdir $(path); fi
