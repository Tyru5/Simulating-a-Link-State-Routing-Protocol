#ifndef ROUTER_H
#define ROUTER_H

#include "project3.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

using std::vector;

class Router {

  
 public:
 Router(const int&  _router_number): // the c++ way .. ;p
  router_number( _router_number )
  ,port( MANAGER_PORT + router_number )
  {}
  
  void routerProcess();

  
 protected: 
  int router_number;
  int router_socket;
  int port;
  int createRouterSocket();
  int createManagerConnection();
  std::vector<int> table;
  void checkError(int status);
  map<int, vector<LSP>> network_map;
  
};

#endif // ROUTER_H
