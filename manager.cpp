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
  clientStatus.resize(num_nodes);
  
  // Start phase 1: Send connection vectors 
  for(int idx = 0; idx < num_nodes; idx++) {
    struct sockaddr_in client;
    socklen_t clientLength = sizeof(client);
    int client_fd = accept(sock_fd, (struct sockaddr*)&client, &clientLength);
    if(client_fd < 0) {
        perror("ERROR on accept");
        exit(1);
    }
    clients.push_back(client_fd);
    cout << "Manager: client_fd: " << client_fd << endl;
    
    int router_number = 0;
    recv(client_fd, &router_number, sizeof(router_number), 0);
    cout << "Manager: Managing: " << router_number << endl;
    
    
	int numberOfIncomingConnections = 0;
	
	int size = 3;
	
    route_table.clear();
    for(int i = 0; i < static_cast<int>(network_table.size()); i++){		 
		for(int c = 0; c < static_cast<int>(network_table[i].size() - 1); c++){
			//cout << tbl[i][c] << " ";
			if(network_table[i][c] == router_number) {
				vector<int> table = network_table.at(i);
				cout<<"Manager: Adding associative table: " << table[0] << " " << table[1]<< " " << table[2] <<" to: "<< router_number << endl;
				//int size = table.size();
						
				numberOfIncomingConnections++;
				route_table.push_back(table);
			} 
			
		}
		
	}
	
	//How many tuples are being sent to the router?	
	cout<<"Manager: numberOfIncomingConnections: " << numberOfIncomingConnections<<endl;
    send(client_fd, &numberOfIncomingConnections, sizeof(numberOfIncomingConnections), 0);
	cout << "Manager: table.size(): " << size << endl;	
	send(client_fd, &size, sizeof(size), 0);
	
	//For every tuple send it to the router 
	if(!route_table.empty()) {
		cout<<"Manager: Table for " << router_number << " is not empty.. attempting to send pair"<<endl;
		for(int j = 0; j< static_cast<int>(route_table.size()); j++){			
				vector<int> table = route_table.at(j);
				cout<<"Manager: sending.. " << table[0] << " " << table[1] << " " << table[2] << " to router: " << router_number << endl; 	
				cout<<"Manager: finding the UDP for Neighbor"<<endl;
				for(int k =0; k<static_cast<int>(route_table.size()-1); k++) {
					cout<<route_table[j][k] << " " <<endl;
				}
				send(client_fd, &table[0], sizeof(int)*size, 0);
		}	
	} else {
		cout<<"Manager: Table for " << router_number << " is empty"<<endl;
	}
	
	/*	
    char buffer[sizeof("Ready!")];
    recv(client_fd, &buffer, sizeof(buffer) , 0);
    clientStatus.at(idx) = SETUP_PHASE;
  }
  
  // Start phase 2: setting up UDP connections
  for(int idx = 0; idx < num_nodes; idx++) {
    int client_fd = clients.at(idx);
    send(client_fd, "Go!", sizeof("Go!"), 0);
   */ 
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
    
    listen(sock_fd, 127);
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
