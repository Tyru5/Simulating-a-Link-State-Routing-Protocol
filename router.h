#ifndef ROUTER_H
#define ROUTER_H

#include "project3.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits> // for numeric_limits
 
#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>
#include <list>
#include <pthread.h>

using std::vector;
using std::list;
using std::ofstream;
using std::cout;
using std::endl;

/* v CODE TAKEN FROM https://rosettacode.org/wiki/Dijkstra's_algorithm#C.2B.2B */
typedef int vertex_t;
typedef double weight_t;
 
const weight_t max_weight = std::numeric_limits<double>::infinity();
 
struct neighbor {
  vertex_t target;
  weight_t weight;
neighbor(vertex_t arg_target, weight_t arg_weight)
: target(arg_target), weight(arg_weight) { }
};
 
typedef std::vector<std::vector<neighbor> > adjacency_list_t;
/* ^ CODE TAKEN FROM https://rosettacode.org/wiki/Dijkstra's_algorithm#C.2B.2B ^ */
void* notify_manager(void* args);
class Router {

  
 public:
 Router(const int&  _router_number, ofstream& _out): // the c++ way .. ;p
  router_number( _router_number )
    ,port( MANAGER_PORT + router_number )
    ,r_out( _out)
    {}

  
  void initNetworkMap(int number_nodes);
  void routerProcess();
  struct sockaddr_in getRouterSockAddr(int router_number);
  bool netWrkTableFull();
  void debugMap();
  /* v CODE TAKEN FROM https://rosettacode.org/wiki/Dijkstra's_algorithm#C.2B.2B */
  std::list<vertex_t> DijkstraGetShortestPathTo(vertex_t vertex, const std::vector<vertex_t> &previous);
  void DijkstraComputePaths(vertex_t source, const adjacency_list_t &adjacency_list,
			    std::vector<weight_t> &min_distance,std::vector<vertex_t> &previous);
  /* ^ CODE TAKEN FROM https://rosettacode.org/wiki/Dijkstra's_algorithm#C.2B.2B ^ */
    
 protected: 
  int router_number;
  int router_socket;
  int port;
  ofstream& r_out;
  int createRouterSocket();
  int createManagerConnection();
  void checkError( int status );
  map<int, vector<LSP>> network_map;
  ROUTER_INFO router_info;
  vector<int> router_connections;
  
};

#endif // ROUTER_H
