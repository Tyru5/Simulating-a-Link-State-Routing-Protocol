#ifndef ROUTER_H
#define ROUTER_H
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int create_router_listener(int port);
void handle_router_connections(int sock_fd);
#endif
