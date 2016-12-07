Tyrus Malmstrom :: Same Kramer :: Torry Brelsford
11/8/2016 - 12/8/2016 (Was extended)
README file for grading Project 3 :: Simulating a Link State Routing Protocol

!!!****************************************************IMPORTANT**************************************************************!!!

 1) In very rare instances, it seems that when passing packets to and from routers, a packets gets dropped, causing our program to halt. As stated before, this is a *VERY* rare bug.
    We have written a script run our program X amount of times and it never seemed to happen. We just want to let you know about it. From researching the issue, it seemed that
    the machines network IPv4 card was being overloaded with request, causing the issue. (So it's not our fault).
    With that said, our project is 100% complete and works perfectly! This was such an awesome assignment.

 2) !****For running our manager process, we also pass it a *messages.txt* file. This file is crucial to pass because it configures the manager where to send the first packet and form there,
    the router to send the packet, etc.****!

 3) We usded code that we found on the web for implementing dijkstra's algorithm. This is okay to do because Subhojeet stated in recitatin that we are allowed to do this as long
    as we give attribution to where we got if from. We attribute this in the header file of the router.h and in this file.
    -> Code we used is from this website: https://rosettacode.org/wiki/Dijkstra%27s_algorithm#C

 4) Lastly, the packet* structure team dependent, meaning that as stated in recitation, we are allowed to pick the packet structure that works best for our team.
    As described below is how our team decided to structure our packets:


***************************************************************************************************************************************************************************
																				          *
::Our Packet* Structure::                                                                                                                                            	  *
Our Team decided on three packet structures, reason being is for simplicity, elegance, and added functionality to the assignment. They are defined as follows:		  *
~As stated by Subhojeet, we can follow the defined packet structure given to us in recitation, or we may use a packet structure that suits our team the best. 		  *
    																					  *
 This is defined below:        	       	   	   	  	   	       	  	      	    	      	     	       	    	      	       	                  *
																					  *
::Packet Structure 1::																			  *
																					  *
typedef struct _LSP { <-- This packet defines the LSP (Link State Packet) for each router.										  *
   int source;																				  *
   int destination; 																			  *
   int cost;																				  *
} LSP; 																					  *
																					  *
																					  *
																					  *
::Packet Structure 2::																			  *
																					  *
typedef struct _ROUTER_INFO { <-- Packet to encapsulate router information												  *
 int number_incoming_connections; 	    		       														  *
 int number_nodes;																			  *
 int number_edges; 																			  *
} ROUTER_INFO;																				  *
  																					  *
																					  *
																					  *
::Packet Structure 3::																			  *
																					  *
typedef struct _PACKET { <-- Packet that network sends to one another, router to router, manager to router etc...							  *
 int src;																				  *
 int dest;																				  *
 time_t timestamp; 																			  *
} Packet;																				  *
																					  *
***************************************************************************************************************************************************************************




::Implementation of Our Project::
   **-->::Current Status of our Code::<--**
 ~ Right off the bat, I just want to state that our program is 100% functinal and it works. There are no problems. It compiles perfectly and the directions for compilation are stated
   below.
 ~ If grader wanted to look at function information, etc, then looking in the source code would be most beneficial.

::Structure of Assignment:: 
~This program is utilizing socket programming to allow for end to end communication. We are using the server* / client* communication model.
 Addtionally, we are also using packets and many of the concepts that are associated with packets such as fragmentation and reassembly.
 Additionally, for this assignment, we are simulating a routing algorithm that we discussed in class. More, specifically, a link state routing protocol (OSPF).

::The process of Compiling and Invoking Our Program::
- To compile and invoke our program are as follows:

  1) To compile our program, all the user has to do is run 'make / make all' which will compile (as well as clean of the project directory by default) and make the manager and router programs.
     - Typing "make manager" or "make ss" makes just the specified module.
     - Typing "make clean" should clean up all object and executable files as well as automatically clean the directory.
     - Lastly, if the user wanted to clean the old project and then compile in one command, the user would type in
      'make clean all'.
      
  2) There are 3 ways of invoking my program. They are as follows:
	   
	   *manager is invoked as follows:*
	   - The manager takes as argument, a single file.
	   Example: manager <input_file> <messages.txt>
	   * input_file -> The file that contains the network topology description of the network. The format is as follows:
	   - The first line of this table contains a single integer N. This means that there are N nodes in the network, whose addresses are 0, 1, 2, ..., N-1.
	   - This line is followed by several lines, one for each point-to-point link in the network. Each line has 3 integers separated by whitespace <X, Y, C>.
	     X and Y are node numbers between 0 and N-1, and C is a positive integer that represents the cost of a link between X and Y.
	   - A -1, which indicates the end of the list of links in the input file
	   - Here is an example of suche a file:
	   -> 10 
	      0 9 40 
	      0 4 60 
	      0 2 40 
	      1 8 70 
	      2 6 70 
	      3 9 20 
	      3 8 70 
	      3 5 70 
	      4 7 70 
	      4 6 40 
	      5 9 70 
	      5 7 40 
	      7 9 60 
	      8 9 70
	      -1
	      
	  - messages.txt -> This file is crucial to pass because it configures the manager where to send the first packet and form there, the router to send the packet, etc.

	   * router( child processes* ) are invoked as follows:*
	   - The router processes aren't (in a scene) explicitly invoked. They are invoked by the manager process that spawns them.
	   * From the manager, they are invoked as follows:
	   -> execl("./router","router", std::to_string(i).c_str(), NULL); <-- This invokes router and passes it a unique id, to differentiate between all other routers.


::Questions I had for the TA::
 - Being that we already knew how to program using the Socket API, we didn't have as mush questions as last project. With that said, there were a few which are defined below:

Q: How do you want the packet structure to be?
A: *Answered during recitation on October 21st* (short answer: up to your group).

And that's pretty much it!
Again, I want to state that this was an awesome assignment and we learned a lot while completing it.
Thanks!

Best,
Tyrus Malmstrom, Sam Kramer, Torry Brelsford.



FOOTNOTE: * indicates multiplicity.
