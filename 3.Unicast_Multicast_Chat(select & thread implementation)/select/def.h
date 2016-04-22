#ifndef DEF_H
#define DEF_H
#include <stdio.h>
#include <regex.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#define SA struct sockaddr
#define  h_addr h_addr_list[0]
#define MAX_LEN 1024
#define max(a,b) (((a)>(b))?(a):(b))
extern void leaveGroup(int recvSock, char *group);
extern void joinGroup(int s, char *group);
extern void reusePort(int sock);
extern void displayDaddr(int sock);
extern void setTTLvalue(int s,u_char *i);
extern void setLoopback(int s,u_char loop);
#endif
