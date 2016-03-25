#ifndef _DRAW_H_
#define _DRAW_H_

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "global.h"

void	draw_labels();
void 	draw_battleField();
void	draw_cursorForShoot();
void	draw_shipSetUp( const char width, const char pos );
void	draw_menu();
void	draw_netServerPortEnter();
void	draw_netClientIPEnter();
void	draw_netClientPortEnter( const char* ip );
void	draw_ERROR( const char* funcName, const char* typeError );
void	draw_help( const char* helpText );
int		draw_load( const char* loadText );

#endif
