/* A simple server in the internet domain using TCP
   The port number is passed as an argument
   This version runs forever, forking off a separate
   process for each connection
   gcc server2.c -lsocket
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

void playRound(int); /* function prototype */
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen, pid;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd,
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0)
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             playRound(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     return 0; /* we never get here */
}

/*************** playRound() **************
 For each message from the client, playRound()
 is called. This method receives the client's
 choice of S (Silent) or B (Betray). It makes
 a random choice as to what it will do. Then
 it returns the result of the round to the
 client.
 *****************************************/
void playRound (int sock)
{
   int n;
   char PrisA[256];

   // Server is reading what the client has submitted
   bzero(PrisA,256);
   n = read(sock,PrisA,255);

   // Server is choosing what it will be
   char* PrisB;
   srand ( time(NULL) ); // seeding rand so it is not always the same :)
   int i= rand() % 2;

   if(i == 0) {
     PrisB = "S";
   } else {
     PrisB = "B";
   }
   printf("Prisoner A Chose: %s", PrisA);
   printf("Prisoner B Chose: %s\n", PrisB);

   // Server is determining the number of years
   char* answer;
   if(strcmp(PrisA,"S\n") == 0) { // Prisoner A chose S
     if(strcmp(PrisB,"S") == 0) { // Prisoner B chose S
       answer = "Prisoner A: 1 year\nPrisoner B: 1 year\n";
     } else { // Prisoner B chose B
       answer = "Prisoner A: 3 years\nPrisoner B: You are Free!\n";
     }
   } else { // Prisoner A chose B
     if(strcmp(PrisB,"S") == 0) { // Prisoner B chose S
       answer = "Prisoner A: You are Free!\nPrisoner B: 3 years\n";
     } else { // Prisoner B chose B
       answer = "Prisoner A: 2 years\nPrisoner B: 2 years\n";
     }
   }

   // Server is sending the results to the client
   char* fancyHeader;
   char* fancyFooter;
   fancyHeader = "\n+=========================+\n+        Results!         +\n+=========================+\n";
   fancyFooter = "+=========================+\n";

   write(sock, fancyHeader, strlen(fancyHeader));
   n = write(sock, answer, strlen(answer));
   if (n < 0) error("ERROR writing to socket");
   write(sock, fancyFooter, strlen(fancyFooter));
}
