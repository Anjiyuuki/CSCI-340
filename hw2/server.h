// -----------------------------------
// CSCI 340 - Operating Systems
// Spring 2023
// server.h header file
// Assignment 2
// Angie Bui
// -----------------------------------

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h> 
#include <sys/socket.h>

#include <netinet/in.h>

#define OK 0
#define FAIL -1
#define DEBUG 1

typedef struct payload_data {
  int pairs;
  int length;
  char** keys;
  char** vals;
} payload_data_t;

// ------------------------------------
// Function prototype that creates a socket and 
// then binds it to the specified port_number 
// for incoming client connections
//
// Arguments:	port_number = port number server socket will be bound to.
//
// Return:      Socket file descriptor (or -1 on failure)
//
int bind_port(unsigned int port_number);

// ------------------------------------
// Function prototype that accepts a client
// socket connection
//
// Arguments:	server file descriptor
//
// Return:      Termination status of client (0 = No Errors, -1 = Error)
//
int accept_client(int server_socket_fd);
