#ifndef _DRAW_H_
#define _DRAW_H_

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
void	draw_loadFullScreen( const char* loadText );
void	draw_loadText( const char* loadText );
void	draw_closeLoadScreen();

#endif
