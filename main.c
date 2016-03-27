#include <stdio.h>
#include <ncurses.h>
#include "game.h"
#include "global.h"
#include "draw.h"
#include "net.h"

int 	setUpScreen();
void	freeMemory();
void	changeGameState( const char newState );

int main() {

	//============================
	//	Variables
	//============================
	char gameIsDone = 0;
	char whoPlayer;	// Server or Client
	char whoFirst;	// Порядок хода

	//============================
	//	Setup
	//============================
	gameState = MENU;
	game_initVariables();
	setUpScreen();

	//============================
	//	Game
	//============================
	while ( !gameIsDone ) {
		if ( gameState == MENU ) {

			switch ( game_mainMenu( &whoPlayer ) ) {
			case NET_SETUP:
				changeGameState( NET_SETUP );
				break;
			case END_GAME:
				changeGameState( END_GAME );
				break;
			}

		} else if ( gameState == NET_SETUP ) {

			//net_setup( whoPlayer );
			net_createSocket( whoPlayer );
			net_connectOpponent( whoPlayer );
			changeGameState( SETUP_SHIP );

		} else if ( gameState == SETUP_SHIP ) {

			draw_labels();
			game_setUpShips();
			changeGameState( WAIT_ENEMY );

		} else if ( gameState == WAIT_ENEMY ) {

			whoFirst = game_initGame( whoPlayer );
			if ( ( whoPlayer == NET_SERVER && whoFirst == 0 ) || ( whoPlayer == NET_CLIENT && whoFirst == 1 ) ) {	// Если 0 первым ходит сервер, при 1 первым ходит клиент
				changeGameState( SHOOT );
			} else {
				changeGameState( WAIT_STEP );
			}

		} else if ( gameState == SHOOT ) {

			game_doStep();
			changeGameState( WAIT_STEP );

		} else if ( gameState == WAIT_STEP ) {

			game_waitStep();
			changeGameState( SHOOT );

		} else if ( gameState == END_GAME ) {

			gameIsDone = 1;

		} else {

			draw_ERROR( "main", "Wrong argument gameState" );

		}
	}

	freeMemory();
	return 0;
}

/*
 * Name: setUpScreen
 * Description: Инициализирует настройки экрана
 * */
int setUpScreen() {
	initscr();	// Инициализируем экран

	int row, col;
	getmaxyx( stdscr, row, col );

	winFstPlayer = subwin( stdscr, 10, 19, 2, 3 );	// Создаем экран первого игрока
	winSecPlayer = subwin( stdscr, 10, 19, 2, 26 + kSPACE_BETWEEN );	// Создаем экран второго игрока
	winHelp = subwin( stdscr, 1, col, row - 1, 0 );	// Создаем экран для вывода дополнительной информации

	start_color();	// Включаем цвет
	init_pair( 1, COLOR_WHITE, COLOR_BLUE );	// Создаем пару СИМВОЛ и ФОН для обычных символов
	init_pair( 2, COLOR_BLUE, COLOR_WHITE );	// Пара для текущего положения курсора
	init_pair( 3, COLOR_RED, COLOR_WHITE );		// Пара для совпадения детали текущего и уже существующего корабля
	init_pair( 4, COLOR_BLUE, COLOR_BLACK );	// Пара для текста подсказки

	bkgdset( COLOR_PAIR( 1 ) );	// Устанавливаем пару как стандартную
	wbkgdset( winFstPlayer, COLOR_PAIR( 1 ) );	// Ставим пару цветов для окна первого ирока
	wbkgdset( winSecPlayer, COLOR_PAIR( 1 ) );	// Ставим пару цветов для окна второго игрока
	wbkgdset( winHelp, COLOR_PAIR( 1 ) );	// Ставим пару цветов для окна помощи


	noecho();	// Отключаем отображение символов при нажатии в консоли
	curs_set( 0 );	// Отключаем отображение курсора
	keypad( stdscr, true );	// Включаем управление вводом с клавиатуры
	return 0;
}

/*
 * Name: freeMemory
 * Description: Очистка памяти
 * */
void freeMemory() {
	delwin( winFstPlayer );
	delwin( winSecPlayer );
	endwin();
}

/*
 * Name: changeGameState
 * Description: Меняет состояние игры
 * */
void changeGameState( const char newState ) {
	if ( gameState == MENU ) {
		draw_help( "Leave MENU state!" );
	} else if ( gameState == NET_SETUP ) {
		draw_help( "Leave NET_SETUP state!" );
	} else if ( gameState == SETUP_SHIP ) {
		draw_help( "Leave SETUP_SHIP state!" );
	} else if ( gameState == SHOOT ) {
		draw_help( "Leave SHOOT state!" );
	} else if ( gameState == END_GAME ) {
		draw_help( "Leave END_GAME state!" );
	} else if ( gameState == WAIT_ENEMY ) {
		draw_help( "Leave WAIT_ENEMY state!" );
	} else if ( gameState == WAIT_STEP ) {
		draw_help( "Leave WAIT_STEP state!" );
	} else {
		draw_ERROR( "changeGameState", "Wrong argument gameState" );
	}
	/*else if ( gameState ==  ) {

	}*/
	//getch();

	gameState = newState;

	if ( gameState == MENU ) {
		draw_help( "Set MENU state!" );
	} else if ( gameState == NET_SETUP ) {
		draw_help( "Set NET_SETUP state!" );
	} else if ( gameState == SETUP_SHIP ) {
		draw_help( "Set SETUP_SHIP state!" );
	} else if ( gameState == SHOOT ) {
		draw_help( "Set SHOOT state!" );
	} else if ( gameState == END_GAME ) {
		draw_help( "Set END_GAME state!" );
	} else if ( gameState == WAIT_ENEMY ) {
		draw_help( "Set WAIT_ENEMY state!" );
	} else if ( gameState == WAIT_STEP ) {
		draw_help( "Set WAIT_STEP state!" );
	} else {
		draw_ERROR( "changeGameState", "Wrong argument gameState" );
	}
	//getch();
}
