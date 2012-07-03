#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <fstream>

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
      cout<<"\nERROR opening socket";
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
      cout<<"\nERROR on binding";
      return(0);
    }

    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    while(1) {
     cout<<"\n***New listen cycle***\n";
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     /* Accept actual connection from the client */
     newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, 
                                &clilen);

     if (newsockfd < 0) 
       {
	 cout<<"\nERROR on accept";
	 return(0);
       }

     /* If connection is established then start communicating */
     memset(buffer, 0, sizeof(buffer));
     n = read(newsockfd,buffer,sizeof(buffer));
     if (n < 0){
         cout<<"\nERROR reading from socket";
	 return(0);
     }

     if(strlen(buffer)==0) {
       cout<<"\n buffer empty";
       continue;
     }

     char comment[] ="<commentstr>"; 
     char *ptr = strstr(buffer,comment);

     //no comment
     if(ptr) {
       cout<<"\nNew Comment: "<<ptr<<"\n";
     } else {
       cout<<"\n No Comment!!"<<"\n";
       continue;
     }

     //if there is only <commentstr></commentstr>
     if(strlen(ptr)<(sizeof(comment)*2+1)){
       cout<<"\n comment empty";
       continue;
     }

     //if there is "\n"
     if(strstr(ptr,"\n")){
       cout<<"\n newline char";
       continue;
     }


     fstream myfile;
     myfile.open ("comments.json");
     myfile.seekp(-1, ios::end);
     myfile<<",\""<<ptr<<"\"]";
     myfile.close();
 
     /* Write a response to the client */
     n = write(newsockfd,"msg was received",18);
     if (n < 0) {
         cout<<"\nERROR writing to socket";
         return(0);
       }

     close(newsockfd);
     cout<<"\n closing socket \n";
    }

    return 0; 
}
