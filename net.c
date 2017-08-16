#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <ncurses.h>
#include "draw.h"
#include "global.h"

#include "net.h"

unsigned short    	port;
char              	ip[16] = "";
int					sock_server, sock_enemy;
struct sockaddr_in 	server_addr;

/*
 * Name: net_setup
 * Description: Инициализируются все настройки для сетевой игры
 * */
void net_setup( const char typeConnection ) {
	echo();		// Включаем отображение вводимых символов

	if ( typeConnection == NET_CLIENT ) {           // Настройка клиента
		draw_netClientIPEnter();
		draw_help( "Enter server IP address(255.255.255.255)" );

		WINDOW *input = NULL; // Создаем окно для ввода
		input = subwin( stdscr, 1, 16, 1, 1 );  // Инициализируем его
		while ( wgetstr( input, ip ) || net_checkIP() ) {	// Проверяем IP на правильность ввода
			draw_netClientIPEnter();
			draw_help( "Wrong input, try again. Enter server IP address(255.255.255.255)" );
		}
		delwin( input );

		draw_netClientPortEnter( ip );
		draw_help( "Input port(1025-10000)" );
		while ( ( !scanw( "%d", &port ) ) || port < 1025 || port > 10000 ) {   // Считываем порт
			draw_netClientPortEnter( ip );
			draw_help( "Wrong input, try again. Input port(1025-10000)" );
		}
	} else if ( typeConnection == NET_SERVER ) {      // Настройка сервера
		draw_netServerPortEnter();
		draw_help( "Input port(1025-10000)" );
		while ( ( !scanw( "%d", &port ) ) || port < 1025 || port > 10000 ) {   // Считываем порт
			draw_netServerPortEnter();
			draw_help( "Wrong input, try again. Input port(1025-10000)" );
		}
	} else {
		draw_ERROR( "net_setup", "Wrong argument typeConnection" );
	}

	noecho();	// Отключаем отображение символов
}

/*
 * Name: net_checkIP
 * Description: Проверяет введенный IP на верность, в случае верности IP возвращает 0, иначе 1
 * */
char net_checkIP() {

	if ( strcmp( ip, "localhost" ) == 0 ) {
		return 0;
	}

	if ( strlen( ip ) > 15 || strlen( ip ) < 7 ) {  // Проверка на длину IP адреса
		return 1;
	}

	char dotCount = 0;
	for ( int i = 0; i < strlen( ip ); i++ ) {
		if ( ip[i] == '.' ) { 	// Проверка но количество точек
			dotCount++;
			continue;
		}
		if ( ip[i] < 48 || ip[i] > 57 ) {  // Проверка на то, является ли символ цифрой
			return 1;
		}
	}
	if ( dotCount != 3 ) {
		return 1;
	}

		// Проверка значений ip адреса, начинаем с конца строки
	for ( int i = strlen( ip ) - 1, val = -1, mn = 1; i >= -1; i-- ) {

		if ( i == -1 || ip[i] == '.' ) {  // Проверка на превышения интервала или если первый символ '.'
			if ( val > 255 || val == -1 ) {
				return 1;
			}
			val = 0;
			mn = 1;
		} else {
			val += mn * ( ip[i] - 48 );
			mn *= 10;
		}

		if ( !(( i == 0 ) || ( i == strlen( ip ) - 1 )) ) {  // Не учитываем в проверке первый и последний символ
			if ( ip[i] == '0' && ip[i - 1] == '.' && ip[i + 1] != '.' ) { // Если 0 в значении стоит первым, и после него нету точки, то это неверно
				return 1;
			}
		} else if ( i == 0 && ip[i] == '0' && ip[i + 1] != '.' ) {	// Если первый символ в строке 0, и после нее нет точки, ошибка
			return 1;
		}

	}

	return 0;
}

/*
 * Name: net_createSocket
 * Desription: Происходит создание сокета
 * */
void net_createSocket( const char typeConnection ) {
	//port=1111;
	//strcpy( ip, "localhost" );
	draw_loadFullScreen( "Create socket" );

	server_addr.sin_family = AF_INET;		// Инициализация параметров
	server_addr.sin_port = htons( port );
	bzero( &( server_addr.sin_zero ), 8 );

	if ( typeConnection == NET_SERVER ) {	// Создаем сокет для сервера
		int val_true = 1;

		if ( ( sock_server = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) {		// Создаем сокет
			draw_ERROR( "net_createSocket", "Server create socket" );
		}

		if ( setsockopt( sock_server, SOL_SOCKET, SO_REUSEADDR, &val_true, sizeof( int ) ) == -1 ) {
			close( sock_server );
			draw_ERROR( "net_createSocket", "Server. Setsockopt" );
		}

		server_addr.sin_addr.s_addr = INADDR_ANY;

		if ( bind( sock_server, ( struct sockaddr * )&server_addr, sizeof( struct sockaddr ) ) == -1 ) {
			close( sock_server );
			draw_ERROR( "net_createSocket", "Server. Unable to bind" );
		}
		if ( listen( sock_server, 1 ) == -1 ) {
			close( sock_server );
			draw_ERROR( "net_createSocket", "Server. Listen" );
		}
	} else if ( typeConnection == NET_CLIENT ) {		// Создаем сокет для клиента
		struct hostent *host;
		host = gethostbyname( ip );

		if ( ( sock_enemy = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) {		// Создаем сокет
			draw_ERROR( "net_createSocket", "Client create socket" );
		}

		server_addr.sin_addr = *( ( struct in_addr * )host->h_addr_list[0] );
	} else {
		draw_ERROR( "net_createSocket", "Wrong argument typeConnection" );
	}
	draw_closeLoadScreen();		// Завершаем дочерний процесс отрисовки экрана загрузки
}

/*
 * Name: net_connectOpponent
 * Description: Подключение к оппоненту
 * */
void net_connectOpponent( const char typeConnection ) {
	draw_loadFullScreen( "Wait opponent" );
	if ( typeConnection == NET_SERVER ) {
		struct sockaddr_in 	client_addr;
		int 				sin_size;
		sin_size = sizeof( struct sockaddr_in );
		sock_enemy = accept( sock_server, ( struct sockaddr* )&client_addr, ( socklen_t* )&sin_size );
	} else if ( typeConnection == NET_CLIENT ) {
		if ( connect( sock_enemy, ( struct sockaddr * )&server_addr, sizeof( struct sockaddr ) ) == -1 ) {
			draw_ERROR( "net_connectOpponent", "Connect to server" );
		}
	} else {
		draw_ERROR( "net_connectOpponent", "Wrong argument typeConnection" );
	}
	draw_closeLoadScreen();		// Завершаем дочерний процесс отрисовки экрана загрузки
}

/*
 * Name: net_sendMessage
 * Description: Отправляет пакет данных оппоненту
 * */
void net_sendMessage( const char* pointer, const size_t sizeMsg ) {
	send( sock_enemy, pointer, sizeMsg, 0 );
}

/*
 * Name: net_recvMessage
 * Description: Принимает пакет данных от оппонента, результат приема записывается в указатель
 * */
void net_recvMessage( char* pointer, const size_t sizeMsg ) {
	if ( recv( sock_enemy, pointer, sizeMsg, 0 ) == 0 ) {
			draw_ERROR( "net_recvMessage", "Enemy disconnect" );
	}
}

/*
 * Name: net_closeConnection
 * Description: Закрывает сокеты
 * */
void net_closeConnection( const char typeConnection ) {

	if ( typeConnection == NET_SERVER ) {
		close( sock_enemy );
		close( sock_server );
	} else if ( typeConnection == NET_CLIENT ) {
		close( sock_enemy );
	} else {
		draw_ERROR( "net_closeConnection", "Wrong argument typeConnection" );
	}

}
