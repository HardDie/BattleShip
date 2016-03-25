#include "net.h"

unsigned short    	port;
char              	ip[16] = "";
int					sock, connected;
struct sockaddr_in 	server_addr;

/*
 * Name: net_setup
 * Description: Инициализируются все настройки для сетевой игры
 * */
void net_setup( const char typeConnection ) {
	echo();

	if ( typeConnection == NET_CLIENT ) {           // Настройка клиента
		draw_netClientIPEnter();
		draw_help( "Enter server IP addres(255.255.255.255)" );

		WINDOW *input = NULL; // Создаем окно для ввода
		input = subwin( stdscr, 1, 16, 1, 1 );  // Инициализируем его
		wgetstr( input, ip ); // Считываем IP
		while ( net_checkIP() ) {
			draw_netClientIPEnter();
			draw_help( "Wrong input, try again. Enter server IP addres(255.255.255.255)" );
			wgetstr( input, ip ); // Считываем IP
		}
		delwin( input );

		draw_netClientPortEnter( ip );
		draw_help( "Input port(1000-10000)" );
		while ( ( !scanw( "%d", &port ) ) || port < 1000 || port > 10000 ) {   // Считываем порт
			draw_netClientPortEnter( ip );
			draw_help( "Wrong input, try again. Input port(1000-10000)" );
		}
	} else if ( typeConnection == NET_SERVER ) {      // Настройка сервера
		draw_netServerPortEnter();
		draw_help( "Input port(1000-10000)" );
		while ( ( !scanw( "%d", &port ) ) || port < 1000 || port > 10000 ) {   // Считываем порт
			draw_netServerPortEnter();
			draw_help( "Wrong input, try again. Input port(1000-10000)" );
		}
	} else {
		draw_ERROR( "net_setup", "Wrong variables typeConnection" );
	}

	noecho();
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
		if ( ip[i] == '.' ) { // Проверка но количество точек
			dotCount++;
		}
		if ( ip[i] < 46 || ip[i] > 57 || ip[i] == 47 ) {  // Проверка на символ цифры
			return 1;
		}
	}
	if ( dotCount != 3 ) {
		return 1;
	}

	for ( int i = strlen( ip ) - 1, val = 0, mn = 1; i >= -1; i-- ) {

		if ( i == -1 || ip[i] == '.' ) {  // Проверка на превышения интервала
			if ( val > 255 ) {
				return 1;
			}
			val = 0;
			mn = 1;
		} else {
			val += mn * ( ip[i] - 48 );
			mn *= 10;
		}

		if ( ( i != 0 ) && ( i != strlen( ip ) - 1 ) ) {  // Не учитываем в проверке первый и последний символ
			if ( ip[i] == '0' && ip[i - 1] == '.' && ip[i + 1] != '.' ) { // Если 0 в значении стоит первым, и после него нету точки, то это неверно
				return 1;
			}
		} else if ( i == 0 && ip[i] == '0' && ip[i + 1] != '.' ) {
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
	int pid;
	pid = fork();
	if ( pid == 0 ) {	// Создаем дочерний процесс отрисовки экрана загрузки
		draw_load( "Create socket" );
	}

	if ( ( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) {		// Создаем сокет
		kill( pid, SIGKILL );	// Завершаем дочерний процесс отрисовки экрана загрузки
		draw_ERROR( "net_createSocket", "Create socket" );
	}

	server_addr.sin_family = AF_INET;		// Инициализация параметров
	server_addr.sin_port = htons( port );
	bzero( &( server_addr.sin_zero ), 8 );

	if ( typeConnection == NET_SERVER ) {
		int val_true = 1;

		if ( setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &val_true, sizeof( int ) ) == -1 ) {
			close( sock );
			kill( pid, SIGKILL );	// Завершаем дочерний процесс отрисовки экрана загрузки
			draw_ERROR( "net_createSocket", "Server. Setsockopt" );
		}

		server_addr.sin_addr.s_addr = INADDR_ANY;

		if ( bind( sock, ( struct sockaddr * )&server_addr, sizeof( struct sockaddr ) ) == -1 ) {
			close( sock );
			kill( pid, SIGKILL );	// Завершаем дочерний процесс отрисовки экрана загрузки
			draw_ERROR( "net_createSocket", "Server. Unable to bind" );
		}
		if ( listen( sock, 1 ) == -1 ) {
			close( sock );
			kill( pid, SIGKILL );	// Завершаем дочерний процесс отрисовки экрана загрузки
			draw_ERROR( "net_createSocket", "Server. Listen" );
		}
	} else if ( typeConnection == NET_CLIENT ) {
		struct hostent *host;
		host = gethostbyname( ip );

		server_addr.sin_addr = *( ( struct in_addr * )host->h_addr_list[0] );
	} else {
		close( sock );
		kill( pid, SIGKILL );	// Завершаем дочерний процесс отрисовки экрана загрузки
		draw_ERROR( "net_createSocket", "Wrong variables typeConnection" );
	}
	kill( pid, SIGKILL );	// Завершаем дочерний процесс отрисовки экрана загрузки
}

/*
 * Name: net_connectOpponent
 * Description: Подключение к оппоненту
 * */
void net_connectOpponent( const char typeConnection ) {
	if ( typeConnection == NET_SERVER ) {
		int pid;
		pid = fork();
		if ( pid == 0 ) {	// Создаем дочерний процесс отрисовки экрана загрузки
			draw_load( "Wait opponent" );
		}

		struct sockaddr_in 	client_addr;
		int 				sin_size;
		sin_size = sizeof( struct sockaddr_in );
		connected = accept( sock, ( struct sockaddr * )&client_addr, &sin_size );

		kill( pid, SIGKILL );	// Завершаем дочерний процесс отрисовки экрана загрузки
	} else if ( typeConnection == NET_CLIENT ) {
		if ( connect( sock, ( struct sockaddr * )&server_addr, sizeof( struct sockaddr ) ) == -1 ) {
			draw_ERROR( "net_connectOpponent", "Connect to server" );
		}
	} else {
		draw_ERROR( "net_connectOpponent", "Wrong variables typeConnection" );
	}
}
