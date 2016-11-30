// Author: Tyrus  :: Sam :: Torry

#ifndef MANAGER_H_INCLUDE
#define MANAGER_H_INCLUDE

// directives:
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>


using std::string;
using std::vector;

class Manager {

 public:
  
 Manager( const string& _input_file): input_file( _input_file ){} // default constructor

  // public member funcitons:
  void parseInputFile();



 protected:
  string input_file;
  int num_nodes;

  vector < vector<int> > network_table;

  
};

#endif //MANAGER_H_INCLUDE
