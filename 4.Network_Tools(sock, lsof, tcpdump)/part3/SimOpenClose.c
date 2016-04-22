#include "def"

main(argc, argv )
int argc;
char *argv[];
{
    int    sd1, sd2;
    struct    sockaddr_in server1, server2,server3;
    struct  hostent *hp, *gethostbyname();
    int rc;
    char buf[1024];
    char   cmd[1024];
    char *msg;
    int portnum;
   
    if (argc !=  6) {
       printf("Usage SimulOpenClose <local_host> <local_port> <remote_host> <remote_port> <message>\n");
       exit (-1);
    }
    
    msg=argv[5];


    sd1 = socket (AF_INET,SOCK_STREAM,0);

    server1.sin_family = AF_INET;
/*
    server1.sin_addr.s_addr = htonl(INADDR_ANY);
*/

    if ( (hp = gethostbyname(argv[1])) == NULL ){
        printf("local host  name error: %s\n", argv[1]);
        exit(-1);
    }
    bcopy ( hp->h_addr, &(server1.sin_addr.s_addr), hp->h_length);
    server1.sin_port = htons(atoi(argv[2]));

    if ( bind( sd1, (SA *) &server1, sizeof(server1) ) < 0 ) {
        close(sd1);
        perror("binding error: first socket ");
        exit(-1);
    }

    server2.sin_family = AF_INET;
    if ( (hp = gethostbyname(argv[3])) == NULL ){
        printf("remote host name error: %s\n", argv[3]);
        exit(-1);
    }

    bcopy ( hp->h_addr, &(server2.sin_addr.s_addr), hp->h_length);
    server2.sin_port = htons(atoi(argv[4]));

    server3.sin_port = 	atoi(argv[2]);
    
    while(1) 
    {
    if ( connect(sd1, (SA *) &server2, sizeof(server2)) < 0) {
            //close (sd1);
           perror("connecting sd1");
            //exit(-1);
    }
	else
	
		break;
    }
            send(sd1, msg, strlen(msg), 0 );
	    printf("Connected\n");
            printf("Sent: %s\n", msg);
            if( (rc=recv(sd1, buf, sizeof(buf), 0)) < 0){
                perror("receiving stream  message");
                exit(-1);
            }
            if (rc > 0){
                buf[rc]=NULL;
                printf("Received: %s\n", buf);
            }
            sleep(2);
		portnum=argv[2];
		
		sprintf (cmd, "netstat | grep %d | egrep TIME_WAIT\n", server3.sin_port );
		printf(cmd);
		close(sd1);
		printf("Closed socket\n");
		system(cmd);
		
}
