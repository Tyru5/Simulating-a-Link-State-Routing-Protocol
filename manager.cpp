// PA3 Assignment

#include "manager.h"

// namespace
using namespace std;

// Macros:
#define DEBUG false


int main(int argc, char* argv[]){

  /* The network is managed by a process called the manager. 
     The manager takes as argument a single file. You will run the manager as follows.
   */
  
  Manager network_manager( argv[1] );
  
  network_manager.parseInputFile();
  
  // Spawn each router for the Network Topology:
  network_manager.spawnRouters( argv );
  
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

  // cout << "printing out the table now! " << endl;
  for(int i = 0; i < num_nodes; i++){
    for(int c = 0; c < 3; c++){
      cout << network_table[i][c] << " ";
    }
    cout << endl;
  }
				 
}

// The manager will spawn one Unix process for each router in the network.
void Manager::spawnRouters( char* argv[] ){


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
      execl("./router","router", argv[1], NULL);
      
    }
    else{/*parent process*/
      
      //run the wait() unix system call that waits for a program to finish/change state
      printf("IN PARENT:Child's PID =  %d\n", routerN );
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
    }
    
  } // end of for



  
  
}
