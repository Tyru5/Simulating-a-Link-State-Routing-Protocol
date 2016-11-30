#include "router.h"
#include "project3.h"

using namespace std;

int main(int argc, char* argv[]) {
    router_process();
    return 0;
}

void router_process() {
    int router_socket = create_router_socket();
    int manager_socket = create_manager_connection();
    
}

int create_manager_connection() {
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server,client;
    socklen_t clientLength;
        
    server.sin_family = AF_INET;
    server.sin_port = htons(MANAGER_PORT);
    inet_pton(AF_INET, "localhost", &server.sin_addr.s_addr);
    
    int result = connect(socket_desc, (struct sockaddr*)(&server), sizeof(server));
    
    return socket_desc;
}

int create_router_socket() {
        
        
}
