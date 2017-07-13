#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
	     while (1) {			
			if (sockfd < 0) 
				error("ERROR on accept");			
			if (sockfd > 0)  {
				/*close(sockfd);*/
				dostuff(sockfd);
				/*exit(0);*/
			}
			else close(sockfd);
		} /* end of while; */
    return 0;
}

void dostuff (int sock)
{
   int n;
   char buffer[256];
   printf("Are you decide to remain Silent (S) or Betray (B)? "); 
   bzero(buffer,256);
   fgets(buffer,255,stdin);
   n = write(sock,buffer,strlen(buffer));
   if (n < 0) error("ERROR writing to socket");
   bzero(buffer,256);
   n = read(sock,buffer,255);  
   if (n < 0) error("ERROR reading to socket");
   printf("%s\n",buffer);
   while(1){
	   printf("Would you like to quit? Yes (Y) or No (N).\n");
	   bzero(buffer,256);
	   fgets(buffer,255,stdin);
	   if (strlen(buffer)==2 ){
		   if (buffer[0]=='Y')
			   exit(0);
		   if (buffer[0]=='N')
			   break;
		   else
			   printf("Invalid response.\n");
		   
		}
		else
			printf("Invalid response.\n");
	}
}