/*---------------------------------------------------------------------------
| COIS 3380 - Systems Programming 
| Lab 6 - Client Server Processes 
|  Option: C - Sockets and Client-Server
|
| Filename: server.c
| PWD: home/nikhilpaiganesh/COIS-3380/lab/lab6/server.c
| Written by : Nikhil Pai Ganesh 
| Student Number: 0595517
| 
| Purpose: Server in Client-Server Process, copies file and sends to client
| Usage: compile: gcc -o server server.c
|        run:   ./server [port] [file to send]
| 
| Example Usage: compile: gcc-o server server.c
|                run:    ./server 55517 /home/COIS/3380/lab4_sourcefile
----------------------------------------------------------------------------*/

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>


void error(const char *err_msg) // Error Message Method
{
    perror(err_msg); // Print error Message 
    exit(1); //Exit the Program
}

int main(int argc, char *argv[]) // Main with Command line args 
{
     //Variables 
     int fd; //File Descriptor
     int newfd; // New File Descriptor 
     int port; //Port Number 
     int n; //Testing Variable 
     char buffer[1024];  //Buffer to send message (I'm storing message in a buffer and then sending it in a data stream)

     socklen_t clilen; //length and size value for socket address
     
     struct sockaddr_in serv_addr, cli_addr; //Stucture for server and client address 
     
     if (argc < 2) { //If Port Number not Provided in the command line
         fprintf(stderr,"Port ERROR\n"); // Print Error Message 
         exit(1); //Program Exited 
     }


     fd = socket(AF_INET, SOCK_STREAM, 0);  //Creating Socket
     if (fd < 0){ //If Socket failed
        error("Socket ERROR\n"); //Error Message
     }

     bzero((char *) &serv_addr, sizeof(serv_addr)); //Clear the size of server address
     port = atoi(argv[1]); //Set Port

     serv_addr.sin_family = AF_INET; //Seting the address family
     serv_addr.sin_addr.s_addr = INADDR_ANY; //setting the IP Address of the socket
     serv_addr.sin_port = htons(port);  //Setting the address port number and converting the short integer of host byte order to network byte order 

     if (bind(fd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) { //Binding to Server 
              error("Binding ERROR\n"); //Error Message 
     }

     listen(fd,1); //Listen to any client connections  1= Number of clients at a time 

     clilen = sizeof(cli_addr); //get client address

     newfd = accept(fd, (struct sockaddr *) &cli_addr, &clilen);// Accept connection

     if (newfd < 0){ //If connection fails 
          error("Connection ERROR"); //Error Message
     }

    FILE *f; // file pointer
    int words = 0; //Calculating the words with this variable
    char c; 

    f=fopen(argv[2],"r"); //Open the file given (ReadOnly)
    while((c=getc(f))!=EOF)	//Counting # of words in the file
	{	
		fscanf(f , "%s" , buffer); //traverse the file word by word 
		if(isspace(c)||c=='\t'){ //recognizing space or tab
		words++; // increment the word
        }
	}

	write(newfd, &words, sizeof(int)); //writing the word count 
    rewind(f); //moving the file pointer to the first location

    char ch ; //Contents of the file variable 
       while(ch != EOF) //Contents of the file (EOF = End of File)
      {		
		fscanf(f , "%s" , buffer); //Scan the words 
		write(newfd, buffer, 1024); //Write the words
		ch = fgetc(f); //traversing each character 
      }

	printf("file %s send SUCCESS\n", argv[2]); //Sucess Message 
    
     close(newfd); //Kill Server 
     close(fd); //Kill Sever 
     return 0; 
}