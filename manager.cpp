// PA3 Assignment

#include "manager.h"

// namespace
using namespace std;

/* Function declarations */
void printNT( const vector< vector<int>>& tbl, const int& edges );

int main(int argc, char* argv[]){

  ofstream m_out( "manager.out" );
  if( !m_out ) cout << "Sorry! Couldn't write out from manager! " << endl;
  
  Manager network_manager( argv[1], argv[2] );
  network_manager.parseInputFile();
  network_manager.parseMessageFile();
  
  network_manager.createRouterListener(MANAGER_PORT);
  
  // Spawn each router for the Network Topology:

  network_manager.spawnRouters( m_out );
  usleep(1000);
  network_manager.configureRouters( m_out );
  network_manager.waitForChildren();
  
  return 0;
}

int Manager::getNumberOfIncomingConnections(const int router_number) {
  //cout << "start getNumberOfIncomingConnections()" << endl;
  int connections = 0;
  for(const auto &ourMap: network_map){
    if(ourMap.first != router_number) {
      for(int idx = 0; idx < static_cast<int>( ourMap.second.size() ); idx++) {
	LSP lsp = ourMap.second.at(idx);
	//cout << lsp.destination << " == " << router_number << endl;
	if(lsp.destination == router_number) connections++;
      }   
    }
  }
  //cout << "end getNumberOfIncomingConnections()" << endl;
  return connections;
}

void Manager::configureRouters( ofstream& ofstr ) {
  
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
    cout << "Manager: client_fd: " << client_fd << endl;
    
    int router_number = 0;
    recv(client_fd, &router_number, sizeof(router_number), 0);
    cout << "Manager: Managing: " << router_number << endl;
    clients.push_back( make_tuple(router_number, client_fd) );
    	
    vector<LSP> router_nodes = network_map.find(router_number)->second;
    int size = router_nodes.size();
	
    route_table.clear();
    ROUTER_INFO router_info;            //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
    
    router_info.number_incoming_connections = getNumberOfIncomingConnections(router_number);
    router_info.number_nodes = num_nodes;
    router_info.number_edges = num_edges;
    if(DEBUG) {
      cout << "Manger: Debug: number_incoming_connections:" << router_info.number_incoming_connections << endl;
      cout << "Manger: Debug: number_nodes:" << router_info.number_nodes << endl;
      cout << "Manger: Debug: number_edges:" << router_info.number_edges << endl;
    }
    
    //How many tuples are being sent to the router?	
    send(client_fd, &router_info, sizeof(router_info) , 0);
    cout << "Manager: table.size(): " << size << endl;	
    send(client_fd, &size, sizeof(size), 0);
    send(client_fd, &router_nodes[0], sizeof(LSP)*size, 0);
    
    cout << "WAITING ON: " << router_number << endl;
    char buffer[sizeof("Ready!")];
    recv(client_fd, &buffer, sizeof(buffer) , 0);
    clientStatus.at(idx) = SETUP_PHASE;
  }


  char go[10];
  memset( go, 0, sizeof(go) );
  strcpy( go, "Go!");

  ofstr << "===== :: End Of Spawning Routers :: ===== " << endl << endl;
  ofstr << "=== :: Letting The Routers know they can start setting up UDP connections :: ===" << endl;
  
  // Start phase 2: setting up UDP connections
  for(int idx = 0; idx < num_nodes; idx++) {
    auto start = chrono::high_resolution_clock::now(); // start timer
    int client_fd = get<1>(clients.at(idx));
    send(client_fd, go, sizeof(go), 0);
    auto finish = chrono::high_resolution_clock::now(); // finish timer
    ofstr << "Manager sending ACK @ time (ns): " << chrono::duration_cast<chrono::nanoseconds>(finish-start).count() << " with ACK message: " << go << endl;
  }
  
  /* END OF PHASE 2*/
  for(int idx = 0; idx < num_nodes; idx++) {
    auto start = chrono::high_resolution_clock::now(); // start timer
    int client_fd = get<1>(clients.at(idx));
    int recv_status = 0;
    recv(client_fd, &recv_status, sizeof(recv_status), 0);
    auto finish = chrono::high_resolution_clock::now(); // finish timer
    ofstr << "Router " << (MANAGER_PORT + idx) << " send ACK back @ time (ns): " << chrono::duration_cast<chrono::nanoseconds>(finish-start).count() << " with ACK val of: " << recv_status << endl;
    // cout << recv_status << endl;
  }

  ofstr << "=== :: End of Letting The Routers know they can start setting up UDP connections :: ===" << endl << endl;;
  ofstr << "=== :: Now Messages :: === " << endl;
  
  /*START OF PHASE 3*/
  for(int idx = 0; idx < num_nodes; idx++) {
    int client_fd = get<1>(clients.at(idx));
    send(client_fd, go, sizeof(go), 0);
  }
  
  // END OF PHASE 3
  for(int idx = 0; idx < num_nodes; idx++) {
    int client_fd = get<1>(clients.at(idx));
    int recv_status = 0;
    recv(client_fd, &recv_status, sizeof(recv_status), 0);
    cout << recv_status << endl;
  }
  
  // START OF PHASE 4
  vector<tuple<int,int>> messagesExpected;
  for(int idx = 0; idx < num_nodes; idx++) {
    int client_fd = get<1>(clients.at(idx));
    vector<Packet> messagesToSend;
    for(int i = 0; i < static_cast<int>(messages.size()); i++) {
      if(messages.at(i).src == get<0>(clients.at(idx))) {
	messagesToSend.push_back(messages.at(i));   
      }
    }
    
    int msg_size = messagesToSend.size();
    send(client_fd, &msg_size, sizeof(msg_size), 0);
    send(client_fd, &messagesToSend[0], sizeof(Packet)*msg_size, 0);

    int expected_msg = 0;
    for(int i = 0; i < static_cast<int>(messages.size()); i++) {
      if(messages.at(i).dest == get<0>(clients.at(idx))) {
	expected_msg++;  
      }
    }
    messagesExpected.push_back(make_tuple(client_fd, expected_msg));
  }
  cout << "Manager waiting for messages" << endl;
  ofstr <<"Manager waiting for messages..." << endl;
  auto start20 = chrono::high_resolution_clock::now(); // start timer
  for(int idx = 0; idx < static_cast<int>(messagesExpected.size()); idx++) {
    int expect_count = get<1>(messagesExpected.at(idx));
    int client_fd = get<0>(messagesExpected.at(idx));
    cout << "Manager recv from " << client_fd << " for " << expect_count << " messages" << endl;
    auto end20 = chrono::high_resolution_clock::now(); // start timer
    ofstr << "@ time (ns):" << chrono::duration_cast<chrono::nanoseconds>(end20-start20).count() << " Manager recv from " << client_fd << " for " << expect_count << " messages" << endl; 
  }
  auto start22 = chrono::high_resolution_clock::now(); // start timer  
  for(int idx = 0; idx < static_cast<int>(messagesExpected.size()); idx++) {
    int expect_count = get<1>(messagesExpected.at(idx));
    int client_fd = get<0>(messagesExpected.at(idx));
    for(int i = 0; i < expect_count; i++) {
      int recv_status = 0;
      recv(client_fd, &recv_status, sizeof(recv_status), 0);
      cout << "Manager recv from " << client_fd;
      auto end22 = chrono::high_resolution_clock::now(); // start timer
      ofstr << "@ time (ns):" << chrono::duration_cast<chrono::nanoseconds>(end22-start22).count() << " Manager recv from router: " << client_fd << endl; 
    }
  }

  ofstr << "=== :: Manager has recieved all Messages and has sent the packet through sucessfully! :: === " << endl << endl;
  
  // *NOW* closing the ofstream:
  ofstr.close();
  
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
  
  for(int idx = 0; idx < num_nodes; idx++) {
    vector<LSP> nodes; 
    network_map.insert(pair<int, vector<LSP>>(idx, nodes));
  }
  
  for(int i = 0; i < static_cast<int>(network_table.size()); i++){
    map<int, vector<LSP>>::iterator it = network_map.find(network_table[i][0]);
    LSP lsp;
    lsp.source = network_table[i][0];
    lsp.destination = network_table[i][1];
    lsp.cost = network_table[i][2];
    it->second.push_back(lsp);
  }

  for(int i = 0; i < static_cast<int>(network_table.size()); i++){
    map<int, vector<LSP>>::iterator it = network_map.find(network_table[i][1]);
    LSP lsp;
    lsp.source = network_table[i][1];
    lsp.destination = network_table[i][0]; // flipped
    lsp.cost = network_table[i][2];
    it->second.push_back(lsp);
  }

  
  if(DEBUG) printNT( network_table, num_edges );
  
  
}

void Manager::parseMessageFile(){
    
    
  ifstream mfile( message_file  );
  
  if( !mfile ){
    cout << "Sorry! Could open [" << message_file << "] !" << endl;
  }
    
  string line;
    
  Packet message_packet;
  stringstream message_stream;
  int src,dest;
  while( getline( mfile, line ) ){

    // cout << message_stream.str() << endl;

    message_stream << line;
    message_stream >> src >> dest;
    message_packet.src = src;
    message_packet.dest = dest;
        
    messages.push_back( message_packet );
    // clearing the stringstream:
    message_stream.str( string() );
    message_stream.clear();
    
  } // end of while statement.
  
  for(int i =0; i < static_cast<int>(messages.size()); i++) cout << messages.at(i).src << " : " << messages.at(i).dest << endl;
    
}

void Manager::waitForChildren(){
  for( int idx = 0; idx < static_cast<int>(child_pross.size()); idx++){
    //run the wait() unix system call that waits for a program to finish/change state
    printf("KILLING PID =  %d\n", child_pross[idx] );
	kill( child_pross[idx], 9 );
  }   
}

void Manager::debugMap() {
  for(const auto &ourMap: network_map){
    cout << "===" << ourMap.first << "===" << endl;
    for(int idx = 0; idx < static_cast<int>( ourMap.second.size() ); idx++) {
      LSP lsp = ourMap.second.at(idx);
      cout << lsp.source << " " << lsp.destination << " " << lsp.cost << endl;
    }
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
    
  listen(sock_fd, 127);
  return sock_fd;				 
}

// The manager will spawn one Unix process for each router in the network.
void Manager::spawnRouters( ofstream& ofstr ){

  ofstr << "===== :: Spawning Routers :: ===== " << endl;
  
  for(int i = 0; i < num_nodes; i++){

    //delcaring and initializing the fork sytem call:
    //fork a child process

    auto start = chrono::high_resolution_clock::now(); // start timer
    pid_t routerN = fork();
    child_pross.push_back( routerN );

    //Checking -> Errors and parent or child status:
    if( routerN < 0){
      
      //opps, something went wrong and the fork() couldn't happen!
      printf("Sorry! Something went wrong. Couldn't fork() the child process. Exiting now...\n");
      
    }
    else if( routerN == 0 ){/*child process*/
      
      //a return value of zero on a fork() means that it is running in new child process. On success, the PID of the child is returned in the parent, and 0 is returned in the child
      //run the exec() unix command to run the expoxch.c program.

      auto finish = chrono::high_resolution_clock::now(); // finish timer
      ofstr << "Spawned router (child process) @ time (ns): " << chrono::duration_cast<chrono::nanoseconds>(finish-start).count() << " with router id: " << MANAGER_PORT + i << endl;
    
      execl("./router","router", std::to_string(i).c_str(), NULL);
      
    }
    
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
