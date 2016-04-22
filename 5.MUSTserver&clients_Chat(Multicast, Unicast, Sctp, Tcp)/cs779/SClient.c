
#include "def.h"
#include <sys/utsname.h>
#include <pthread.h>
#define MAXHOSTNAME 80
#define BUFSIZE 1024
char buf[BUFSIZE];
char rbuf[BUFSIZE];
void *GetUserInput(void*);
  int rc, cc;
  int   sd;
const char *server_machines[5]={"something","somethingelse","somethingmore","atria", "sirius"};

main( argc, argv )
int argc;
char *argv[];

{
   int childthr;
  struct   sockaddr_in server;
  struct   sockaddr_in client;
  struct  hostent *hp, *gethostbyname();
  struct sockaddr_in from;
  struct sockaddr_in addr;
  int fromlen;
  int length;
  char ThisHost[80];
  int i,err;     //counter for server machine loop and err for thread creation
  int pn=10252;
  char port[100];
  pthread_t	tid;


  /* Construct name of socket to send to. */
    server.sin_family = AF_INET; 
  /* OR server.sin_family = hp->h_addrtype; */
	if (argc == 2) {
        sprintf(port,"%s", argv[1]);
        pn = atoi(port);
		}
  server.sin_port = htons(pn);
  /*OR    server.sin_port = sp->s_port; */

  /*   Create socket on which to send  and receive */

  sd = socket (AF_INET,SOCK_STREAM,IPPROTO_SCTP); 
  /* sd = socket (hp->h_addrtype,SOCK_STREAM,0); */
  
  if (sd<0) {
    perror("opening stream socket");
    exit(-1);
  }

    /** get MGServer Host information*/

	
	for(i=0;i<sizeof(server_machines);i++)
	{
		hp = gethostbyname(server_machines[i]);
		bcopy ( hp->h_addr, &(server.sin_addr), hp->h_length); 

		if ( connect(sd, (SA *) &server, sizeof(server)) == 0 )
			break;
	}

	if(i==sizeof(server_machines))
	{
		close(sd);
		fprintf(stderr, "Can't find host \n");
		exit(0);
	}


  fromlen = sizeof(from);
  if (getpeername(sd,(SA *)&from,&fromlen)<0){
    perror("could't get peername\n");
    exit(1);
  }
  printf("Connected to MUST Server: ");
  printf("%s:%d\n", inet_ntoa(from.sin_addr),
      ntohs(from.sin_port));
  if ((hp = gethostbyaddr((char *) &from.sin_addr.s_addr,
      sizeof(from.sin_addr.s_addr),AF_INET)) == NULL)
    fprintf(stderr, "Can't find host %s\n", inet_ntoa(from.sin_addr));
  else
    printf("(Name is : %s)\n", hp->h_name);
	
    err=pthread_create(&tid, NULL, &GetUserInput, NULL);
	if (err != 0)
	{
            printf("\ncan't create thread :[%s]", strerror(err));
			exit(1);
			}
 // childpid = fork();
 // if (childpid == 0) {
  // printf("\nType anything followed by RETURN, or type CTRL-D to exit\n");
   // GetUserInput();
 // }

/** get data from USER, send it SERVER,
      receive it from SERVER, display it back to USER  */

  for(;;) {
    cleanup(rbuf);
    if( (rc=recv(sd, rbuf, sizeof(rbuf), 0)) < 0){
      //perror("receiving stream  message");
      exit(-1);
    }
    if (rc >0){
      rbuf[rc]=NULL;
      printf("%s", rbuf);
    }
	else {
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

void *GetUserInput(void *arg)
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
		kill(getppid(), SIGINT);
		//kill(getppid(), SIGINT);
		close(sd);
		exit (0);
	}
    if (send(sd, sendBuf, sizeof(sendBuf), 0) <0 )
      perror("sending stream message");
	  
  }
  printf ("EOF... exit\n");
  close(sd);
  return(NULL);
  // pthread_exit();
  // kill(getppid(), 9);
  //exit (0);
}

