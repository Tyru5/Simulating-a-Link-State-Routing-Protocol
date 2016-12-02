// PA3 Assignment

#include "manager.h"
#include "project3.h"

// namespace
using namespace std;

#define DEBUG false

int main(int argc, char* argv[]){

  /* The network is managed by a process called the manager. 
     The manager takes as argument a single file. You will run the manager as follows.
   */

  Manager network_manager( argv[1] );
  network_manager.parseInputFile();
  
  int sock_fd = network_manager.createRouterListener(MANAGER_PORT);  
  //pthread_t threadId;
  //pthread_create(&threadId, NULL, handle_router_connections, (void*)&router_socket);
   struct sockaddr_in client;
   socklen_t clientLength;
   listen(sock_fd, 1);
   int client_fd = accept(sock_fd, (struct sockaddr*)&client, &clientLength);
  return 0;
}


void Manager::parseInputFile(){

  string line;
  
  ifstream file( input_file  );
  
  if( !file ){
    cout << "Sorry! Could open [" << input_file << "] !" << endl;
  }

  // Get how many nodes there are in the file:
  stringstream node_stream;
  getline( file, line );
  node_stream << line;
  node_stream >> num_nodes;

  // allocate network table size:
  network_table = vector< vector<int> >( num_nodes, vector<int>(3, -1) );
  
  stringstream iss;
  int row = 0;
  int col = 0;
  while( getline( file, line ) ){

    if( line == "-1") break;
    
    iss << line;
    if(DEBUG) cout << iss.str() << endl;

    iss >> network_table[row][col];
    col++; // col 1
    iss >> network_table[row][col];
    col++; // col 2
    iss >> network_table[row][col];

    row++;
    col = 0; // reset
    
    // clearing the stringstream:
    iss.str( string() );
    iss.clear();
    
  } // end of while statement.

  cout << "printing out the table now! " << endl;
  for(int i = 0; i < num_nodes; i++){
    for(int c = 0; c < 3; c++){
      cout << network_table[i][c] << " ";
    }
    cout << endl;
  }
}

void handle_router_connections(int sock_fd) {
    struct sockaddr_in client;
    socklen_t clientLength;
 	listen(sock_fd, 1);
    while(1) {
        int client_fd = accept(sock_fd, (struct sockaddr*)&client, &clientLength);   
        //pthread_t threadId;
        //pthread_create(&threadId, NULL, handle_connection, (void*)&client_fd);
    }
}

int Manager::createRouterListener(int port) {
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
