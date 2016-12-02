#include "router.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cout << "Incorrect program arguments. Example: ./router 1" << endl; 
        exit(1);
    }
    cout << "argv:" << argv[1] << endl;
    int router_number = atoi(argv[1]);
    Router router(router_number);
    router.routerProcess();
    return 0;
}

void Router::routerProcess() {
    int router_socket = createRouterSocket();
    cout << "Created a UDP socket on " << MANAGER_PORT+router_number << " with the sock_filedescriptor " << router_socket << endl;
    int manager_socket = createManagerConnection();
    const char* router_number_str = to_string(router_number).c_str();
    cout << "router number: " << router_number << endl;
    int status = send(manager_socket, router_number_str, sizeof(router_number_str), 0);
    if(status == -1) {
        cout << "Failed to send the manager my router number :(" << endl;
        exit(1);
    }
    
    int size = 0;
    status = recv(manager_socket, &size, sizeof(size), 0);
    table.resize(size);

    status = recv(manager_socket, &table[0], sizeof(int)*size, 0);
    
    if(DEBUG)
    {
        cout << "printing table for " << router_number << " ";
        for(int idx = 0; idx < table.size(); idx++) cout << table.at(idx) << " ";
        cout << endl;
    }
    
    status = send(manager_socket, "Ready!", sizeof("Ready!"), 0); 
}

int Router::createManagerConnection() {
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

int Router::createRouterSocket() {
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
