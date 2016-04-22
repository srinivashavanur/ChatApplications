#include <arpa/inet.h>
#include <netdb.h>
#include "def.h"
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>

#include <stdlib.h>
#include<sys/ipc.h>
#include <pthread.h>
#include<sys/shm.h>
#include <string.h>
#include <unistd.h>
#define MAX_LEN 1024
#define u_char char
#define MAXHOSTNAME 80
#define BUFSIZE 1024
#define MAXT 10
#define MAX_CLIENTS 512
#define MAX1 100000000
#define MAXCLIENTS  12
/*--------------------------We initialize here---------------------------------*/
int cc,cc3,i,streamsd;
int flag1,flag2,flag3,bufferport;
int Mchatcli=0;
int r,irem1,irem2,x1, pr,prem1,prem2,px,x2,x3,x4;
char bufip[1024],recvBuf[1024];
char StreamPort[100]="10252",UDPports[100]="10254";
char UDPPortNew[100]="10254";
char username[1024];
char sockdt[1024];
char buf[1024],buf1[1024],recvBuf[1024],recvBuf1[1024];
int UDPsocket,udp_socket_fd,udp_socket_bind,send_unicast;
int i,psd2, psd1,ssd2,usd,usd2;
int numTChatCli,numSChatCli,numUChatCli;
int tclients = 0,dclients =0,sclients=0,uclients=0;
char record[100];
u_char          TimeToLive;
u_char          loop;
char LISTout[MAX_LEN];
int cc1=0;
int tsocket[MAXCLIENTS],dsocket[MAXCLIENTS],ssocket[MAXCLIENTS],usocket[MAXCLIENTS];
char tsocket1[MAX1];
int tcpsocket;
int ThreadIndexT;
int tcp_socket1, tcp_sock3;
pthread_t	tid1,tid2,tid3,tid4,tid5;
char buf[1024];
int cc,binding,sn;
char *Multicastip;
int UDPport;
int fromlen;
struct user_name 
{
  char    logname[10];

}; 

struct host_name {
    char    hostname[MAX_LEN];
};

struct host_name THname[MAXCLIENTS],MHname[MAXCLIENTS],SHname[MAXCLIENTS],UHname[MAXCLIENTS];
struct  hostent *hp, *gethostbyname();
struct  hostent *hp2,*hp3,*hp4;
struct in_addr ipv4addr;
struct  sockaddr_in client,server,streamserver,userver;
struct sockaddr_in uniCast,uniCast1;
struct sockaddr_in t_server1;
struct sockaddr_in t_server3;
 struct sockaddr_in LocalHost;
int UDPsocket;
struct sockaddr_in LocalHost;
struct sockaddr_in TList[MAXT];
struct user_name Tname[MAXT];
struct sockaddr_in from ;
struct sockaddr_in udp_server;
/*---------------------------Function prototype declaration---------------------------*/
void *mcasttomcast(void *);
void *tcpmain(void *);
void* tcptoudp(void *);
int Tgetfd();
void INThandler( int sig );
void QUIThandler(int sig);
void *udptotcp(void *);
void* Streammain(void *);
void* UDPtoall(void* arg);
void* UDPmain1(void *);
void* SCTPtoUDP(void *arg);
void* McasttoUDP(void *arg);
int CreateUDPsocketNew();
void* UDPtoMcast(void *arg);
void TPIPEhandler(int sig);

void test();
void reusePort(int);
void CreateTcpSocket1();
void CreateTcpSocket2();
void CreateUdpSocket();
void joinGroup(int s, char *group);
/*-------------------------------Begin of Main function------------------------*/
int main(int argc,char *argv[])
{
        
		int forkprocess, childprocess, finalchildprocess;
        
        
        char ThisHost[80];
        
		gethostname(ThisHost, MAXHOSTNAME);
	
        if  ( (hp = gethostbyname(ThisHost)) == NULL )
        {
               	fprintf(stderr, "Can't find host %s\n", argv[1]);
               	exit(-1);
    	}
		bcopy ( hp->h_addr, &(server.sin_addr), hp->h_length);
		
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
		bufferport=px;
		sprintf(bufip,"%d.%d.%d.%d",x1,x2,x3,x4);
		Multicastip = bufip;
        printf("\n");
        printf("Server Host: %s\n",ThisHost);
        //printf("MServer Port: 	%s\n","10252");
		//printf("TServer Port: 	%s\n","10253");
		
        printf("Multicast IP:   %d.%d.%d.%d\n",x1,x2,x3,x4);
		printf("Multicast Port:  %d\n", bufferport);
        printf("\n");
	
		//CreateTcpSocket1();
		//CreateTcpSocket2();
		
		signal( SIGINT, INThandler );
		signal( SIGQUIT, QUIThandler );
		
		
		/*-------------------------Create a TCP Socket 1-----------------------------*/
        t_server1.sin_family = AF_INET;
        t_server1.sin_addr.s_addr = htonl(INADDR_ANY);
		if(argc == 1)
		{
        t_server1.sin_port = htons(10252);
		printf("MServer Port: %d\n",10252);
		}
		else
		{
			t_server1.sin_port = htons(atoi(argv[1]));
			printf("MServer Port: %s\n",argv[1]);
		}
        tcp_socket1 = socket (AF_INET,SOCK_STREAM,0);
        if (tcp_socket1 <0)
        {
                perror("Error in Creating Socket");
                exit(-1);
        }

        reusePort(tcp_socket1);

        /*--------------------Binding to the TCP Server port-----------------------*/

        if ((bind( tcp_socket1, (SA *) &t_server1, sizeof(t_server1))) == -1)
        {
                close(tcp_socket1);
                perror("Error in Binding in Socket 1");
                exit(-1);
        }

        /*--------------------Listen for the clients-----------------------*/
        listen(tcp_socket1,22);
		
		
		
		
		/*-------------------------Create a TCP Socket 2-----------------------------*/
        t_server3.sin_family = AF_INET;
        t_server3.sin_addr.s_addr = htonl(INADDR_ANY);
		if(argc == 1)
		{
			
        t_server3.sin_port = htons(10253);
		printf("TServer Port: %d\n",10253);
		printf("Stream  Port:  %d\n",10252);
		printf("QServer Port:  %d\n",10252);
		printf("UDPServer Port:%d\n",10254);
		
		}
		else
		{
				
			
			t_server3.sin_port = htons(atoi(argv[1])+1);
			printf("TServer Port: %d\n",atoi(argv[1])+1);
			sprintf(StreamPort,"%d",atoi(argv[1]));
			printf("Stream Port: %d\n",atoi(argv[1]));
			printf("QServer Port: %d\n",atoi(argv[1]));
			printf("UDPServer Port: %d\n",atoi(argv[1])+2);
			sprintf(UDPports,"%d",atoi(argv[1])+2);
			//
		}
        tcp_sock3 = socket (AF_INET,SOCK_STREAM,0);
        if (tcp_sock3 <0)
        {
                perror("Error in Creating Socket");
                exit(-1);
        }

        reusePort(tcp_sock3);

        /*--------------------Binding to the TCP Server port-----------------------*/

        if ((bind( tcp_sock3, (SA *) &t_server3, sizeof(t_server3))) == -1)
        {
                close(tcp_sock3);
                perror("Error in Binding in Socket 1");
                exit(-1);
        }

        /*-------------------listen for the clients-----------------------*/
        listen(tcp_sock3,22);
		
	
		CreateUdpSocket();
		CreateUDPsocketNew();
		
		
		pthread_create(&tid1, NULL, &mcasttomcast, NULL);		
		pthread_create(&tid2,NULL,&tcpmain,NULL);
		pthread_create(&tid3, NULL, &Streammain, NULL);
		pthread_create(&tid4, NULL,&UDPmain1,NULL);
		pthread_join(tid1,NULL);
		pthread_join(tid2,NULL);
		pthread_join(tid3,NULL);
		pthread_join(tid4,NULL);
		
		
}



void* UDPmain1(void* arg)
{
	    userver.sin_family = AF_INET;
		userver.sin_addr.s_addr = htonl(INADDR_ANY); 
		
			userver.sin_port =  htons(atoi(UDPports));
		
		usd = socket (AF_INET,SOCK_STREAM,IPPROTO_SCTP);
	
		reusePort(usd);
	
		if (usd <0)
        {
                perror("Error in Creating Socket");
                exit(-1);
        }

        reusePort(usd);
		if ( bind( usd, (SA *) &userver, sizeof(userver) ) < 0 ) {
			close(usd);
			perror("binding udT");
			exit(-1);
		}

		listen(usd,4);
	
    fromlen = sizeof(from);
    for(;;) {


        

        usd2  = accept(usd, (SA *)&from, &fromlen);
        if (usd2 < 0)
		{
			perror("ERROR in opening listening stream socket");
			exit(-1);
		}

				
            inet_pton(AF_INET, inet_ntoa(from.sin_addr), &ipv4addr);
                        hp4 = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
			sprintf(UHname[uclients].hostname,"(%s:%d)",hp4->h_name,ntohs(from.sin_port));
			printf("\nNew UClient:    (%s:%d)\n",hp4->h_name,ntohs(from.sin_port)); 
            printf("\n");
			numUChatCli = uclients;
			usocket[numUChatCli] = usd2;
			uclients = uclients+1;
			pthread_create(&tid4, NULL, &UDPtoMcast, (void*)&numUChatCli);
			//pthread_create(&tid5,NULL,&McasttoUDP, NULL);
			

}
}



void* UDPtoMcast(void *usock)
{
	struct sockaddr_in GroupAddress;
	char            buf[MAX_LEN];
    char            Message[MAX_LEN];
	char 			sender[MAX_LEN];
    int             bytes = 0,bytes1=0;
	int usockref;
	//CreateUdpSocket();
	usockref = *((int *)usock);
	//udp_server.sin_family=AF_INET;
	//	udp_server.sin_addr.s_addr=inet_addr(Multicastip);
	//udp_server.sin_port=htons(px);            //take care

		while(1)
				{
						memset(buf, '\0', MAX_LEN);
						cc=recv(usocket[usockref],buf,sizeof(buf), 0);
						
						if (cc <= 0) 
						{
							printf("UChatClient Disconnected: %s\n",  UHname[usockref].hostname);
							close(usocket[usockref]);
							usocket[usockref] = -1;
							
							break;
							perror("ERROR in reading from TCP socket");
							exit(-1);
						}
						
						sn = sendto(udp_socket_fd, buf, cc, 0, (SA *) &udp_server, sizeof(udp_server));
						if(sn < 0)
						{
							perror("Cannot tcp connect to multicast\n");
						}
						
				}	

    }


    void* mcasttomcast(void *arg)
	{
			//printf("MServe (pid:%d)\n", getpid());	
			fromlen = sizeof(from);
			for(;;)
			{
				
				psd2 = accept(tcp_socket1,(SA *)&from, &fromlen);
                inet_pton(AF_INET, inet_ntoa(from.sin_addr), &ipv4addr);
                hp2 = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);	
				sprintf(MHname[dclients].hostname,"(%s:%d)", hp2->h_name,ntohs(from.sin_port));
                printf("New MChatClient:   (%s:%d)\n",hp2->h_name, ntohs(from.sin_port));
			    printf("\n");
				numTChatCli = dclients;
                        		dsocket[numTChatCli] = psd2;		 
								dclients = dclients+1;
                sprintf(bufip,"%d.%d.%d.%d:%d",x1,x2,x3,x4,bufferport);
                sn = send(psd2,bufip,sizeof(bufip),0);	
                if (sn <= 0 )
				{
						
						perror("Error in sending to multicast message");
				}
				
			}
	}
	
	void* Streammain(void *arg)
	{

    streamserver.sin_family = AF_INET;
    streamserver.sin_addr.s_addr = htonl(INADDR_ANY); 
    streamserver.sin_port =  htons(atoi(StreamPort));

    streamsd = socket (AF_INET,SOCK_STREAM,IPPROTO_SCTP);
	
    reusePort(streamsd);
    if ( bind( streamsd, (SA *) &streamserver, sizeof(streamserver) ) < 0 ) {
        close(streamsd);
        perror("binding sdT");
        exit(-1);
    }
   
    listen(streamsd,4);
    fromlen = sizeof(from);
    for(;;) {


        

        ssd2  = accept(streamsd, (SA *)&from, &fromlen);
        if (ssd2 < 0)
		{
			perror("ERROR in opening listening stream socket");
			exit(-1);
		}

        
            inet_pton(AF_INET, inet_ntoa(from.sin_addr), &ipv4addr);
                        hp3 = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
			sprintf(SHname[sclients].hostname,"(%s:%d)",hp3->h_name,ntohs(from.sin_port));
			printf("\nNew SClient or QClient:    (%s:%d)\n",hp3->h_name,ntohs(from.sin_port)); 
            printf("\n");
			numSChatCli = sclients;
			ssocket[numSChatCli] = ssd2;
			sclients = sclients+1;
			pthread_create(&tid3, NULL, &SCTPtoUDP, (void*)&numSChatCli);
					
    }
	}
	
	
	void* SCTPtoUDP(void *sctpsock)
	{
		int sctpsockref;						
				sctpsockref = *((int *)sctpsock);
					CreateUdpSocket();
				udp_server.sin_family=AF_INET;
		udp_server.sin_addr.s_addr=inet_addr(Multicastip);
		udp_server.sin_port=htons(px);
							
				//printf("TCPtoUDP (pid: %d)\n",getpid());
				while(1)
				{
						memset(buf, '\0', MAX_LEN);
						cc=recv(ssocket[sctpsockref],buf,sizeof(buf), 0);
						
						if (cc <= 0) 
						{
							printf("SClient or QClient Disconnected: %s\n",  SHname[sctpsockref].hostname);
							close(ssocket[sctpsockref]);
							ssocket[sctpsockref] = -1;
							
							break;
							perror("ERROR in reading from TCP socket");
							exit(-1);
						}
						
						sn = sendto(udp_socket_fd, buf, cc, 0, (SA *) &udp_server, sizeof(udp_server));
						if(sn < 0)
						{
							perror("Cannot sctp connect to multicast\n");
						}
						
				}
		
	}
	
	void* tcpmain(void *arg)
	{
		int i=0,xi;
		//printf("TServe (pid:%d)\n", getpid());
		
		for(;;)
		{
				//xi=Tgetfd();		
						
						fromlen = sizeof(from);
						psd1 = accept(tcp_sock3,(SA *)&from, &fromlen);
						
						if (psd1 < 0)
						{
							perror("ERROR in opening listening stream socket");
							exit(-1);
						}
						
                        inet_pton(AF_INET, inet_ntoa(from.sin_addr), &ipv4addr);
                        hp2 = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
                        
						sprintf(THname[tclients].hostname,"(%s:%d)", hp2->h_name,ntohs(from.sin_port));
						printf("New TChatClient:   (%s:%d)\n",hp2->h_name,ntohs(from.sin_port));
                        printf("\n");
						numTChatCli = tclients;
                        		tsocket[numTChatCli] = psd1;		 
								tclients = tclients+1;	
						//Mchatcli = dclients;
						pthread_create(&tid3, NULL, &tcptoudp, (void*)&numTChatCli);
						pthread_create(&tid4,NULL,&udptotcp,NULL);			
		}	
		
	}
	
int Tgetfd ()
{
  int i;
	
  for (i = 0; i < MAXT; i++)
  {
	 
        if (tsocket[i] == -1) return (i);
		
  }
}

	
	void* tcptoudp(void *tcpsock)
	{
				int tsockref;						
				tsockref = *((int *)tcpsock);	
							
				//printf("TCPtoUDP (pid: %d)\n",getpid());
				while(1)
				{
						memset(buf, '\0', MAX_LEN);
						cc=recv(tsocket[tsockref],buf,sizeof(buf), 0);
						
						if (cc <= 0) 
						{
							printf("TChatClient Disconnected: %s\n",  THname[tsockref].hostname);
							close(tsocket[tsockref]);
							tsocket[tsockref] = -1;
							
							break;
							perror("ERROR in reading from TCP socket");
							exit(-1);
						}
						
						sn = sendto(udp_socket_fd, buf, cc, 0, (SA *) &udp_server, sizeof(udp_server));
						if(sn < 0)
						{
							perror("Cannot tcp connect to multicast\n");
						}
						
				}	
	}
	
	void INThandler( int sig ) 
{
	test();   
}

void test()
{
	
	
	char msg[MAX_LEN];
	int i;
	printf("\n\n\n");
	printf("-------The List of All Clients--------\n");
	printf("###############TChatClients#######################\n");
	printf("\n");
	
	for(i=0;i<tclients;i++)
	{
		if(tsocket[i] != -1)
		{
			printf("%s\t\n",THname[i].hostname);
		}
	}
	printf("\n");
	printf("##################################################\n");
	
	
	printf("###############MChatClients#######################\n");
	printf("\n");
	
	
							
			for(i=0;i<dclients;i++)
			{
			if(dsocket[i] != -1)
			{
				printf("%s\t\n",MHname[i].hostname);
			}
			}	
		
	printf("\n");
	printf("##################################################\n");
	
	printf("###############SClients#######################\n");
	printf("\n");
	
	for(i=0;i<sclients;i++)
	{
		if(ssocket[i] != -1)
		{
			printf("%s\t\n",SHname[i].hostname);
		}
	}
	printf("\n");
	printf("##################################################\n");
	
	printf("###############UClients#######################\n");
	printf("\n");
	
	for(i=0;i<uclients;i++)
	{
		if(usocket[i] != -1)
		{
			printf("%s\t\n",UHname[i].hostname);
		}
	}
	printf("\n");
	printf("##################################################\n");
}

void QUIThandler(int sig)
{
	printf("Quit it man\n");
	
					
							for(i=0; i < tclients; i++)
							{
								tcpsocket=tsocket[i];
								if (tcpsocket == -1)
								continue;
								if(send(tcpsocket,"Bye Bye no more chatting",cc1,0)<0)
									//perror ("sending message to TCP clients... \n");
											printf("");
							}	
							
							for(i=0; i < uclients; i++)
							{
								
								if (usocket[i] == -1)
								continue;
								if(send(usocket[i],"Bye Bye no more chatting",cc1,0)<0)
									//perror ("sending message to TCP clients... \n");
										printf("");	
										
							}	
							
							for(i=0; i < sclients; i++)
							{
								
								if (ssocket[i] == -1)
								continue;
								if(send(ssocket[i],"Bye Bye no more chatting",cc1,0)<0)
								//if(write(ssocket[i],"Bye Bye no more chatting",cc1)<0)
									//perror ("sending message to TCP clients... \n");	
												printf("");
							//	if (sendto(ssocket[i], sendBuf, sizeof(sendBuf), 0,(SA *) &server, sizeof(server)) <0 )
							}
							
							
							sn = sendto(udp_socket_fd, "Bye Bye no more chatting", cc, 0, (SA *) &udp_server, sizeof(udp_server));
						if(sn < 0)
						{
							perror("Cannot connect to multicast\n");
						}
	exit(1);
}

	void* udptotcp(void *udpsock)
	{
		int i,udpsockref;
		//udpsockref = *((int *)udpsock);
		//printf("UDPtoTCP (pid: %d)\n",getpid());
		char            sender[100];
		
		for(;;)
		{			
						memset(recvBuf, '\0', BUFSIZE);
						cc1=recv(udp_socket_fd,recvBuf,MAX_LEN,0);
						
						if (cc1 < 0) 
						{
							printf("ERROR in reading from UDP socket\n");
							//printf("MChatClient Disconnected: %s\n",  THname[udpsockref].hostname);
							//close(dsocket[udpsockref]);
							//dsocket[udpsockref] = -1;
							
							//break;
							exit(-1);
						} 
						else 
						{
							hp = gethostbyaddr((char *) &from.sin_addr.s_addr, sizeof(from.sin_addr.s_addr),AF_INET);
							sprintf( sender, "(%s): ", hp->h_name);
							recvBuf[cc1] = '\0';	
											
							for(i=0; i < tclients; i++)
							{
								
								if (tsocket[i] == -1)
								continue;
								if(send(tsocket[i],recvBuf,cc1,0)<0)
									//perror ("sending message to TCP clients1111... \n");	
										printf("");
							}		


							recvBuf[cc1] = '\0';
											
							for(i=0; i < sclients; i++)
							{
								
								if (ssocket[i] == -1)
								continue;
								if(write(ssocket[i],recvBuf,strlen(recvBuf))<0)
									//perror ("sending message to STCP clients1111... \n");		
											printf("");
							}
							recvBuf[cc1] = '\0';
							
							for(i=0;i<uclients;i++)
							{
								if(usocket[i] == -1)
								continue;
								 if(write(usocket[i],recvBuf,strlen(recvBuf))<0)
									//perror ("sending message to UDP clients1111... \n");	
											printf("");
							} 
								
							
						}
		}
	}

int CreateUDPsocketNew()
{
   
	

    LocalHost.sin_family = AF_INET;
    LocalHost.sin_port = htons(atoi( UDPPortNew ) );
    LocalHost.sin_addr.s_addr = htonl(INADDR_ANY);


    if ((UDPsocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("can't create UDP socket R: \n");
        exit(-1);
    }
    reusePort(UDPsocket);

    if (bind(UDPsocket, (SA *) & LocalHost, sizeof(LocalHost)) < 0) {
        printf("error in bind UDP\n");
        exit(-1);
    }

    TimeToLive = 2;
    setTTLvalue(UDPsocket, &TimeToLive);

    loop = 1;           /* enable loopback */
    setLoopback(UDPsocket, loop);


    return UDPsocket;

}	
	
	

void CreateUdpSocket()
{
		sprintf(bufip,"%d.%d.%d.%d",x1,x2,x3,x4);
		Multicastip = bufip;
		udp_server.sin_family=AF_INET;
		udp_server.sin_addr.s_addr=inet_addr(Multicastip);
		udp_server.sin_port=htons(px);
		//getSharedMem();	
        udp_socket_fd= socket(AF_INET,SOCK_DGRAM,0);
		if (udp_socket_fd<0) {
			perror("opening datagram socket");
			exit(-1);
			}
		reusePort(udp_socket_fd);	
		
		if ( bind( udp_socket_fd, (SA *) &udp_server, sizeof(udp_server) ) < 0 ) {
		close(udp_socket_fd);
		perror("binding name to datagram socket");
		exit(-1);
		}
		joinGroup(udp_socket_fd, Multicastip);
}

void reusePort(int s)
{
  int one=1;

  if ( setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char *) &one,sizeof(one)) == -1 )
  {
    printf("error in set socket\n");
    exit(-1);
  }
}

void setTTLvalue(int s, u_char *ttl_value)
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


void joinGroup(int s, char *group)
{
	struct sockaddr_in groupStruct;
	struct ip_mreq  mreq;	/* multicast group info structure */
	if ((groupStruct.sin_addr.s_addr = inet_addr(group)) == -1){
		printf("error in inet_addr\n");
		exit(0);
	}

	/* check if group address is indeed a Class D address */
	mreq.imr_multiaddr = groupStruct.sin_addr;
	mreq.imr_interface.s_addr = INADDR_ANY;

	if (setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &mreq,
		       sizeof(mreq)) == -1) {
		printf("error in joining group \n");
		exit(-1);
	}
}
