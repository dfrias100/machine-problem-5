/* 
    File: dataserver.cpp

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 

    Dataserver main program for MPs in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "netreqchannel.hpp"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* VARIABLES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

void connection_handler(int fd);

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

std::string int2string(int number) {
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- GENERATE THE DATA */
/*--------------------------------------------------------------------------*/

std::string generate_data() {
  // Generate the data to be returned to the client.
  usleep(1000 + (rand() % 5000));
  return int2string(rand() % 100);
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- HANDLER WRAPPER */
/*--------------------------------------------------------------------------*/

void* connection_handler_wrapper(void* args) {
  connection_handler(*(int *)args);
  close(*(int *)args);
  delete args;
  return nullptr;
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- THE PROCESS REQUEST LOOP */
/*--------------------------------------------------------------------------*/

void connection_handler(int fd) {
  for(;;) {
    std::cout << "Reading next request from channel..." << std::flush;
    char buf[255]; // 255 is MAX_MESSAGE in netreqchannel.hpp
    read(fd, buf, 255);
    std::cout << "done." << std::endl;
    std::cout << "New request is " << buf << std::endl;

    std::string request = buf;

    if (request.compare("quit") == 0) {
      std::cout << "Writing " << "'bye'" << " to socket " << fd << std::endl;
      write(fd, "bye", 4);
      usleep(10000);
      break;
    }

    if (request.compare(0, 5, "hello") == 0) {\
      std::cout << "Writing " << "'hello to you too'" << " to socket " << fd << std::endl;
      write(fd, "hello to you too", 17);
    }

    if (request.compare(0, 4, "data") == 0) {
      std::string data = generate_data();
      std::cout << "Writing '" << data << "' to socket " << fd << std::endl;
      write(fd, data.c_str(), data.length()+1);
    }
  }
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
  unsigned short port_num = 0;
  int backlog = 0;
        
  int opt;

  while((opt = getopt(argc, argv, ":p:b:")) != -1) {
    switch (opt) {
      case 'p':
        sscanf(optarg, "%hu", &port_num);
        break;
      case 'b':
        sscanf(optarg, "%d", &backlog);
        break;
      case ':':
        std::cout << "Invalid parameters or no parameters passed. Check your input and start again." << std::endl;
        return 1;
      case '?':
        std::cout << "Unknown argument" << std::endl;
        return 1;
    }
  }

  if (port_num == 0 || backlog == 0) {
    std::cout << "Invalid parameters or no parameters passed. Check your input and start again." << std::endl;
    exit(1);
  } 

  NetworkRequestChannel control_channel = NetworkRequestChannel(port_num, &connection_handler_wrapper, backlog);
}

