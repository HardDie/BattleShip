#ifndef _GAME_H_
#define _GAME_H_

#define _POSIX_SOURCE
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "global.h"
#include "draw.h"
#include "net.h"

void	game_initVariables();
void	game_doStep();
void	game_setUpShips();
char	game_checkShipForSet( const char width, const char pos );
char	game_checkDotForSet( const char x, const char y );
char	game_mainMenu( char* whoPlayer );
char	game_initGame( const char typeConnection );
void	game_waitStep();

#endif
