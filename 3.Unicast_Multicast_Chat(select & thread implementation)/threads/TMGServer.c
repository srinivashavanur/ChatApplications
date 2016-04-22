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
#define MAX_CLIENTS 512
/*--------------------------We initialize here---------------------------------*/
int cc;
int flag1,flag2,flag3,bufferport;
int r,irem1,irem2,x1, pr,prem1,prem2,px,x2,x3,x4;
char bufip[1024];
char buf[1024],buf1[1024],recvBuf[1024],recvBuf1[1024];
int UDPsocket,udp_socket_fd,udp_socket_bind,send_unicast;
int i,psd2, psd1;
int numTChatCli;
int tclients = 0;
int tsocket[MAX_CLIENTS];
int tcp_socket1, tcp_sock3;
pthread_t	tid1,tid2,tid3,tid4;
char buf[1024];
int cc,binding,sn;
char *Multicastip;
int UDPport;
int fromlen;
struct  hostent *hp, *gethostbyname();
struct  hostent *hp2,hp3;
struct in_addr ipv4addr;
struct  sockaddr_in client,server;
struct sockaddr_in uniCast,uniCast1;
struct sockaddr_in t_server1;
struct sockaddr_in t_server3;
struct sockaddr_in LocalHost;
struct sockaddr_in from ;
struct sockaddr_in udp_server;
/*---------------------------Function prototype declaration---------------------------*/
void *mcasttomcast(void *);
void *tcpmain(void *);
void* tcptoudp();
void *udptotcp(void *);
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
        printf("MServer Port: 	%s\n","10252");
		printf("TServer Port: 	%s\n","10253");
        printf("Multicast IP:   %d.%d.%d.%d\n",x1,x2,x3,x4);
		printf("Multicast Port:  %d\n", bufferport);
        printf("\n");
	
		CreateTcpSocket1();
		CreateTcpSocket2();
		CreateUdpSocket();

		pthread_create(&tid1, NULL, &mcasttomcast, NULL);		
		pthread_create(&tid2,NULL,&tcpmain,NULL);
		pthread_join(tid1,NULL);
		pthread_join(tid2,NULL);
		
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
                printf("New MChatClient:   (%s:%d)\n",hp2->h_name, ntohs(from.sin_port));
			    printf("\n");
                sprintf(bufip,"%d.%d.%d.%d:%d",x1,x2,x3,x4,bufferport);
                sn = send(psd2,bufip,sizeof(bufip),0);	
                if (sn < 0 )
                 perror("Error in sending to multicast message");
			}
	}
	
	void* tcpmain(void *arg)
	{
		//printf("TServe (pid:%d)\n", getpid());
		for(;;)
		{
						
						
						
						fromlen = sizeof(from);
						psd1 = accept(tcp_sock3,(SA *)&from, &fromlen);
						if (psd1 < 0)
						{
							perror("ERROR in opening listening stream socket");
							exit(-1);
						}
                        inet_pton(AF_INET, inet_ntoa(from.sin_addr), &ipv4addr);
                        hp2 = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
                        printf("New TChatClient:   (%s:%d)\n",hp2->h_name, ntohs(from.sin_port));
                        printf("\n");
						
						numTChatCli = tclients;
						tsocket[numTChatCli] = psd1;
						tclients = tclients+1;
						
						
						pthread_create(&tid3, NULL, &tcptoudp, (void*)&numTChatCli);
						pthread_create(&tid4,NULL,&udptotcp,NULL);			
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
						
						if (cc < 0) 
						{
							perror("ERROR in reading from TCP socket");
							exit(-1);
						}
						if(cc == 0)
						{
							pthread_exit(NULL);
							
							
						}
						sn = sendto(udp_socket_fd, buf, cc, 0, (SA *) &udp_server, sizeof(udp_server));
						if(sn < 0)
						{
							perror("Cannot connect to multicast\n");
						}
						
				}	
	}

	void* udptotcp(void *arg)
	{
		int i, tcpsocket, cc1 = 0;
		//printf("UDPtoTCP (pid: %d)\n",getpid());
		
		
		for(;;)
		{			
						memset(recvBuf, '\0', BUFSIZE);
						cc1=recv(udp_socket_fd,recvBuf,MAX_LEN,0);
						
						if (cc1 < 0) 
						{
							printf("ERROR in reading from UDP socket\n");
							exit(-1);
						} 
						else 
						{
							recvBuf[cc1] = '\0';		
							for(i=0; i < tclients; i++)
							{
								tcpsocket=tsocket[i];
								if (tcpsocket == -1)
								continue;
								if(send(tcpsocket,recvBuf,cc1,0)<0)
									perror ("sending message to TCP clients... \n");			
							}								
						}
		}
	}
void CreateTcpSocket1()
{
        /*-------------------------Create a TCP Socket 1-----------------------------*/
        t_server1.sin_family = AF_INET;
        t_server1.sin_addr.s_addr = htonl(INADDR_ANY);
        t_server1.sin_port = htons(10252);
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
}

void CreateTcpSocket2()
{
	
        /*-------------------------Create a TCP Socket 2-----------------------------*/
        t_server3.sin_family = AF_INET;
        t_server3.sin_addr.s_addr = htonl(INADDR_ANY);
        t_server3.sin_port = htons(10253);
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
