#include "def.h"
#define MAXHOSTNAME 80
#define BUFSIZE 1024
#include <stddef.h>
#include <sys/utsname.h>
#include <string.h>
#include <pthread.h>
#define NULL1 '\0'
#define SA      struct sockaddr
void exit(int);
char buf[BUFSIZE],bufferip[BUFSIZE],bufferport[BUFSIZE];
char rbuf[BUFSIZE];
  pthread_t	tid1,tid2;
int cport;
  int rc, cc;
  int   sd;
void* getMessage(void*);
void* sendMessage(void*);
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
	int inSock = 0;
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

                /** get TCPClient1 Host information, NAME and INET ADDRESS */

        gethostname(ThisHost, MAXHOSTNAME);

        printf("Chat Server Name: %s\n", ThisHost);
        if  ( (hp = gethostbyname(ThisHost)) == NULL )
        {
        	fprintf(stderr, "Can't find host %s\n", argv[1]);
                exit(-1);
        }
        bcopy ( hp->h_addr, &(server.sin_addr), hp->h_length);
        printf("Chat Server IP: %s \n", inet_ntoa(server.sin_addr));

	 sd = socket (AF_INET,SOCK_STREAM,0);	

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
			 if(argc == 1)
			 {
			 server.sin_port = htons(10252);
			 }
			 else
			 {
				server.sin_port = htons(atoi(argv[1]));
			 }

                if ( connect(sd, (SA *) &server, sizeof(server)) < 0 )
                        {
                                close(sd);
				printf("\nTry connecting to: %s ...   connecting to Server at '%s'  failed\n",host[allHosts],host[allHosts]);
                        }
		else
		{
			printf("\nTry connecting to: %s ...   connecting to Server at '%s'  succesfull\n ",host[allHosts],host[allHosts]);  
			break;
		}
			allHosts++;
		}

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
	




                for(;;)
                        {

                                if( (rc=recv(sd, buf, sizeof(buf), 0)) < 0)
                                        {
                                                perror("receiving stream  message");
                                                exit(-1);
                                        }
                                if (rc > 0)
                                        {
                                                buf[rc]=NULL1;
                                                substr=strtok(buf,":");
                                                sprintf(bufferip,"%s",substr);
                                                printf("\nReceived Multicast IP: %s\n",bufferip);
                                                substr=strtok(NULL,":");
                                                sprintf(bufferport,"%s",substr);
                                                printf("Received Multicast Port: %s\n\n",bufferport);
                                        }
				MulticastIPAddress = bufferip;
        			sscanf(bufferport,"%d",&port);
        			UDPport = htons(port);
        			LocalHost.sin_family = AF_INET;
        			LocalHost.sin_port = UDPport;
        			LocalHost.sin_addr.s_addr = htons(INADDR_ANY);


        			if ((UDPsocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        			{
                			printf("can't create UDP socket: \n");
                			exit(-1);
        			}
       				 reusePort(UDPsocket);

        		        if (bind(UDPsocket, (SA *) & LocalHost, sizeof(LocalHost)) < 0)
        		      	{
                		        printf("error in bind\n");
                		        exit(-1);
        		        }
        		        LocalHost.sin_port = htons(cport);


                 	        TimeToLive = 2;
                        	setTTLvalue(UDPsocket, &TimeToLive);

                		loop = 1;               /* enable loopback */
                		setLoopback(UDPsocket, loop);

                		joinGroup(UDPsocket, MulticastIPAddress);
/*
                		if (fork() == 0)
						{
						//printf("Hostsss are : %s\n",hp->h_name);
                		sendMessage(0, UDPsocket);
						}
                		getMessage(UDPsocket, 1);
                        }
*/

						pthread_create(&tid1, NULL, &sendMessage,(void*)&inSock);
						pthread_create(&tid2, NULL, &getMessage, (void*)&UDPsocket);
						
						pthread_join(tid1,NULL);
						pthread_join(tid2,NULL);
						
						
				}
}

void joinGroup(int s, char *group)
{
        struct sockaddr_in groupStruct;
        struct ip_mreq  mreq;   /* multicast group info structure */

        if ((groupStruct.sin_addr.s_addr = inet_addr(group)) == -1)
                printf("error in inet_addr\n");

        /* check if group address is indeed a Class D address */
        mreq.imr_multiaddr = groupStruct.sin_addr;
        mreq.imr_interface.s_addr = INADDR_ANY;

        if (setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &mreq, sizeof(mreq)) == -1)
	{
                printf("error in joining group \n");
                exit(-1);
        }
}


void leaveGroup(int recvSock, char *group)
{
        struct sockaddr_in groupStruct;
        struct ip_mreq  dreq;   /* multicast group info structure */

        if ((groupStruct.sin_addr.s_addr = inet_addr(group)) == -1)
                printf("error in inet_addr\n");

        dreq.imr_multiaddr = groupStruct.sin_addr;
        dreq.imr_interface.s_addr = INADDR_ANY;

        if (setsockopt(recvSock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *) &dreq, sizeof(dreq)) == -1) 
	{
                printf("error in leaving group \n");
                exit(-1);
        }
        printf("process quitting multicast group %s \n", group);
}

void reusePort(int s)
{
        int             one = 1;

        if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof(one)) == -1) {
                printf("error in setsockopt,SO_REUSEPORT \n");
                exit(-1);
        }
}

void setTTLvalue(int s, u_char * ttl_value)
{
        if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (char *) ttl_value,
                       sizeof(u_char)) == -1) {
                printf("error in setting loopback value\n");
        }
}

void setLoopback(int s, u_char loop)
{
        if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, (char *) &loop,
                       sizeof(u_char)) == -1) {
                printf("error in disabling loopback\n");
        }
}

void* sendMessage(void* outSock)
{
		char sendBuf[MAX_LEN],sendBuf1[MAX_LEN];
		int bytes=0;
		struct sockaddr_in GroupAddress;
		
		struct utsname	myhostname;
	
		if (uname(&myhostname) < 0)
			perror("uname_error");
		
		GroupAddress.sin_family = AF_INET;
		GroupAddress.sin_port = UDPport;
		GroupAddress.sin_addr.s_addr = inet_addr(MulticastIPAddress);
		printf("Type in message to be sent to the group followed by ENTER\n");
		printf("Hit CTRL-D to quit from the chat\n");
		
		for(;;)
		{
				memset(sendBuf,'\0',MAX_LEN);
				memset(sendBuf1,'\0',MAX_LEN);
				bytes=read(0, sendBuf1, MAX_LEN);
				snprintf(sendBuf, sizeof(sendBuf), "(%s.cs.odu.edu): %s\n", myhostname.nodename, sendBuf1);
				
				if(bytes < 0)
				{
						printf("error in reading from STDIN\n");
						exit(-1);
				}
				else if(bytes == 0)
				{
						kill(getpid(),9);
						exit(0);
				}
				else if(sendto(UDPsocket, sendBuf, sizeof(sendBuf), 0, (SA *) &GroupAddress, sizeof(GroupAddress)) < 0)
				{
					printf("error in sendto\n");
					exit(-1);
				}
		}
}

void* getMessage(void* inSocks)
{
	int   bytes = 0,inSock;
	char  recvBuf[MAX_LEN];
	inSock = *((int *)inSocks);

	for (;;)
	{
		
		memset(recvBuf, '\0', MAX_LEN);
		bytes = recv(inSock, recvBuf, MAX_LEN, 0);
		if (bytes < 0)
		{
			printf("error in reading from multicast socket\n");
			exit(-1);
		} 
		else
		{		
			recvBuf[bytes] = '\0';
			 printf("%s\n", recvBuf);
		}
	}
}

cleanup(recvBuf)
char *recvBuf;
{
  int i;
  for(i=0; i<MAX_LEN; i++) buf[i]=NULL;
}