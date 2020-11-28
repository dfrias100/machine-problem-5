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

static int nthreads = 0;

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

//void handle_process_loop(NetworkRequestChannel & _channel);

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
/* LOCAL FUNCTIONS -- THREAD FUNCTIONS */
/*--------------------------------------------------------------------------*/

/*void * handle_data_requests(void * args) {

  NetworkRequestChannel * data_channel =  (NetworkRequestChannel*)args;

  // -- Handle client requests on this channel. 
  
  handle_process_loop(*data_channel);

  // -- Client has quit. We remove channel.
 
  delete data_channel;

  return nullptr; // keep compiler happy
}*/

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- INDIVIDUAL REQUESTS */
/*--------------------------------------------------------------------------*/

/*void process_hello(NetworkRequestChannel & _channel, const std::string & _request) {
  _channel.cwrite("hello to you too");
}

void process_data(NetworkRequestChannel & _channel, const std::string &  _request) {
  std::string data = generate_data();
  _channel.cwrite(data);
}

void process_newthread(NetworkRequestChannel & _channel, const std::string & _request) {
  int error;
  nthreads ++;

  // -- Name new data channel

  std::string new_channel_name = "data" + int2string(nthreads) + "_";
 
  // -- Pass new channel name back to client

  _channel.cwrite(new_channel_name);

  // -- Construct new data channel (pointer to be passed to thread function)
  
  NetworkRequestChannel * data_channel = new RequestChannel(new_channel_name, RequestChannel::Side::SERVER);

  // -- Create new thread to handle request channel

  pthread_t thread_id;
  //  std::cout << "starting new thread " << nthreads << std::endl;
  if ((error = pthread_create(& thread_id, nullptr, handle_data_requests, data_channel))) {
    fprintf(stderr, "p_create failed: %s\n", strerror(error));
  }  

}*/

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- THE PROCESS REQUEST LOOP */
/*--------------------------------------------------------------------------*/

/*void process_request(NetworkRequestChannel & _channel, const std::string & _request) {

  if (_request.compare(0, 5, "hello") == 0) {
    process_hello(_channel, _request);
  }
  else if (_request.compare(0, 4, "data") == 0) {
    process_data(_channel, _request);
  }
  else if (_request.compare(0, 9, "newthread") == 0) {
    process_newthread(_channel, _request);
  }
  else {
    _channel.cwrite("unknown request");
  }

}

void handle_process_loop(NetworkRequestChannel & _channel) {

  for(;;) {

    std::cout << "Reading next request from channel (" << _channel.name()
	      << ") ..." << std::flush;
    std::string request = _channel.cread();
    std::cout << " done (" << _channel.name() << ")." << std::endl;
    std::cout << "New request is " << request << std::endl;

    if (request.compare("quit") == 0) {
      _channel.cwrite("bye");
      usleep(10000);          // give the other end a bit of time.
      break;                  // break out of the loop;
    }

    process_request(_channel, request);
  }
  
}*/

void connection_handler(int fd) {
  for(;;) {
    std::cout << "Reading next request from channel..." << std::flush;
    char buf[255];
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

void* connection_handler_wrapper(void* args) {
  connection_handler(*(int *)args);
  close(*(int *)args);
  delete args;
  return nullptr;
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

  //  std::cout << "Establishing control channel... " << std::flush;
  NetworkRequestChannel control_channel = NetworkRequestChannel(port_num, &connection_handler_wrapper, backlog);
  //  std::cout << "done.\n" << std::endl;

  //handle_process_loop(control_channel);

}

