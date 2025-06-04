#include <iostream>
#include <sstream>
#include <unistd.h>

#include "../include/http_tcpServer_linux.h"

int			main(int ac, char **av)
{
  using namespace	http;
  TcpServer		server = TcpServer("0.0.0.0", 8080);

  server.startListen();
  return 0;
}
