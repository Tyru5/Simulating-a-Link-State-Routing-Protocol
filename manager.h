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


using std::string;
using std::vector;


void handle_router_connections(int sock_fd);

class Manager {

 public:
  
 Manager( const string& _input_file): input_file( _input_file ){} // default constructor

  // public member funcitons:
  void parseInputFile();
  int createRouterListener(int port);


 protected:
  string input_file;
  int num_nodes;
  vector < vector<int> > network_table;
};
#endif //MANAGER_H_INCLUDE
