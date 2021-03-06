#ifndef _GAME_H_
#define _GAME_H_

void	game_initVariables();
char	game_doStep();
void	game_setUpShips();
char	game_checkShipForSet( const char width, const char pos );
char	game_checkDotForSet( const char x, const char y );
char	game_mainMenu( char* whoPlayer );
char	game_initGame( const char typeConnection );
char	game_waitStep();
char	game_checkShoot( const char shootCoord );
char	game_checkWin( const char typeConnection );

#endif
