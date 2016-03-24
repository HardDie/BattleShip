#include "draw.h"

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
	waddch( winSecPlayer, b_field[(int)y_coor][(int)x_coor] );	// Отрисовка курсора
	wattron( winSecPlayer, COLOR_PAIR( 1 ) );	// Возвращаем исходные цвета
	wrefresh( winSecPlayer );
}

/*
 * Name: draw_shipSetUp
 * Description: Отрисовывает корабль при расстановке их на поле
 * */
void draw_shipSetUp( const char width, const char pos ) {
	wattron( winFstPlayer, COLOR_PAIR( 2 ) );	// Инверсия цвета для курсора
	if ( pos == 0 ) {
		for ( char i = 0; i < width; i++ ) {
			wmove( winFstPlayer, y_coor, ( x_coor + i ) * 2 );
			if ( a_field[(int)y_coor][ (int)x_coor + i ] == kTile_ship ) {	// Если точка совпала уже с существующим кораблем выводит красным цветом
				wattron( winFstPlayer, COLOR_PAIR( 3 ) );
			}
			waddch( winFstPlayer, kTile_ship );	// Отрисовка коробля
			wattron( winFstPlayer, COLOR_PAIR( 2 ) );	// Возвращаем цвет
		}
	} else {
		for ( char i = 0; i < width; i++ ) {
			wmove( winFstPlayer, y_coor + i, x_coor * 2 );
			if ( a_field[ (int)y_coor + i ][(int)x_coor] == kTile_ship ) {	// Если точка совпала уже с существующим кораблем выводит красным цветом
				wattron( winFstPlayer, COLOR_PAIR( 3 ) );
			}
			waddch( winFstPlayer, kTile_ship );	// Отрисовка коробля
			wattron( winFstPlayer, COLOR_PAIR( 2 ) );	// Возвращаем цвет
		}
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
	mvprintw( row * 0.05, col / 2 - 5, "Main menu!" );
	mvprintw( row * 0.1, col * 0.05, "1. Server" );
	mvprintw( row * 0.15, col * 0.05, "2. Client" );
  mvprintw( row * 0.2, col * 0.05, "Q. Exit" );
	refresh();
}

/*
 * Name: draw_netServerPortEnter
 * Description: Выводит меню для сервера ввода порта
 * */
void draw_netServerPortEnter() {
  clear();
  printw( "SERVER Setup\n" );
  printw( ">" );
  refresh();
}

/*
 * Name: draw_netClientIPEnter
 * Decription: Выводит меня для ввода IP адреса для клиента
 */
void  draw_netClientIPEnter() {
  clear();
  printw( "CLIENT Setup\n" );
  printw( ">" );
  //printw( "Enter server IP addres(255.255.255.255): " );
  refresh();
}

/*
 * Name: draw_netClientPortEnter
 * Decription: Выводит меню для клиента ввода порта
 */
void  draw_netClientPortEnter( const char* ip ) {
  clear();
  printw( "CLIENT Setup\n" );
  printw( "IP: %s\n", ip );
  //printw( "Input port(1000-10000): " );
  printw( ">" );
  refresh();
}

/*
 * Name: draw_ERROR
 * Decription: Выводит ошибку программы
 */
void draw_ERROR( const char* funcName, const char* typeError ) {
  clear();
  wattron( stdscr, COLOR_PAIR( 3 ) ); // Делаем вывод ошибки красным цветом
  int row, col;
	getmaxyx( stdscr, row, col );
  mvprintw( 0, col / 2 - 3, "ERROR!" );
  mvprintw( row / 2, col / 2 - strlen( funcName ) / 2  - 3, "Func: %s", funcName );
  mvprintw( row / 2 + 1, col / 2 - strlen( typeError ) / 2, "%s", typeError );
  wattron( stdscr, COLOR_PAIR( 1 ) ); // Возвращаем цвета
  refresh();
  getch();  // Делаем задержку для прочтения ошибки
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
 * Name: draw_load
 * Description: Отрисовывает текст для загрузки
 * */
 void draw_load( const char* loadText ) {
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
