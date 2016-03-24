#ifndef _NET_H_
#define _NET_H_

#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <ncurses.h>
#include <netdb.h>
#include "draw.h"

void    net_setup( const char typeConnection );
char    net_checkIP();
void    net_createSocket( const char typeConnection );

#endif
