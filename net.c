#include "net.h"

unsigned short    port;
char              ip[16] = "";

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
      if ( ip[i] == '0' && ip[ i - 1 ] == '.' && ip[ i + 1 ] != '.' ) { // Если 0 в значении стоит первым, и после него нету точки, то это неверно
        return 1;
      }
    } else if ( i == 0 && ip[i] == '0' && ip[ i + 1 ] != '.' ) {
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
  struct sockaddr_in  server_addr;
	int                 true = 1;

	if ( ( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) {
		draw_ERROR( "net_createSocket", "Create socket" );
	}

	if ( setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int) ) == -1 ) {
		close( sock );
		draw_ERROR( "net_createSocket", "Setsockopt" );
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( 5000 );
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero( &( server_addr.sin_zero ), 8 );

	if ( bind( sock, ( struct sockaddr * )&server_addr, sizeof( struct sockaddr ) ) == -1 ) {
		close( sock );
		draw_ERROR( "net_createSocket", "Unable to bind" );
	}
	if ( listen( sock, 1 ) == -1 ) {
		close( sock );
		draw_ERROR( "net_createSocket", "Listen" );
	}
}
