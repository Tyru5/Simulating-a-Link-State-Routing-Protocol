#ifndef ROUTER_H
#define ROUTER_H
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include "project3.h"

//using std::vector;

class Router {
public:
    Router(const int router_number) { this->router_number = router_number; port = MANAGER_PORT + router_number;}
    void routerProcess();
protected: 
    int router_number;
    int router_socket;
    int port;
    int createRouterSocket();
    int createManagerConnection();
    std::vector<int> table;
    void checkError(int status);
    //vector < vector<int> > rnetwork_table;
   
};

#endif
