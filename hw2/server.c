// -----------------------------------
// CSCI 340 - Operating Systems
// Spring 2023
// server.c source file
// Assignment 2
//Angie Bui
// -----------------------------------
#include "server.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
// ------------------------------------
// Parses key/value pair from specified string, and sets it to `payload_data_t` argument.
// Arguments: 
// pair = pointer to string which contains key/value pair to be parsed.
// data = pointer to `payload_data_t` variable for which key/value pair will be set after parsing.
void http_request_parse_pair(const char* pair, payload_data_t* data)
{
 // keep track of length of key and value 
  int keylen = 0;
  int vallen = 0;
  // arrays of characters used to store the key and value strings
  // Arbitrary lengths 
  // initializes elements to 0
  char key[128] = {0};
  char val[512] = {0};
  int pairlen = (int)strlen(pair); //takes a string as an argument and returns its length
  
  // If this is the first pair being parsed, allocate memory for the keys and values arrays
  // if the keys and vals arrays in the data struct are NULL, allocate memory for 
  //these arrays to store the key-value pairs that will be parsed by the http_request_parse_pair() 
  //function.
  if (data->keys == NULL && data->vals == NULL) {
    data->keys = (char**)calloc(data->pairs, sizeof(char*));
    data->vals = (char**)calloc(data->pairs, sizeof(char*));
  }
  for (int i = 0; i < pairlen && pair[i] != '='; i++) {
    key[keylen++] = pair[i];
  }
  for (int i = keylen+1; i < pairlen; i++) {
    val[vallen++] = pair[i];
  }
  data->keys[data->length] = (char*)calloc(1, keylen+1);
  data->vals[data->length] = (char*)calloc(1, vallen+1);
  strcpy(data->keys[data->length], key);
  strcpy(data->vals[data->length], val);
  data->length++;
}

// ------------------------------------
// Fills key/value pairs for `payload_data_t` argument.
// Arguments:	payload = pointer to string which contains payload to be parsed.
//            data    = pointer to `payload_data_t` variable for which key/value pairs will be filled.
void http_request_parse_payload(char* payload, payload_data_t* data) 
{
  int payloadlen = (int)strlen(payload); //get length of 'payload string'
  for (int i = 0; i < payloadlen; i++) { //loop through characters
    data->pairs += (int)(payload[i] == '&'); // Increment the number of pairs in the payload
	//if an '&' is encountered.
  }
  if (data->pairs > 0) { //if at least 1 
    char* pair = strtok(payload, "&"); //split into pairs
    while (pair != NULL) {
      http_request_parse_pair(pair, data); //parse each pair
      pair = strtok(NULL, "&"); //next 
    }
  }
}

// ------------------------------------
// Grabs payload from GET request.
// Arguments:	route = pointer to string which contains route from retrieved GET request.
// Returns: payload data (key/value pairs) parsed from request.
payload_data_t http_get_request_get_payload(char* route) 
{
  int pathlen = 0; //length of path
  int routelen = strlen(route); //len route string
  payload_data_t data = {0};  // new struct 
  //loop to iterate over the characters in the route string until a query string delimiter is found.
  for (int i = 0; i < routelen; i++) {
    if (route[i] == '?') {  // if ? is found
      data.pairs += 1; //Increment the number of key-value pairs in the query string.
      break;
    }
    pathlen++;
  }
  route += pathlen+1; //update route pointer skip over path and query string delimiter
  http_request_parse_payload(route, &data);
  return data;  //returns data sturct
}

// ------------------------------------
// Grabs payload from POST request.
// Arguments:	request = pointer to string which contains POST request.
// Returns: payload data (key/value pairs) parsed from request.
payload_data_t http_post_request_get_payload(char* request) 
{
  char payload[256] = {0};
  payload_data_t data = { //new payload_data_t struct with one key-value pair.
    .pairs = 1
  };
  char* line = strtok(request, "\r\n"); //by lines
  while (line != NULL) { // if still lines
    memset(payload, 0, sizeof payload); //clear buffer
    strcpy(payload, line); //copy current line to payload buffer
    line = strtok(NULL, "\r\n"); // next line
  }
  http_request_parse_payload(payload, &data); // Parse the payload string and populate the data struct.
  return data; // Return the data struct
}

// ------------------------------------
// Builds contents for GET request.
// Arguments:	route    = pointer to string which contains route from retrieved GET request.
//            contents = pointer to string which contains route 
//                       from retrieved GET request header.
void http_set_get_response_contents(char* route, char* contents) 
{
  char tempbuf[256] = {0};
   // If the requested route is "/SimplePost.html", read the file contents into the buffer
  if (strcmp(route, "/SimplePost.html") == 0) {
    int fd = open("./SimplePost.html", O_RDONLY);
    if (fd < 0) {
		//error library
      perror("Cannot open ./SimplePost.html"), exit(EXIT_FAILURE);
    }
    if (read(fd, contents, 4096) == -1) {
		//error library
      perror("Cannot read ./SimplePost.html"), exit(EXIT_FAILURE);
    }
    close(fd);
  }
  else {
	  //construct data
    payload_data_t data = http_get_request_get_payload(route);
    strcat(contents,
      "\r\n<html>"
          "<body>"
            "<h2>GET Operation</h2>"
            "<table border=1 width=\"50%\">"
              "<tr>"
                "<th>Key</th>"
                "<th>Value</th>"
              "</tr>"
    );
	// Add a row to the table for each key-value pair in the payload data
    for (int i = 0; i < data.pairs; i++) {
      sprintf(tempbuf,             
        "<tr>"
          "<th>%s</th>"
          "<th>%s</th>"
        "</tr>",
        data.keys[i],
        data.vals[i]
      );
      strcat(contents, tempbuf);
      memset(tempbuf, 0, sizeof tempbuf);
      free(data.keys[i]);
      free(data.vals[i]);
    }
    if (data.pairs != 0) {
      free(data.keys);
      free(data.vals);
    }
    strcat(contents,
            "</table>"
        "</body>"
      "</html>"
    );
  }
}

// ------------------------------------
// Builds contents for POST request.
// Arguments:	request  = pointer to POST request data.
//            contents = pointer to array which will be filled with contents.
void http_set_post_response_contents(char* request, char* contents) {
  char tempbuf[256] = {0};
  payload_data_t data = http_post_request_get_payload(request);
  strcat(contents,
    "\r\n<html>"
        "<body>"
          "<h1>POST Operation</h1>"
          "<table cellpadding=5 cellspacing=5 border=1>"
            "<tr>"
              "<th>Key</th>"
              "<th>Value</th>"
            "</tr>"
  );
  for (int i = 0; i < data.pairs; i++) {
    sprintf(tempbuf,             
      "<tr>"
        "<td><b>%s</b></td>"
        "<td>%s</td>"
      "</tr>",
      data.keys[i],
      data.vals[i]
    );
    strcat(contents, tempbuf);
    memset(tempbuf, 0, sizeof tempbuf);
    free(data.keys[i]);
    free(data.vals[i]);
  }
  if (data.pairs != 0) {
    free(data.keys);
    free(data.vals);
  }
  strcat(contents,
        "</table>"
      "</body>"
    "</html>"
  );
}

// ------------------------------------
// Sends a response to a GET request.
// Arguments:	sockfd = client's port number.
//            route  = pointer to string which contains route from retrieved GET request.
void http_get_response(int sockfd, char* route) 
{
  char tempbuf[128] = {0};
  char contentsbuf[4096] = {0};
  http_set_get_response_contents(route, contentsbuf);
  write(sockfd, "HTTP/1.1 200 OK\r\n", 17);
  write(sockfd, "Connection: close\r\n", 19);
  sprintf(tempbuf, "%s: %ld\r\n", 
    "Content-Length", strlen(contentsbuf));
  write(sockfd, tempbuf, strlen(tempbuf));
  write(sockfd, "Content-Type: text/html\r\n", 25);
  write(sockfd, contentsbuf, strlen(contentsbuf));
}

// ------------------------------------
// Sends a response to a POST request.
// Arguments:	sockfd  = client's port number.
//            request = pointer to data received from POST request.
void http_post_response(int sockfd, char* request) 
{
  char tempbuf[128] = {0};
  char contentsbuf[4096] = {0};
  http_set_post_response_contents(request, contentsbuf);
  write(sockfd, "HTTP/1.1 200 OK\r\n", 17);
  write(sockfd, "Connection: close\r\n", 19);
  sprintf(tempbuf, "%s: %ld\r\n", 
    "Content-Length", strlen(contentsbuf));
  write(sockfd, tempbuf, strlen(tempbuf));
  write(sockfd, "Content-Type: text/html\r\n", 25);
  write(sockfd, contentsbuf, strlen(contentsbuf));
}

// ------------------------------------
// Function which sends response to the client.
// Arguments:	sockfd = client's port number.
void http_server_respond(int sockfd) 
{// array initialized to 0
  char method[32] = {0};
  char route[512] = {0};
  char request[2048] = {0};
    // line reads data from the socket represented by sockfd and stores it in the request array
  if (read(sockfd, request, sizeof request) == -1) {
    perror("Cannot receive request"), exit(EXIT_FAILURE);
  }
    //use strcmp to compare the method string to "GET" and "POST"
  sscanf(request, "%s %s HTTP/1.1\r\n", method, route);
  if (strcmp(method, "GET") == 0) {
    http_get_response(sockfd, route);
  }
  if (strcmp(method, "POST") == 0) {
    http_post_response(sockfd, request);
  }
  write(sockfd, "\r\n", 2);//newline character to the socket
  close(sockfd);//close
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// Function prototype that creates a socket and 
// then binds it to the specified port_number 
// for incoming client connections
// 
// Arguments:	port_number = port number server socket will be bound to.
//
// Return:      Socket file descriptor (or -1 on failure)
//
int bind_port(unsigned int port_number)
{
  // -------------------------------------
  // NOTHING TODO HERE :)
  // -------------------------------------
  // DO NOT MODIFY !

  int socket_fd;
  int set_option = 1;

  struct sockaddr_in server_address;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set_option,
	     sizeof(set_option));

  if (socket_fd < 0) return FAIL;

  bzero((char *) &server_address, sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(port_number);

  if (bind(socket_fd, (struct sockaddr *) &server_address,
	     sizeof(server_address)) == 0) {
    return socket_fd;
  } else {
    return FAIL;
  }
} // end bind_port function

// ------------------------------------
// Function prototype that accepts a client
// socket connection
// 
// Arguments:	server file descriptor
//
// Return:      Termination status of client (0 = No Errors, -1 = Error)
//
int accept_client(int server_socket_fd)
{
  int client_socket_fd = -1;
  socklen_t client_length; 
  struct sockaddr_in client_address;

  client_length = sizeof(client_address);

  client_socket_fd = accept(server_socket_fd,
			     (struct sockaddr *) &client_address,
			     &client_length);
	

	
  // --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // TODO:
  // -------------------------------------
  // Modify code to fork a child process
  // -------------------------------------
  if (client_socket_fd < 0) {
    return FAIL; // If error occurred while accepting the client connection, return a FAIL status
  }
  pid_t pid = fork(); // fork a child process
  // If the current process is the child process, handle the HTTP request and exit with a success status
  if (pid == 0) {
    http_server_respond(client_socket_fd);
    exit(EXIT_SUCCESS);
  }
  // If the current process is the parent process, close the client socket file descriptor and return an OK status
  close(client_socket_fd);
  return OK;
} // end accept_client function
