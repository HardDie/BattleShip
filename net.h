#ifndef _NET_H_
#define _NET_H_

void    net_setup( const char typeConnection );
char    net_checkIP();
void    net_createSocket( const char typeConnection );
void	net_connectOpponent( const char typeConnection );
void	net_sendMessage( const char* pointer, const size_t sizeMsg );
void	net_recvMessage( char* pointer, const size_t sizeMsg );
void	net_closeConnection( const char typeConnection );

#endif
