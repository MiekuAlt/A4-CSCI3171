/************************ client ************************
By: Michael Altair & Chaoran Zhou

This code is derived off of tutorial code provided by
http://www.linuxhowtos.org/C_C++/socket.htm it was then
modified in order to fulfill the requirements of this
assignment.
 ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Prototypes
void showIntro();
void playRound(int sock);

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
    showIntro(); // Displays a little intro for the user
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
				playRound(sockfd);
			}
			else close(sockfd);
		} /* end of while; */
    return 0;
}

/*************** playRound() **************
 This method prompts the user as to what
 their decision is to be, if they will
 Betray or remain Silent. It then sends
 their response to the server
 *****************************************/
void playRound (int sock)
{
   int n;
   char buffer[256];
   printf("\nIs your decision to remain Silent (S) or Betray (B)? ");
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
		   if (buffer[0]=='Y' || buffer[0]=='y')
			   exit(0);
		   if (buffer[0]=='N' || buffer[0]=='n')
			   break;
		   else
			   printf("Invalid response, please enter Y or N\n");
  		}
  		else
  			printf("Invalid response, please enter S or B.\n");
      }
}

/*************** showIntro() **************
Displays an attractive little introduction
for the user.
 *****************************************/
void showIntro() {
  char* fancyHeader;
  char* seperator;
  char* signature;
  char* introMsg;
  fancyHeader = "\n+=========================+\n+   Prisoner's Dilemma    +\n+=========================+\n";
  seperator = "+=========================+\n";
  signature = "+  by: Michael & Chaoran  +\n";
  introMsg =  "+   You (Prisoner A) and  +\n+Prisoner B are being held+\n+   by police! You must   +\n+ decide if you are going +\n+ to remain Silent (S) or +\n+ Betray (B) Prisoner B!  +\n";
  printf(fancyHeader);
  printf(signature);
  printf(seperator);
  printf(introMsg);
  printf(seperator);
}
