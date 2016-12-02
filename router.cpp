#include "router.h"
#include "project3.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cout << "Incorrect program arguments. Example: ./router 1" << endl; 
        exit(1);
    }
    cout << "argv:" << argv[1] << endl;
    int router_number = atoi(argv[1]);
    router_process(router_number);
    return 0;
}

void router_process(int router_number) {
    int router_socket = create_router_socket(MANAGER_PORT+router_number);
    cout << "Created a UDP socket on " << MANAGER_PORT+router_number << " with the sock_filedescriptor " << router_socket << endl;
    int manager_socket = create_manager_connection();
    const char* router_number_str = to_string(router_number).c_str();
    cout << "router number: " << router_number << endl;
    int status = send(manager_socket, router_number_str, sizeof(router_number_str), 0);
    if(status == -1) {
         cout << "Failed to send the manager my router number :(" << endl;
	 exit(1);
    }
    char buffer[4096];
    status = recv(manager_socket, buffer, sizeof(buffer), 0);
    cout << buffer << endl;
    status = send(manager_socket, "Ready!", sizeof("Ready!"), 0); 
}


int create_manager_connection() {
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client;
    int one = 1;
    
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
    client.sin_family = AF_INET;
    client.sin_port = htons(MANAGER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &client.sin_addr.s_addr);
    
    int result = connect(socket_desc, (struct sockaddr*)(&client), sizeof(client));
    if(result == -1) {
        cout << "Couldn't connect to the manager!" << endl;
        exit(1);
    }
    
    return socket_desc;
}

int create_router_socket(int port) {
    int socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
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
