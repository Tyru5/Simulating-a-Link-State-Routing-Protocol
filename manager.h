// Author: Tyrus  :: Sam :: Torry

#ifndef MANAGER_H_INCLUDE
#define MANAGER_H_INCLUDE

// custom headers
#include "project3.h"

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
#include <map>
#include <tuple>

// For Fork() -- Child processes
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using std::string;
using std::vector;
using std::tuple;


void handle_router_connections(int sock_fd);
 
class Manager {

 public:
  
 Manager( const string& _input_file, const string& _message_file ): input_file( _input_file ), message_file(_message_file){} // default constructor

  // public member funcitons:
  void parseInputFile();
  int createRouterListener( int port );
  void spawnRouters();
  void configureRouters();
  void debugMap();
  int getNumberOfIncomingConnections(const int router_number);
  void parseMessageFile();
  void waitForChildren();
  
 protected:
  
  string input_file;
  string message_file;
  int num_lines = 0;
  int num_nodes;
  int num_edges = 0;
  vector < vector<int> > network_table;
  vector < vector<int> > route_table;
  int sock_fd;
  vector<int> clientStatus;
  vector<tuple<int, int>> clients;
  map<int, vector<LSP>> network_map;
  vector< Packet > messages;
  vector< pid_t > child_pross;
  
};
#endif //MANAGER_H_INCLUDE
