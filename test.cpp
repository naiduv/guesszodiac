#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

using namespace std;

int main( int argc, char *argv[] )
{
    socklen_t sockfd, newsockfd, portno, clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
	return(0);
    }
    /* Initialize socket structure */
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    portno = 5001;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
 
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         return(0);
    }

    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
   int count = 0;
   while(count<1) {
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     /* Accept actual connection from the client */
     newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, 
                                &clilen);

     if (newsockfd < 0) 
       {
	 perror("ERROR on accept");
	 return(0);
       }

     /* If connection is established then start communicating */
     memset(buffer, 0, sizeof(buffer));
     n = recv( newsockfd,buffer,sizeof(buffer),MSG_PEEK);
     if (n < 0){
	 perror("ERROR reading from socket");
	 return(0);
     }
     
     char comment[] ="<commentstr>"; 
     char *ptr = strstr(buffer,comment);
     if(ptr)
       cout<<"\nNew Comment: "<<ptr<<"\n";
     
     
     /* Write a response to the client */
     n = write(newsockfd,"msg received",12);
     if (n < 0) {
	 perror("ERROR writing to socket");
         return(0);
       }

     close(newsockfd);
    }
    return 0; 
}
