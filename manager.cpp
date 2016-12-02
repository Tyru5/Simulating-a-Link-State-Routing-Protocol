// PA3 Assignment

#include "manager.h"

// namespace
using namespace std;

/* Function declarations */
void printNT( const vector< vector<int>>& tbl, const int& edges );

int main(int argc, char* argv[]){

  /* The network is managed by a process called the manager. 
     The manager takes as argument a single file. You will run the manager as follows.
   */
  
  Manager network_manager( argv[1] );
  network_manager.parseInputFile();
  
  network_manager.createRouterListener(MANAGER_PORT);  
  // Spawn each router for the Network Topology:
  network_manager.spawnRouters();
  network_manager.configureRouters();


  
  return 0;
}

void Manager::configureRouters() {
  socklen_t clientLength;
  struct sockaddr_in client;
  clientStatus.resize(num_nodes);
  for(int idx = 0; idx < num_nodes; idx++) {
    int client_fd = accept(sock_fd, (struct sockaddr*)&client, &clientLength);
    clients.push_back(client_fd);
    char recv_buffer[11];
    recv(client_fd, &recv_buffer, sizeof(recv_buffer), 0);
    cout << "router " << recv_buffer << " connected." << endl;
    //network_table.get(atoi(recv_buffer));
    int router_number = atoi(recv_buffer);
    vector<int> table = network_table.at(router_number);
    int size = table.size();
    const char* sizeStr = to_string(size).c_str();
    send(client_fd, &size, sizeof(size), 0);
    send(client_fd, &table[0], sizeof(int)*size, 0);
    char buffer[sizeof("Ready!")];
    recv(client_fd, &buffer, sizeof(buffer) , 0);
    clientStatus.at(idx) = SETUP_PHASE;
  }
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

  if(DEBUG) cout << "num_nodes = " << num_nodes << endl;

  while( getline( file, line ) ){

    if( line == "-1") break;

    num_edges++;
    num_lines++; // these two are essentially the same thing haha...
    
  } // end of while statement.

  /* Second pass through of the file.. i know, not the best */
  // allocate network table size:
  network_table = vector< vector<int> >( num_lines, vector<int>(3, -1) );
  
  ifstream file2( input_file );

  // Get how many nodes there are in the file:
  stringstream node_stream2;
  int discard = 0;
  getline( file2, line );
  node_stream2 << line;
  node_stream2 >> discard;
  
  stringstream iss;
  int row = 0;
  int col = 0;
  while( getline( file2, line ) ){

    if( line == "-1") break;

    iss << line;
    // cout << iss.str() << endl;

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

  if(DEBUG) printNT( network_table, num_edges );
  
  
}

void handle_router_connections( int sock_fd ) {
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
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    int one = 1;

    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);
    
    if( bind(sock_fd, (struct sockaddr *) &server, sizeof(server)) == -1 ){ // means it failed to bind... 0 on success, -1 if not
      cout << "Couldn't bind the port to the socket!" << endl;
      exit(1);
    }
    
    listen(sock_fd, 1);
    return sock_fd;				 
}

// The manager will spawn one Unix process for each router in the network.
void Manager::spawnRouters(){


  for(int i = 0; i < num_nodes; i++){

    //delcaring and initializing the fork sytem call:
    //fork a child process
    pid_t routerN = fork();


    //Checking -> Errors and parent or child status:
    if( routerN < 0){
      
      //opps, something went wrong and the fork() couldn't happen!
      printf("Sorry! Something went wrong. Couldn't fork() the child process. Exiting now...\n");
      
    }
    else if( routerN == 0 ){/*child process*/
      
      //a return value of zero on a fork() means that it is running in new child process. On success, the PID of the child is returned in the parent, and 0 is returned in the child
      //run the exec() unix command to run the expoxch.c program.
      
      execl("./router","router", std::to_string(i).c_str(), NULL);
      
    }
    else{/*parent process*/
      
      //run the wait() unix system call that waits for a program to finish/change state
      /*      printf("IN PARENT:Child's PID =  %d\n", routerN );
      int status;
      waitpid( routerN, &status, 0 );
      //checking status codes:
      if ( WIFEXITED(status) ){ // returns true if the child terminated normally
	if(status == 0){
	  printf("IN PARENT: Child's exit code: %d (OK)\n", WEXITSTATUS(status));
	}
	else{
	  printf("IN PARENT: Child's exit code: %d (error)\n", WEXITSTATUS(status));
	}
      }
      printf("IN PARENT: Parent (PID = %d): done\n",getppid());
*/    }
    
  } // end of for
  
}

// ~~~~~~~~~~~~~~~~~~~~~HELPER FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Function that prints out the network_table AND number of edges */
void printNT( const vector< vector<int>>& tbl, const int& edges ){
  
  // cout << "printing out the table now! " << endl;
  for(int i = 0; i < static_cast<int>(tbl.size()); i++){
    for(int c = 0; c < static_cast<int>(tbl[i].size()); c++){
      cout << tbl[i][c] << " ";
    }
    cout << endl;
  }

  cout << "The number of edges in the network = " << edges << endl;
  
}
