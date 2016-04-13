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
	gameState = GS_MENU;
	game_initVariables();
	setUpScreen();

	//============================
	//	Game
	//============================
	while ( !gameIsDone ) {
		if ( gameState == GS_MENU ) {

			switch ( game_mainMenu( &whoPlayer ) ) {
			case GS_NET_SETUP:
				changeGameState( GS_NET_SETUP );
				break;
			case GS_END_GAME:
				changeGameState( GS_END_GAME );
				break;
			}

		} else if ( gameState == GS_NET_SETUP ) {

			//net_setup( whoPlayer );
			net_createSocket( whoPlayer );
			net_connectOpponent( whoPlayer );
			changeGameState( GS_SETUP_SHIP );

		} else if ( gameState == GS_SETUP_SHIP ) {

			draw_labels();
			game_setUpShips();
			changeGameState( GS_WAIT_ENEMY );

		} else if ( gameState == GS_WAIT_ENEMY ) {

			whoFirst = game_initGame( whoPlayer );
			if ( whoFirst == 1 ) {	// 1 - ходит первым, 2 - вторым
				changeGameState( GS_SHOOT );
			} else if ( whoFirst == 2 ) {
				changeGameState( GS_WAIT_STEP );
			} else {
				draw_ERROR( "main", "Wrong argument whoPlayer" );
			}

		} else if ( gameState == GS_SHOOT ) {

			switch ( game_doStep() ) {
				case SHOOT_BIT:
					break;
				case SHOOT_MISS:
					changeGameState( GS_WAIT_STEP );
					break;
			}

			switch( game_checkWin( whoPlayer ) ) {
				case GAME_WIN:
					changeGameState( GS_YOU_WIN );
					break;
				case GAME_LOSE:
					changeGameState( GS_YOU_LOSE );
					break;
				case GAME_NOTHING:
					break;
			}

		} else if ( gameState == GS_WAIT_STEP ) {

			switch ( game_waitStep() ) {
				case SHOOT_BIT:
					break;
				case SHOOT_MISS:
					changeGameState( GS_SHOOT );
					break;
			}

			switch( game_checkWin( whoPlayer ) ) {
				case GAME_WIN:
					changeGameState( GS_YOU_WIN );
					break;
				case GAME_LOSE:
					changeGameState( GS_YOU_LOSE );
					break;
				case GAME_NOTHING:
					break;
			}

		} else if ( gameState == GS_YOU_WIN ) {

			draw_loadFullScreen( "You WIN!" );
			getch();
			draw_closeLoadScreen();
			changeGameState( GS_END_GAME );

		} else if ( gameState == GS_YOU_LOSE ) {

			draw_loadFullScreen( "You LOSE!" );
			getch();
			draw_closeLoadScreen();
			changeGameState( GS_END_GAME );

		} else if ( gameState == GS_END_GAME ) {

			net_closeConnection( whoPlayer );
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
	if ( gameState == GS_MENU ) {
		draw_help( "Leave GS_MENU state!" );
	} else if ( gameState == GS_NET_SETUP ) {
		draw_help( "Leave GS_NET_SETUP state!" );
	} else if ( gameState == GS_SETUP_SHIP ) {
		draw_help( "Leave GS_SETUP_SHIP state!" );
	} else if ( gameState == GS_SHOOT ) {
		draw_help( "Leave GS_SHOOT state!" );
	} else if ( gameState == GS_END_GAME ) {
		draw_help( "Leave GS_END_GAME state!" );
	} else if ( gameState == GS_WAIT_ENEMY ) {
		draw_help( "Leave GS_WAIT_ENEMY state!" );
	} else if ( gameState == GS_WAIT_STEP ) {
		draw_help( "Leave GS_WAIT_STEP state!" );
	} else if ( gameState == GS_YOU_WIN ) {
		draw_help( "Leave GS_YOU_WIN state!" );
	} else if ( gameState == GS_YOU_LOSE ) {
		draw_help( "Leave GS_YOU_LOSE state!" );
	} else {
		draw_ERROR( "changeGameState", "Wrong argument gameState" );
	}
	/*else if ( gameState ==  ) {

	}*/
	//getch();

	gameState = newState;

	if ( gameState == GS_MENU ) {
		draw_help( "Set GS_MENU state!" );
	} else if ( gameState == GS_NET_SETUP ) {
		draw_help( "Set GS_NET_SETUP state!" );
	} else if ( gameState == GS_SETUP_SHIP ) {
		draw_help( "Set GS_SETUP_SHIP state!" );
	} else if ( gameState == GS_SHOOT ) {
		draw_help( "Set GS_SHOOT state!" );
	} else if ( gameState == GS_END_GAME ) {
		draw_help( "Set GS_END_GAME state!" );
	} else if ( gameState == GS_WAIT_ENEMY ) {
		draw_help( "Set GS_WAIT_ENEMY state!" );
	} else if ( gameState == GS_WAIT_STEP ) {
		draw_help( "Set GS_WAIT_STEP state!" );
	} else if ( gameState == GS_YOU_WIN ) {
		draw_help( "Set GS_YOU_WIN state!" );
	} else if ( gameState == GS_YOU_LOSE ) {
		draw_help( "Set GS_YOU_LOSE state!" );
	} else {
		draw_ERROR( "changeGameState", "Wrong argument gameState" );
	}
	//getch();
}
