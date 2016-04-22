#include "def.h"
#include <sys/utsname.h>
#define MAXHOSTNAME 80
#define BUFSIZE 1024
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#define NULL1 '\0'
#define SA      struct sockaddr
void exit(int);
char buf[BUFSIZE],bufferip[BUFSIZE],bufferport[BUFSIZE];
char rbuf[BUFSIZE];
int cport;
  int rc, cc;
  int   sd;
  pthread_t	tid1;
void sendMessage(int inSock, int outSock);
void *GetUserInput();
void getMessage(int inSock, int outSock);
struct sockaddr_in client;
struct  hostent *hp, *gethostbyname();

int UDPsocket;

int UDPport;
char *MulticastIPAddress;

u_char TimeToLive;
u_char loop;


main( argc, argv )
int argc;
char *argv[];
{
  int childpid;
  struct   sockaddr_in server;
  struct   sockaddr_in client;
  struct  hostent *hp, *gethostbyname();
  char *substr;
  struct  servent *sp;
  struct sockaddr_in from;
  struct sockaddr_in addr;
  struct sockaddr_in LocalHost;
  int port;
  int fromlen;
  int length;
  char ThisHost[80];
  int allHosts;
  char *host[]={"something", "somethingmore", "somethingelse","atria", "sirius"};

	sp = getservbyname("echo", "tcp");
	gethostname(ThisHost, MAXHOSTNAME);
	//printf("Chat Server Name: %s\n", ThisHost);
        if  ( (hp = gethostbyname(ThisHost)) == NULL )
        {
        	fprintf(stderr, "Can't find host %s\n");
                exit(-1);
        }
    bcopy ( hp->h_addr, &(server.sin_addr), hp->h_length);
    //printf("Chat Server IP: %s \n", inet_ntoa(server.sin_addr));
	//sd = socket (AF_INET,SOCK_STREAM,0);	
        if (sd<0)
        {
               perror("opening stream socket");
               exit(-1);
        }

		allHosts=0;
		while(allHosts < sizeof(host))
		{
			sd = socket (AF_INET,SOCK_STREAM,0);
			server.sin_family = AF_INET;
			hp = gethostbyname(host[allHosts]);
            bcopy ( hp->h_addr, &(server.sin_addr.s_addr), hp->h_length);
			server.sin_port = htons(10253);

			if ( connect(sd, (SA *) &server, sizeof(server)) < 0 )
                  {
					printf("\nTry connecting to: %s ...   connecting to Server at '%s'  failed\n",host[allHosts],host[allHosts]);
					close(sd);
                  }
			else
				{	
				printf("\nTry connecting to: %s ...   connecting to Server at '%s'  succesfull\n ",host[allHosts],host[allHosts]);  
				break;
				}
		allHosts++;
		
		}
		
				printf("Chat Server Host Name: %s\n", ThisHost);
				printf("Chat Server IP address: %s \n", inet_ntoa(server.sin_addr));
                int len=sizeof(client);
                if(getsockname(sd, (SA *)&client,&len)==-1)
		{
	
			perror("Couldn't get sockname");
			exit(1);
		}
		else
                        {
                                cport=ntohs(client.sin_port);
                        }
/*
 childpid = fork();
 if (childpid == 0) {
   GetUserInput();
 }
 */
	pthread_create(&tid1, NULL, &GetUserInput, NULL);


/** get data from USER, send it SERVER,
      receive it from SERVER, display it back to USER  */

  for(;;) {
    cleanup(rbuf);
    if( (rc=recv(sd, rbuf, sizeof(buf), 0)) < 0){
      perror("receiving stream  message");
      exit(-1);
    }
    if (rc > 0){
      rbuf[rc]=NULL;
      printf("%s\n", rbuf);
    }else {
      printf("Disconnected..\n");
      close (sd);
      exit(0);
    }


  }
}
cleanup(buf)
char *buf;
{
  int i;
  for(i=0; i<BUFSIZE; i++) buf[i]=NULL;
}

void *GetUserInput()
{
	 char sendBuf[MAX_LEN];
	char data[MAX_LEN];
	struct utsname  myname;
	if (uname(&myname) < 0)
                perror("uname_error");

for(;;) {
    printf("\nType anything followed by RETURN, or type CTRL-D to exit\n");
    cleanup(buf);
	memset(sendBuf, '\0', MAX_LEN);
                memset(data, '\0', MAX_LEN);
    rc=read(0,buf, sizeof(buf));
	 snprintf(sendBuf, sizeof(sendBuf), "(%s.cs.odu.edu): %s\n", myname.nodename, buf);
    if (rc == 0) 
	{
		//kill(getppid(), SIGINT);
		close(sd);
		exit (0);
	}
    if (send(sd, sendBuf, sizeof(sendBuf), 0) <0 )
      perror("sending stream message");
  }
  //printf ("EOF... exit\n");
  //close(sd);
  
}
