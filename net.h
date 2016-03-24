#ifndef _NET_H_
#define _NET_H_

#include <ncurses.h>
#include "draw.h"

void    net_setup( const char typeConnection );
char    net_checkIP();
void    net_createSocket( const char typeConnection );

#endif
