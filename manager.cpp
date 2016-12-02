#include "manager.h"
#include "project3.h"

using namespace std;

int main(int argc, char* argv[]) {
    int router_socket = create_router_listener(MANAGER_PORT);  
    handle_router_connections(router_socket);
    return 0;
}

void handle_router_connections(int sock_fd) {
    struct sockaddr_in client;
    socklen_t clientLength;
 	listen(sock_fd, 1);	
	int clientDesc = accept(sock_fd, (struct sockaddr*)&client, &clientLength);   
}

int create_router_listener(int port) {
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    int one = 1;

    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);
    
    if( bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) == -1 ){ // means it failed to bind... 0 on success, -1 if not
      cout << "Couldn't bind the port to the socket!" << endl;
      exit(1);
    }
    
    return socket_desc;
}
