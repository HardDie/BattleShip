#include "game.h"

/*
 * Name: game_initVariables
 * Description: Сбрасывает все параметры игры в изначальное положение
 * */
void game_initVariables() {
	for ( int i = 0; i < 10; i++ ) {
		for ( int j = 0; j < 10; j++ ) {
			a_field[i][j] = b_field[i][j] = kTile_background;
		}
	}
}

/*
 * Name: game_doStep
 * Description: Выбор клетки для стрельбы и отрисовка курсора выбора хода, возвращается координата выстрела старшие 4 бита координата X младшие Y
 * */
char game_doStep() {
	char isDone = 0, reDraw = 1;
	while ( !isDone ) {
		if ( reDraw ) {
			draw_battleField();
			draw_cursorForShoot();
			draw_help( "Please choose coordinate for shoot, and press Enter" );
			reDraw = 0;
		}
		switch ( getch() ) {	// Перемещение курсора
			case KEY_UP:
				if ( y_coor > 0 ) {
					y_coor--;
					reDraw = 1;
				}
				break;
			case KEY_DOWN:
				if ( y_coor < 9 ) {
					y_coor++;
					reDraw = 1;
				}
				break;
			case KEY_LEFT:
				if ( x_coor > 0 ) {
					x_coor--;
					reDraw = 1;
				}
				break;
			case KEY_RIGHT:
				if ( x_coor < 9 ) {
					x_coor++;
					reDraw = 1;
				}
				break;
			case 10:	// Enter
				isDone = 1;
				if ( b_field[(int)y_coor][(int)x_coor] == kTile_ship ) {
					b_field[(int)y_coor][(int)x_coor] = kTile_bitShip;
				} else {
					b_field[(int)y_coor][(int)x_coor] = kTile_miss;
				}

				reDraw = 1;
				break;
		}
	}
	draw_battleField();
	char result = 0;	// Переменная для возврата значения
	for ( char i = 0; i < 4; i++ ) {
		if ( y_coor & 1 << i ) {	// В 4 младших бита записываем координату y
			result |= 1 << i;
		}
		if ( ( x_coor / 2 ) & 1 << i ) {	// В 4 старших бита записываем координату x
			result |= 1 << ( i + 4 );
		}
	}
	return result;
}

/*
 * Name: game_setUpShips
 * Description: Расстановка кораблей на поле
 * */
 void game_setUpShips() {
	 char isDone = 0, reDraw = 1;
	 char shipNum = 1;
	 char pos = 0;	// Расположение коробля 0 - горизонтально, 1 - вертикально
	 x_coor = 0;
	 y_coor = 0;
	 char width;	// Ширина коробля

	 while ( !isDone ) {
		 switch ( shipNum ) {	// Выбор ширины коробля
			case 1:		// Четырехпалубники
				width = 4;
				break;
			case 2:		// Трехпалубники
			case 3:
				width = 3;
				break;
			case 4:		// Двухпалубники
			case 5:
			case 6:
				width = 2;
				break;
			case 7:		// Однопалубники
			case 8:
			case 9:
			case 10:
				width = 1;
				break;
		 }
		 if ( reDraw ) {	// Отрисовка
			draw_battleField();
			draw_shipSetUp( width, pos );
			draw_help( "Setup your ships, use ARROWS, TAB and ENTER" );
			mvprintw( 17, 0, "x:%d y:%d\nShip num:%d pos:%d\nwidth: %d", x_coor, y_coor, shipNum, pos, width );
			reDraw = 0;
		 }
		 switch ( getch() ) {	// Передвижение курсора с кораблем
			case KEY_DOWN:
				if ( pos == 0 ) {
					if ( y_coor < 9 ) {
						y_coor++;
						reDraw = 1;
					}
				} else {
					if ( y_coor + width < 10 ) {
						y_coor++;
						reDraw = 1;
					}
				}
				break;
			case KEY_UP:
				if ( y_coor > 0 ) {
					y_coor--;
					reDraw = 1;
				}
				break;
			case KEY_RIGHT:
				if ( pos == 0 ) {
					if ( x_coor + width < 10 ) {
						x_coor++;
						reDraw = 1;
					}
				} else {
					if ( x_coor < 9 ) {
						x_coor++;
						reDraw = 1;
					}
				}
				break;
			case KEY_LEFT:
				if ( x_coor > 0 ) {
					x_coor--;
					reDraw = 1;
				}
				break;
			case 9:		// TAB
				if ( pos == 0 ) {
					if ( y_coor + width <= 10 ) {
						pos = 1;
						reDraw = 1;
					}
				} else {
					if ( x_coor + width <= 10 ) {
						pos = 0;
						reDraw = 1;
					}
				}
				break;
			case 10:	// Enter
				if ( !( game_checkShipForSet( width, pos ) ) ) {
					if ( pos == 0 ) {
						for ( char i = 0; i < width; i++ ) {
							a_field[(int)y_coor][ (int)x_coor + i ] = kTile_ship;
						}
					} else {
						for ( char i = 0; i < width; i++ ) {
							a_field[ (int)y_coor + i ][(int)x_coor] = kTile_ship;
						}
					}
					reDraw = 1;
					shipNum++;
					if ( shipNum == 11 ) {
						isDone = 1;
					}
				}
				break;
		 }
	 }
 }

 /*
  * Name: game_checkShipForSet
  * Description: Проверяет можно ли корабль установить в это место на поле, в случае успеха возвращает 0, иначе 1
  * */
char game_checkShipForSet( const char width, const char pos ) {
	  if ( pos == 0 ) {
		  for ( int i = 0; i < width; i++ ) {
			  if ( game_checkDotForSet( x_coor + i, y_coor ) ) {
				  return 1;
			  }
		  }
	  } else {
		  for ( int i = 0; i < width; i++ ) {
			  if ( game_checkDotForSet( x_coor, y_coor + i ) ) {
				  return 1;
			  }
		  }
	  }
	  return 0;
  }

/*
 * Name: game_checkDotForSet
 * Description: Проверяет точку на отсутствие вокруг нее короблей, в случае отсутствия помех возврвщвет 0, иначе 1
 * */
char game_checkDotForSet( const char x, const char y ) {
	if ( x != 0 && x != 9 ) {	// Левый и правый куб
		if ( a_field[(int)y][ (int)x - 1 ] != kTile_background ) {
			return 1;
		}
		if ( a_field[(int)y][ (int)x + 1 ] != kTile_background ) {
			return 1;
		}
	}
	if ( y != 0 && y != 9 ) {	// Верхний и нижний куб
		if ( a_field[ (int)y - 1 ][(int)x] != kTile_background ) {
			return 1;
		}
		if ( a_field[ (int)y + 1 ][(int)x] != kTile_background ) {
			return 1;
		}
	}
	if ( x != 0 && y != 0 ) {	// Левый верхний куб
		if ( a_field[ (int)y - 1 ][ (int)x - 1 ] != kTile_background ) {
			return 1;
		}
	}
	if ( x != 0 && y != 9 ) {	// Левый нижний куб
		if ( a_field[ (int)y + 1 ][ (int)x - 1 ] != kTile_background ) {
			return 1;
		}
	}
	if ( x != 9 && y != 0 ) {	// Правый верхний куб
		if ( a_field[ (int)y - 1 ][ (int)x + 1 ] != kTile_background ) {
			return 1;
		}
	}
	if ( x != 9 && y != 9 ) {	// Правый нижний куб
		if ( a_field[ (int)y + 1 ][ (int)x + 1 ] != kTile_background ) {
			return 1;
		}
	}
	return 0;
}

/*
 * Name: game_mainMenu
 * Description: Производится выбор пункта в главном меню, тип клиента записывается в передающуюся переменную, а игровой режим возвращается
 * */
char game_mainMenu( char* whoPlayer ) {
	draw_menu();
	draw_help( "To select a menu point, press key" );
	while ( 1 ) {
		switch ( getch() ) {
			case 49:	// 1
				*whoPlayer = NET_SERVER;
				return NET_SETUP;
				break;
			case 50:	// 2
				*whoPlayer = NET_CLIENT;
				return NET_SETUP;
				break;
			case 113:	// Q
				return END_GAME;
				break;
		}
	}
}
