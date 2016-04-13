#include "draw.h"

static int handleLoadScreen = -1;

/*
 * Name: draw_labels
 * Description: Отрисовывает сетку поля
 * */
void	draw_labels() {
	clear();
	printw( " Your table\t\t" );
	for ( char i = 0; i < kSPACE_BETWEEN; i++ ) {
		addch( ' ' );
	}
	printw( "Opponet table\n  -A-B-C-D-E-F-G-H-I-J-" );
	for ( char i = 0; i < kSPACE_BETWEEN; i++ ) {
		addch( ' ' );
	}
	printw( "  -A-B-C-D-E-F-G-H-I-J-\n" );
	for ( char i = 0; i < 10; i++ ) {
		printw( "%2d|", i + 1 );
		printw( "                   |" );
		for ( char i = 0; i < kSPACE_BETWEEN; i++ ) {
			addch( ' ' );
		}
		printw( "%2d|", i + 1 );
		printw( "                   |\n" );
	}
	printw( "  " );
	printw( "---------------------" );
	for ( char i = 0; i < kSPACE_BETWEEN; i++ ) {
		addch( ' ' );
	}
	printw( "  " );
	printw( "---------------------" );
	refresh();
}

/*
 * Name: draw_battleField
 * Description: Выводит игровые поля обоих игроков
 * */
void draw_battleField() {
	wclear( winFstPlayer );
	wclear( winSecPlayer );
	for ( int i = 0; i < 10; i++ ) {
		for ( int j = 0; j < 10; j++ ) {
			if ( j != 9 ) {
				wprintw( winFstPlayer, "%c|", a_field[i][j] );
				wprintw( winSecPlayer, "%c|", b_field[i][j] );
			} else {
				waddch( winFstPlayer, a_field[i][j] );
				waddch( winSecPlayer, b_field[i][j] );
			}
		}
	}
	wrefresh( winFstPlayer );
	wrefresh( winSecPlayer );
}

/*
 * Name: draw_cursorForShoot
 * Description: Выводит курсор для выбора места стрельбы
 * */
void draw_cursorForShoot() {
	wattron( winSecPlayer, COLOR_PAIR( 2 ) );	// Инверсия цвета для курсора
	wmove( winSecPlayer, y_coor, x_coor * 2 );
	waddch( winSecPlayer, b_field[( int ) y_coor][( int ) x_coor] );	// Отрисовка курсора
	wattron( winSecPlayer, COLOR_PAIR( 1 ) );	// Возвращаем исходные цвета
	wrefresh( winSecPlayer );
}

/*
 * Name: draw_shipSetUp
 * Description: Отрисовывает корабль при расстановке их на поле
 * */
void draw_shipSetUp( const char width, const char pos ) {
	wattron( winFstPlayer, COLOR_PAIR( 2 ) );	// Инверсия цвета для курсора
	if ( pos == 0 ) {	// Для горизонтального положения коробля
		for ( char i = 0; i < width; i++ ) {
			wmove( winFstPlayer, y_coor, ( x_coor + i ) * 2 );
			if ( a_field[( int ) y_coor][( int ) x_coor + i] == kTile_ship ) {	// Если точка совпала уже с существующим кораблем выводит красным цветом
				wattron( winFstPlayer, COLOR_PAIR( 3 ) );
			}
			waddch( winFstPlayer, kTile_ship );	// Отрисовка коробля
			wattron( winFstPlayer, COLOR_PAIR( 2 ) );	// Возвращаем цвет
		}
	} else if ( pos == 1 ) {	// Для вертикального
		for ( char i = 0; i < width; i++ ) {
			wmove( winFstPlayer, y_coor + i, x_coor * 2 );
			if ( a_field[( int ) y_coor + i][( int ) x_coor] == kTile_ship ) {	// Если точка совпала уже с существующим кораблем выводит красным цветом
				wattron( winFstPlayer, COLOR_PAIR( 3 ) );
			}
			waddch( winFstPlayer, kTile_ship );	// Отрисовка коробля
			wattron( winFstPlayer, COLOR_PAIR( 2 ) );	// Возвращаем цвет
		}
	} else {
		draw_ERROR( "draw_shipSetUp", "Wrong argument pos" );
	}
	wattron( winFstPlayer, COLOR_PAIR( 1 ) );	// Возвращаем исходные цвета
	wrefresh( winFstPlayer );
}

/*
 * Name: draw_menu
 * Description: Выводит меню игры
 * */
void draw_menu() {
	clear();
	int row, col;
	getmaxyx( stdscr, row, col );
	mvprintw( 1, col / 2 - 5, "Main menu!" );
	mvprintw( 2, 4, "1. Server" );
	mvprintw( 3, 4, "2. Client" );
	mvprintw( 4, 4, "Q. Exit" );
	refresh();
}

/*
 * Name: draw_netServerPortEnter
 * Description: Выводит меню для сервера. Ввод порта
 * */
void draw_netServerPortEnter() {
	clear();
	printw( "SERVER Setup\n" );
	printw( ">" );
	refresh();
}

/*
 * Name: draw_netClientIPEnter
 * Decription: Выводит меня для клиента. Ввода IP адреса
 */
void  draw_netClientIPEnter() {
	clear();
	printw( "CLIENT Setup\n" );
	printw( ">" );
	refresh();
}

/*
 * Name: draw_netClientPortEnter
 * Decription: Выводит меню для клиента. Ввод порта
 */
void  draw_netClientPortEnter( const char* ip ) {
	clear();
	printw( "CLIENT Setup\n" );
	printw( "IP: %s\n", ip );
	printw( ">" );
	refresh();
}

/*
 * Name: draw_ERROR
 * Decription: Выводит ошибку программы, ожидает нажатия и аварийно завершает программу
 */
void draw_ERROR( const char* funcName, const char* typeError ) {
	if ( handleLoadScreen != -1 ) {
		draw_closeLoadScreen();
	}
	clear();
	wattron( stdscr, COLOR_PAIR( 3 ) ); // Делаем вывод ошибки красным цветом
	int row, col;
	getmaxyx( stdscr, row, col );
	mvprintw( 0, col / 2 - 3, "ERROR!" );
	mvprintw( row / 2, col / 2 - strlen( funcName ) / 2 - 3, "Func: %s", funcName );
	mvprintw( row / 2 + 1, col / 2 - strlen( typeError ) / 2, "%s", typeError );
	wattron( stdscr, COLOR_PAIR( 1 ) ); // Возвращаем цвета
	refresh();
	getch();  // Делаем задержку для прочтения ошибки
	endwin();
	exit( 1 );
}

/*
 * Name: draw_help
 * Description: Выводит подсказу для действий
 * */
void  draw_help( const char* helpText ) {
	wclear( winHelp );
	wattron( winHelp, COLOR_PAIR( 4 ) );
	wprintw( winHelp, "Help: %s", helpText );
	wattron( winHelp, COLOR_PAIR( 1 ) );
	wrefresh( winHelp );
}

/*
 * Name: draw_loadFullScreen
 * Description: Отрисовывает текст для загрузки очищая экран
 * */
void draw_loadFullScreen( const char* loadText ) {
	handleLoadScreen = fork();
	if ( handleLoadScreen == 0 ) {	// Создаем дочерний процесс отрисовки экрана загрузки
		int row, col, dot;
		getmaxyx( stdscr, row, col );
		dot = 1;
		while ( 1 ) {
			clear();
			mvprintw( row / 2, col / 2 - strlen( loadText ) / 2, "%s", loadText );
			for ( int i = 0; i < dot; i++ ) {
				addch( '.' );
			}
			if ( ++dot > 5 ) {
				dot = 1;
			}
			refresh();
			sleep( 1 );
		}
	}
}

/*
 * Name: draw_loadText
 * Description: Отрисовывает текст для загрузки не очищая экран
 * */
void draw_loadText( const char* loadText ) {
	handleLoadScreen = fork();
	if ( handleLoadScreen == 0 ) {	// Создаем дочерний процесс отрисовки экрана загрузки
		int row, col, dot;
		getmaxyx( stdscr, row, col );
		dot = 1;
		while ( 1 ) {
			mvprintw( row / 2 + 1, col / 2 - strlen( loadText ) / 2, "%s", loadText );
			for ( int i = 0; i < 5; i++ ) {
				if ( i < dot ) {
					addch( '.' );
				} else {
					addch( ' ' );
				}

			}
			if ( ++dot > 5 ) {
				dot = 1;
			}
			refresh();
			sleep( 1 );
		}
	}
}

/*
 * Name: draw_closeLoadScreen
 * Description: Закрывает процесс отрисовки окна
 * */
void draw_closeLoadScreen() {
	kill( handleLoadScreen, SIGKILL );
	handleLoadScreen = -1;
}
