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
