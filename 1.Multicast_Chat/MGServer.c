#include "def.h"
#define MAXHOSTNAME 80
void exit(int);

char bufferip[1024],bufferport[500];

main( argc, argv )
int argc;
char *argv[];
{
  int   sd, psd;
  struct   sockaddr_in server;
  struct  hostent *hp, *gethostbyname();
  struct  servent *sp;
  struct sockaddr_in from;
  int fromlen;
  int length;
  char ThisHost[80];
  int pn;
  int childpid;
  int r,irem1,irem2,x1, pr,prem1,prem2,px,x2,x3,x4;
	

	gethostname(ThisHost, MAXHOSTNAME);
	printf("\nServer Hostname:  %s\n", ThisHost);
        	if  ( (hp = gethostbyname(ThisHost)) == NULL )
        	{
                	fprintf(stderr, "Can't find host %s\n", argv[1]);
                	exit(-1);
    		}
        bcopy ( hp->h_addr, &(server.sin_addr), hp->h_length);


        /** Construct name of socket */
        server.sin_family = AF_INET;

	server.sin_addr.s_addr = htons(INADDR_ANY);
       		
    	server.sin_port = htons(10252); 

	//sd = socket (AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sd = socket (AF_INET,SOCK_STREAM,0);

        if (sd<0)
                {
                        perror("opening stream socket");
                        exit(-1);
                }

    	reusePort(sd);
    	if ( bind( sd, (SA *) &server, sizeof(server) ) < 0 )
                {
                        close(sd);
                        perror("binding name to stream socket");
                        exit(-1);
                }

    /** get port information and  prints it out */
    	length = sizeof(server);
    	if ( getsockname (sd, (SA *)&server,&length) )
                {
                        perror("getting socket name");
                        exit(0);
                }

	printf("Server Port is: %d\n", ntohs(server.sin_port));
	srand (time(NULL));
	r = rand()%11000; 
	prem1=10000-r;
        prem2=random()%1000;
        px=prem1+r+prem2;
        x2=rand()%255;
        x3=rand()%255;
        x4=rand()%255;
        pr=rand()%224;
         irem1=224-pr;
         irem2=rand()%16;
         x1=irem1+pr+irem2;

        printf("The Multicast Ip: %d.%d.%d.%d\n",x1,x2,x3,x4);
        printf("The Multicast Port: %d\n",px);
	sprintf(bufferip,"%d.%d.%d.%d",x1,x2,x3,x4);
	sprintf(bufferport,"%d",px);
  	listen(sd,4);
  	fromlen = sizeof(from);
  	for(;;)
	{
    		psd  = accept(sd, (SA *)&from, &fromlen);
    		childpid = fork();
    		if ( childpid == 0) 
		{
      			EchoServe(psd, from);
    		}
  	}
}

EchoServe(psd, from)
int psd;

struct sockaddr_in from;
{
	char buf[512];
  	int rc;
  	struct  hostent *hp, *gethostbyname();

  	if ((hp = gethostbyaddr((char *)&from.sin_addr.s_addr,sizeof(from.sin_addr.s_addr),AF_INET)) == NULL)
    		fprintf(stderr, "Can't find host %s\n", inet_ntoa(from.sin_addr));
  	else
		sprintf(buf,"%s:%s",bufferip,bufferport);
		rc=strlen(buf);

    		if (rc > 0)
		{
      			buf[rc]='\0';
      			printf("The New Chat Client is: (%s:%d)\n", hp->h_name, ntohs(from.sin_port));
      			if (send(psd, buf, rc, 0) <0 )
        		perror("sending stream message");
    		}
    		else 
		{
      			printf("TCP/Client: %s:%d\n", inet_ntoa(from.sin_addr),ntohs(from.sin_port));
      			printf("(Name is : %s)\n", hp->h_name);
      			printf("Disconnected..\n");
      			close (psd);
      			exit(0);
    		}
 }

void reusePort(int s)
{
  int one=1;
  if ( setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char *) &one,sizeof(one)) == -1 )
  {
    printf("error in setsockopt,SO_REUSEPORT \n");
    exit(-1);
  }
}      
