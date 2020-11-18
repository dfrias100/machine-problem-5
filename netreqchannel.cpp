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

}

NetworkRequestChannel::NetworkRequestChannel(const unsigned short _port_no, void * (*connection_handler) (int *)) {

}

NetworkRequestChannel::~NetworkRequestChannel() {

}

string NetworkRequestChannel::send_request(string _request) {
    return "message";
}

string NetworkRequestChannel::cread() {
    return "message";
}

int NetworkRequestChannel::cwrite(string _msg) {
    return 0;
}