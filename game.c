#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "global.h"
#include "draw.h"
#include "net.h"

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
 * Description: Выбор клетки для стрельбы и отрисовка курсора выбора хода, передается координата выстрела старшие 4 бита координата X младшие Y, возвращает тип попадания
 * */
char game_doStep() {
	char isDone = 0, reDraw = 1;
	char result;	// Переменная для возврата значения
	while ( !isDone ) {
		if ( reDraw ) {		// Отрисовка экрана
			draw_battleField();
			draw_cursorForShoot();
			draw_help( "Please choose coordinate for shoot use ARROWS or WASD, and press Enter" );
			reDraw = 0;
		}
		switch ( getch() ) {	// Перемещение курсора
		case 119:	// W
		case KEY_UP:
			if ( y_coor > 0 ) {
				y_coor--;
				reDraw = 1;
			}
			break;
		case 115:	// S
		case KEY_DOWN:
			if ( y_coor < 9 ) {
				y_coor++;
				reDraw = 1;
			}
			break;
		case 97:	// A
		case KEY_LEFT:
			if ( x_coor > 0 ) {
				x_coor--;
				reDraw = 1;
			}
			break;
		case 100:	// D
		case KEY_RIGHT:
			if ( x_coor < 9 ) {
				x_coor++;
				reDraw = 1;
			}
			break;
		case 10:	// Enter
			if ( b_field[ ( int )y_coor ][ ( int )x_coor ] != kTile_ship && b_field[ ( int )y_coor ][ ( int )x_coor ] != kTile_background ) {	// Если текущаю точка выстрела не равна кораблю или пустой точке, то продолжаем стрелять
				continue;
			}
			isDone = 1;
			result = 0;
			for ( char i = 0; i < 4; i++ ) {
				if ( y_coor & 1 << i ) {	// В 4 младших бита записываем координату y
					result |= 1 << i;
				}
				if ( x_coor & 1 << i ) {	// В 4 старших бита записываем координату x
					result |= 1 << ( i + 4 );
				}
			}
			net_sendMessage( &result, 1 );	// Отправляем противнику координаы выстрела
			net_recvMessage( &result, 1 );	// Принимаем результат, попадание, промах или убийство корабля
			if ( result == SHOOT_BIT ) {
				b_field[ ( int )y_coor ][ ( int )x_coor ] = kTile_bitShip;
			} else if ( result == SHOOT_MISS ) {
				b_field[ ( int )y_coor ][ ( int )x_coor ] = kTile_miss;
			} else {
				draw_ERROR( "game_doStep", "Wrong argument b_field[y_coor][x_coor]" );
			}
			reDraw = 1;
			break;
		}
	}
	draw_battleField();	// Отрисовываем игровые поля еще раз, чтобы при ожидании видеть, попал или нет выстрел
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
	x_coor = 0;		// Зануляем курсор
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
			draw_help( "Setup your ships, use ARROWS or WASD, TAB and ENTER" );
			reDraw = 0;
		}

		switch ( getch() ) {	// Передвижение курсора с кораблем
		case 115:	// S
		case KEY_DOWN:
			if ( pos == 0 ) {
				if ( y_coor < 9 ) {
					y_coor++;
					reDraw = 1;
				}
			} else if ( pos == 1 ) {
				if ( y_coor + width < 10 ) {
					y_coor++;
					reDraw = 1;
				}
			} else {
				draw_ERROR( "game_setUpShips; case KEY_DOWN", "Wrong argument pos" );
			}
			break;
		case 119:	// W
		case KEY_UP:
			if ( y_coor > 0 ) {
				y_coor--;
				reDraw = 1;
			}
			break;
		case 100:	// D
		case KEY_RIGHT:
			if ( pos == 0 ) {
				if ( x_coor + width < 10 ) {
					x_coor++;
					reDraw = 1;
				}
			} else if ( pos == 1 ) {
				if ( x_coor < 9 ) {
					x_coor++;
					reDraw = 1;
				}
			} else {
				draw_ERROR( "game_setUpShips; case KEY_RIGHT", "Wrong argument pos" );
			}
			break;
		case 97:	// A
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
			} else if ( pos == 1 ) {
				if ( x_coor + width <= 10 ) {
					pos = 0;
					reDraw = 1;
				}
			} else {
				draw_ERROR( "game_setUpShips; case 9", "Wrong argument pos" );
			}
			break;
		case 10:	// Enter
			if ( !( game_checkShipForSet( width, pos ) ) ) {	// Если функция проверки на возможность установки коробля возвращает 0, то записываем корабля в массив поля
				if ( pos == 0 ) {
					for ( char i = 0; i < width; i++ ) {
						a_field[( int ) y_coor][( int ) x_coor + i] = kTile_ship;
					}
				} else if ( pos == 1 ) {
					for ( char i = 0; i < width; i++ ) {
						a_field[( int ) y_coor + i][( int ) x_coor] = kTile_ship;
					}
				} else {
					draw_ERROR( "game_setUpShips; case 10", "Wrong argument pos" );
				}
				reDraw = 1;
				shipNum++;	// Переходим к установке следующего коробля
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
		for ( int i = 0; i < width; i++ ) {		// В цикле проверяет каждую точку на возможность ее установки
			if ( game_checkDotForSet( x_coor + i, y_coor ) ) {
				return 1;
			}
		}
	} else if ( pos == 1 ) {
		for ( int i = 0; i < width; i++ ) {
			if ( game_checkDotForSet( x_coor, y_coor + i ) ) {
				return 1;
			}
		}
	} else {
		draw_ERROR( "game_checkShipForSet", "Wrong argument pos" );
	}
	return 0;
}

/*
 * Name: game_checkDotForSet
 * Description: Проверяет точку на отсутствие вокруг нее короблей, в случае отсутствия помех возврвщвет 0, иначе 1
 * */
char game_checkDotForSet( const char x, const char y ) {
	if ( x != 0 && x != 9 ) {	// Левый и правый куб
		if ( a_field[( int ) y][( int ) x - 1] != kTile_background ) {		// Слева
			return 1;
		}
		if ( a_field[( int ) y][( int ) x + 1] != kTile_background ) {		// Справа
			return 1;
		}
	}
	if ( y != 0 && y != 9 ) {	// Верхний и нижний куб
		if ( a_field[( int ) y - 1][( int ) x] != kTile_background ) {		// Снизу
			return 1;
		}
		if ( a_field[( int ) y + 1][( int ) x] != kTile_background ) {		// Сверху
			return 1;
		}
	}
	if ( x != 0 && y != 0 ) {	// Левый верхний куб
		if ( a_field[( int ) y - 1][( int ) x - 1] != kTile_background ) {
			return 1;
		}
	}
	if ( x != 0 && y != 9 ) {	// Левый нижний куб
		if ( a_field[( int ) y + 1][( int ) x - 1] != kTile_background ) {
			return 1;
		}
	}
	if ( x != 9 && y != 0 ) {	// Правый верхний куб
		if ( a_field[( int ) y - 1][( int ) x + 1] != kTile_background ) {
			return 1;
		}
	}
	if ( x != 9 && y != 9 ) {	// Правый нижний куб
		if ( a_field[( int ) y + 1][( int ) x + 1] != kTile_background ) {
			return 1;
		}
	}
	return 0;
}

/*
 * Name: game_mainMenu
 * Description: Производится выбор пункта в главном меню, тип игрока записывается в передающуюся переменную, а следующий игровой режим возвращается
 * */
char game_mainMenu( char* whoPlayer ) {
	draw_menu();
	draw_help( "To select a menu point, press key" );
	while ( 1 ) {
		switch ( getch() ) {
		case 49:	// 1
			*whoPlayer = NET_SERVER;
			return GS_NET_SETUP;
			break;
		case 50:	// 2
			*whoPlayer = NET_CLIENT;
			return GS_NET_SETUP;
			break;
		case 113:	// Q
			*whoPlayer = NET_CLIENT;
			return GS_END_GAME;
			break;
		}
	}
}

/*
 * Name: game_initGame
 * Description: Сервер инициализирует кто будет первый ходить и ждет пока противника, а клиент просто ждет противника, результатом функция возвращает кто ходит первый
 * */
char game_initGame( const char typeConnection ) {
	draw_loadFullScreen( "Wait ready enemy" );

	char first;

	if ( typeConnection == NET_SERVER ) {
		srand( time( NULL ) );
		if ( rand() % 2 ) {		// Определяем кто первый будет ходить
			first = FIRST_SERVER;
		} else {
			first = FIRST_CLIENT;
		}
		net_sendMessage( &first, 1 );	// Сообщаем клиенту, о том кто первый ходит
		net_recvMessage( &first, 1 );	// Ждем обратного подтверждения
		draw_closeLoadScreen();		// Завершаем дочерний процесс отрисовки экрана загрузки
		if ( first == FIRST_SERVER ) {	// Сообщаем о порядке хода
			draw_loadFullScreen( "Your step first" );
			first = 1;
		} else if ( first == FIRST_CLIENT ) {
			draw_loadFullScreen( "Your step second" );
			first = 2;
		} else {
			draw_ERROR( "game_initGame", "Wrong argument first" );
		}
	} else if ( typeConnection == NET_CLIENT ) {
		net_recvMessage( &first, 1 );	// Принимаем от сервера порядок хода
		net_sendMessage( &first, 1 );	// Делаем обратное подтверждение
		draw_closeLoadScreen();		// Завершаем дочерний процесс отрисовки экрана загрузки
		if ( first == FIRST_CLIENT ) {	// Сообщаем о порядке хода
			draw_loadFullScreen( "Your step first" );
			first = 1;
		} else if ( first == FIRST_SERVER ) {
			draw_loadFullScreen( "Your step second" );
			first = 2;
		} else {
			draw_ERROR( "game_initGame", "Wrong recieved argument first" );
		}
	} else {
		draw_ERROR( "game_initGame", "Wrong argument typeConnection" );
	}
	sleep( 2 );	// В течении нескольких секунд держим заставку о порядке хода
	draw_closeLoadScreen();		// Завершаем дочерний процесс отрисовки экрана загрузки

	return first;	// Возвращаем порядок хода
}

/*
 * Name: game_waitStep
 * Description: Ожидает хода противника и сообщает о промахе, попадании либо убийстве коробля, сообщает тип попадания
 * */
char game_waitStep() {
	draw_battleField();
	draw_loadText( "Wait while enemy shoot" );

	char shootCoord;

	net_recvMessage( &shootCoord, 1 );	// Прием координаты выстрела противника
	char answer;
	answer = game_checkShoot( shootCoord );	// Проверяем выстрел
	net_sendMessage( &answer, 1 );	// Сообщаем противнику о результате выстрела

	draw_closeLoadScreen();		// Завершаем дочерний процесс отрисовки экрана загрузки
	return answer;
}

/*
 * Name: game_checkShoot
 * Description: Проверяет выстрел оппонента, и возвращает промах, попал или убил
 * */
char game_checkShoot( const char shootCoord ) {
	int x = 0, y = 0;
	for ( int i = 0, mn = 1; i < 4; i++, mn *= 2 ) {	// Вывод координат выстрела из одного байта
		if ( shootCoord & 1 << i ) {
			y += mn;
		}
		if ( shootCoord & 1 << ( i + 4 ) ) {
			x += mn;
		}
	}

	if ( a_field[y][x] == kTile_background ) {		// Определяем попал или нет противник и отвечаем ему
		a_field[y][x] = kTile_miss;
		return SHOOT_MISS;
	} else if ( a_field[y][x] == kTile_ship ) {
		a_field[y][x] = kTile_bitShip;
		return SHOOT_BIT;
	} else {
		draw_ERROR( "game_checkShoot", "Wrong recieved argument a_field[y][x]" );
	}
	return SHOOT_MISS;
}

/*
 * Name: game_checkWin
 * Description: Проверяет условие выигрыша, возвращает статус победы
 * */
char game_checkWin( const char typeConnection ) {
	char result = GAME_LOSE;

	for ( int i = 0; i < 9; i++ ) {		// Проверяем поле на наличие хоть одного неубитого коробля
		for ( int j = 0; j < 9; j++ ) {
			if ( a_field[i][j] == kTile_ship ) {
				result = GAME_NOTHING;
				break;
			}
		}
		if ( result == GAME_NOTHING ) {
			break;
		}
	}

	if ( typeConnection == NET_SERVER ) {

		net_sendMessage( &result, 1 );
		if ( result == GAME_LOSE ) {
			return GAME_LOSE;
		} else if ( result == GAME_NOTHING ) {
			net_recvMessage( &result, 1 );
			if ( result == GAME_LOSE ) {
				return GAME_WIN;
			} else if ( result == GAME_NOTHING ) {
				return GAME_NOTHING;
			} else {
				draw_ERROR( "game_checkWin", "SERVER wrong recieved argument result" );
			}
		} else {
			draw_ERROR( "game_checkWin", "SERVER wrong argument result" );
		}


	} else if ( typeConnection == NET_CLIENT ) {

		char tmp = result;
		net_recvMessage( &result, 1 );
		if ( result == GAME_LOSE ) {
			return GAME_WIN;
		} else if ( result == GAME_NOTHING ) {
			result = tmp;
			net_sendMessage( &result, 1 );
			if ( result == GAME_LOSE ) {
				return GAME_LOSE;
			} else if ( result == GAME_NOTHING ) {
				return GAME_NOTHING;
			} else {
				draw_ERROR( "game_checkWin", "CLIENT wrong argument result" );
			}
		} else {
			draw_ERROR( "game_checkWin", "CLIENT wrong recieved argument result" );
		}

	} else {
		draw_ERROR( "game_checkWin", "Wrong argument typeConnection" );
	}
	return result;
}
