// Author: Tyrus  :: Sam :: Torry

#ifndef MANAGER_H_INCLUDE
#define MANAGER_H_INCLUDE

// directives:
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

// For Fork() -- Child processes
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using std::string;
using std::vector;


void handle_router_connections(int sock_fd);

class Manager {

 public:
  
 Manager( const string& _input_file ): input_file( _input_file ){} // default constructor

  // public member funcitons:
  void parseInputFile();
  int createRouterListener( int port );
  void spawnRouters();
  void configureRouters();

 protected:
  
  string input_file;
  int num_lines = 0;
  int num_nodes;
  int num_edges = 0;
  vector < vector<int> > network_table;
  int sock_fd;
  vector <int> clients;


  
};
#endif //MANAGER_H_INCLUDE
