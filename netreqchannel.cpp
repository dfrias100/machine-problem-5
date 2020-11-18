/* 
    File: netreqchannel.cpp

    Author: Daniel Frias
            Department of Computer Science
            Texas A&M University
    Date  : 2020/11/16

    Source code for the NetReqChannel class.
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "netreqchannel.hpp"

/*--------------------------------------------------------------------------*/
/* NAME SPACES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS P C B u f f e r */
/*--------------------------------------------------------------------------*/

NetworkRequestChannel::NetworkRequestChannel(const string _server_host_name, const unsigned short _port_no) {
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;

    sin.sin_port = htons(_port_no);

    if (struct hostent * phe = gethostbyname(_server_host_name.c_str()))
        memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    else if ((sin.sin_addr.s_addr = inet_addr(_server_host_name.c_str())) == INADDR_NONE) {
        cerr << "can't get " << _server_host_name << " host entry\n";
        exit(1);
    }
    
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { 
        cerr << "can't create socket: " << s << strerror(errno);
        exit(1);
    }

    if (connect(s, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        cerr << "can't connect to server." << endl;
        exit(1);
    }

    fd = s;
    my_side = Side::CLIENT;
}

NetworkRequestChannel::NetworkRequestChannel(const unsigned short _port_no, void * (*connection_handler) (int *)) {

}

NetworkRequestChannel::~NetworkRequestChannel() {

}

string NetworkRequestChannel::send_request(string _request) {
    cwrite(_request);
    string s = cread();
    return s;
}

string NetworkRequestChannel::cread() {
    char buf[MAX_MESSAGE];

    if (read(fd, buf, MAX_MESSAGE) < 0) {
        perror(string("Request Channnel (" + to_string(this->fd) + "): Error reading from socket!").c_str());
    }

    std::string s = buf;

    cout << "Request Channel (" << this->fd << ") reads [" << buf << "]" << endl;

    return s;
}

int NetworkRequestChannel::cwrite(string _msg) {
    if (_msg.length() >= MAX_MESSAGE) {
        cerr << "Message too long for channel!" << endl;
    }

    cout << "Request Channel (" << this->fd << ") writing [" << _msg << "]";

    const char* s = _msg.c_str();

    if (write(fd, s, strlen(s)+1) < 0) {
        perror(string("Request Channel (" + to_string(this->fd) + ") : Error writing to socket!").c_str());
    }

    cout << "(" << this->fd << ") done writing." << endl;
    return 0;
}