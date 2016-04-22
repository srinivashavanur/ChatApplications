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
#include<sys/shm.h>
#include <string.h>
#include <unistd.h>
#define MAX_LEN 1024
#define u_char char
#define MAXHOSTNAME 80
#define BUFSIZE 1024
/*--------------------------We initialize here---------------------------------*/
int cc;
int flag1,flag2,flag3,bufferport;
int r,irem1,irem2,x1, pr,prem1,prem2,px,x2,x3,x4;
char bufip[1024];
char buf[1024],buf1[1024],recvBuf[1024],recvBuf1[1024];
int UDPsocket,udp_socket_fd,udp_socket_bind,send_unicast;
int i,psd2, psd1;
int tcp_socket1, tcp_sock3;
char buf[1024];
char *Multicastip;
int UDPport;
struct  hostent *hp;
struct  sockaddr_in client,server;
struct sockaddr_in uniCast,uniCast1;
struct sockaddr_in t_server1;
struct sockaddr_in t_server3;
struct sockaddr_in LocalHost;
struct sockaddr_in from ;
struct sockaddr_in udp_server;
/*---------------------------Function prototype declaration---------------------------*/
void reusePort(int);
void CreateTcpSocket1();
void CreateTcpSocket2();
void CreateUdpSocket();
void joinGroup(int s, char *group);
/*-------------------------------Begin of Main function------------------------*/
int main(int argc,char *argv[])
{
        int cc,binding,sn;
		int forkprocess, childprocess, finalchildprocess;
        struct  hostent *hp, *gethostbyname();
        struct  hostent *hp2,hp3;
        struct in_addr ipv4addr;
        int fromlen;
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
		
        fromlen = sizeof(from);
        
        forkprocess	= fork();
        if(forkprocess == 0)
        {
                printf("MServe (pid %d)\n", getppid());
                printf("TServe (pid %d)\n", getpid());
				printf("Press CTRL+C to come out of this\n");
				flag1=1;
        }
        if(flag1)
        {
                for(;;)
                {
                        psd1 = accept(tcp_sock3,(SA *)&from, &fromlen);
                        inet_pton(AF_INET, inet_ntoa(from.sin_addr), &ipv4addr);
                        hp2 = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
                        printf("New TChatClient:   (%s:%d)\n",hp2->h_name, ntohs(from.sin_port));
                        printf("\n");
                        childprocess=fork();
                        if(childprocess == 0)
                        {	
				finalchildprocess=fork();
				if(finalchildprocess == 0)
				{				
					printf("TCPtoUDP (pid %d)\n",getpid());		
					for(;;)
       		                        {
						/*--------------This will receive message from tcp and send it to multicast clients-------------------*/
						cc=recv(psd1,buf,sizeof(buf), 0);
						sprintf(buf1,"(%s):%s",hp2->h_name,buf);
						sn = sendto(udp_socket_fd, buf1, sizeof(buf1), 0, (SA *) &udp_server, sizeof(udp_server));
						if(sn < 0)
						{
							perror("Cannot connect to multicast\n");
						}	
					}
							
				}									
				else
				{	
					printf("UDPtoTCP (pid %d)\n",getpid());						
					for(;;)
					{
						/*-------------------------This will receive message from multicast clients and send it to tcp----------------------*/
						memset(recvBuf, '\0', BUFSIZE);
						CreateUdpSocket();
						recvfrom(udp_socket_fd, recvBuf, sizeof(recvBuf), 0,(SA *)&udp_server,sizeof(udp_server));
						send(psd1,recvBuf,sizeof(recvBuf),0);
					}
				}
										
                        }
                }
        }
        else
        {
                for(;;)
                {
		/*------------------This will accept connection from sport and send it to all multicast clients -------------------------------*/
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
