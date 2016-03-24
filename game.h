#ifndef _GAME_H_
#define _GAME_H_

#include <ncurses.h>
#include "global.h"
#include "draw.h"

void	game_initVariables();
char	game_doStep();
void	game_setUpShips();
char	game_checkShipForSet( const char width, const char pos );
char	game_checkDotForSet( const char x, const char y );
char  game_mainMenu( char* whoPlayer );

#endif
