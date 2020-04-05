/*----------------------------------------------------------------
| COIS 3380 - Systems Programming 
| Lab 6 - Client Server Processes 
|  Option: C - Sockets and Client-Server
|
| Filename: client.c
| PWD: home/nikhilpaiganesh/COIS-3380/lab/lab6/client.c
| Written by : Nikhil Pai Ganesh 
| Student Number: 0595517
| 
| Purpose: Client in Client-Server Process, recieves file
| Usage: compile: gcc -o client client.c
|        run:   ./client [host] [port]
| 
| Example Usage: compile: gcc-o client client.c
|                run:    ./client 127.0.0.1 55517
----------------------------------------------------------------*/

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <ctype.h>

void error(const char *err_msg) // Error Message Method
{
    perror(err_msg); // Print error Message
    exit(0); //Exit the Program
}

int main(int argc, char *argv[]) // Main with Command line args 
{
    //Variables 
    int fd; //File Descriptor
    int port; //Port Number 
    int n; //Testing Variable
    char buffer[1024]; //Buffer to send message (I'm storing message in a buffer and then sending it in a data stream)

    struct sockaddr_in serv_addr; //Stucture for server address 
    struct hostent *serv; //store host info

    if (argc < 3){ //If filename is wrong 
       fprintf(stderr,"Usage ERROR\n"); //Usage Error
       exit(0); //Kill the client 
    }
    
    port = atoi(argv[2]); //Set Port 

    fd = socket(AF_INET, SOCK_STREAM, 0); //Creating Socket 
    if (fd < 0){ //If Socket failed
        error("Socket ERROR\n"); //Error Message 
    }

    serv = gethostbyname(argv[1]); //Setting host
    if (serv == NULL) { //If host is wrong 
        fprintf(stderr,"host ERROR (use 127.0.0.1 for local)\n"); //Error Message
        exit(0); //Kill Client 
    }

    bzero((char *) &serv_addr, sizeof(serv_addr)); //Clear the size of server address
    serv_addr.sin_family = AF_INET; //Seting the address family 

    bcopy((char *)serv->h_addr,(char *)&serv_addr.sin_addr.s_addr,serv->h_length); //Server properties copy
    serv_addr.sin_port = htons(port); ////Setting the address port number and converting the short integer of host byte order to network byte order 


    if (connect(fd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ //Connect to Server error
        error("Connection to server ERROR\n"); //Error Message
    }
        bzero(buffer,1024);//Clear Buffer


        FILE *fp; //File Pointer
         int ch = 0; //Contents of the file variable
            fp = fopen("lab_sourcefile_local_clone","a"); //If no such file exists , create one and append data         
            
        int words; //variable to count words 
	    
        read(fd, &words, sizeof(int)); //Read the words from the clients 
        
        while(ch != words){ //loop for words number of words 
        	 read(fd, buffer, 1024); //read the file 
	   	 fprintf(fp, " %s", buffer);   //print the file 
		 ch++; //increment the word 
	   }
     	printf("----------------FILE RECIEVED SUCESSFULLY--------------\n"); //Sucess Message
	   printf("--- file name: lab_sourcefile_local_clone ---\n"); //Sucess Message
    
    close(fd); //Kill Client 
    return 0;
}