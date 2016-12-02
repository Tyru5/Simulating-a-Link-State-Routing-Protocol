#ifndef ROUTER_H
#define ROUTER_H
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void router_process(int router_number);
int create_router_socket(int port);
int create_manager_connection();
#endif
