#include "global.h"

const 	char 			kSPACE_BETWEEN = ( 8 * 3 );	// Расстояние между окнами полей
const 	char 			kTile_background = ' ';
const 	char 			kTile_ship = '0';
const 	char 			kTile_bitShip = 'X';
const 	char 			kTile_miss = '*';
				char 			a_field[10][10];	// Поле первого игрока
				char 			b_field[10][10];	// Поле второго игрока
				char 			x_coor = 0, y_coor = 0;	// Координаты курсора
		WINDOW 		*	winFstPlayer = NULL;	// Окно первого игрока
		WINDOW 		*	winSecPlayer = NULL;	// Окно второго игрока
		WINDOW		*	winHelp = NULL;	// Окно вывода дополнительной информации
		char			gameState;
