#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <ncurses.h>

extern const 	char 				kSPACE_BETWEEN;	// Расстояние между полями
extern const 	char 				kTile_background;
extern const 	char 				kTile_ship;
extern const 	char 				kTile_bitShip;
extern const 	char 				kTile_miss;
extern			char 				a_field[10][10];	// Поле первого игрока
extern			char 				b_field[10][10];	// Поле второго игрока
extern			char 				x_coor, y_coor;	// Координаты курсора
extern 			WINDOW 		*		winFstPlayer;
extern 			WINDOW 		*		winSecPlayer;
extern			WINDOW		*		winHelp;
extern 			char				gameState;	// Состояние игры

				enum	{	// Состояние игры
							GS_MENU = 0,
							GS_NET_SETUP,
							GS_SETUP_SHIP,
							GS_WAIT_ENEMY,
							GS_WAIT_STEP,
							GS_SHOOT,
							GS_END_GAME
						};

				enum	{
							NET_SERVER = 1,
							NET_CLIENT
						};

				enum	{
							SHOOT_MISS,
							SHOOT_BIT,
							SHOOT_KILL
						};

#endif
