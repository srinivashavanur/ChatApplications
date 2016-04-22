NAME: SRINIVAS JAYATHEERTH HAVANUR

CS779 HW #2

Implement the following programs in any language (e.g., C, C++, Python) using  FORK

Name:
TMGServer  -      TCP-Multicast Group  Server.
MChatClient  -    Multicast-Chat Client.
TChatClient  -     TCP-Chat Client

SYNOPSIS
		TMGServer     
		MChatClient
		TChatClient
  

DESCRIPTION 
 
 
TMGServer:    
       	Creates a TCP server socket and binds it to sport  =10xyz, where xyz is the last 3 digits of your ODU UIN.
       	Generates a random multicast IP address I and a random port P :
       	I  is in the range: (224.0.0.0 to 239.255.255.255) &  P in the range (10000 to 11000)
       	Accepts connections from MChatClients.
	For each client, it displays its <hostname, Port> and sends it  I and  P then  close the connection.
	Creates a TCP server socket T and binds it to sport+1 to accept connections from TChatClients.
        Creates a UDP socket U,   binds it to P  to receive chat messages sent to the multicast group (I,P).
        Any multicast message received at U is sent to all TChatClients preceded by the sender’s hostname.
        Any message received from any TChatClient is sent to (I,P) out of U and to all TChatClients preceded by the sender’s hostname.
 
     
MChatClient:
       	Creates a TCP socket, connects it to the TMGServer  and reads the values of  I and P.
	The client contact the server which is running at any of the following hosts:
	{atria, sirius, something, somethingmore, somethingelse}and listening at sport.
       	Creates a UDP socket U,   binds it to P  to receive chat messages sent to the multicast group (I,P).
       	Reads any message typed by the user and sends it to (I,P) out of U.
       	Receive any message from U  and display it proceeded by the original sender’s hostname.
       	Exits when the user types CTRL-D.
 
TChatClient:
	Creates a TCP socket T, connects it to the TMGServer.
	The client contact the server which is running at any of the following hosts:
	{atria, sirius, something, somethingmore, somethingelse}and listening at sport+1.
	Reads any message typed by the user and sends it out of  T.
        Receive any message from T  and display it proceeded by the original sender’s hostname.
        Exits when the user types CTRL-D.

Include files:
	MChatClient.c
	TChatClient.c
	TMGServer.c
	Makefile
	def.h

Include executables: 
	MChatClient
	TChatClient
	TMGServer

Compile Instructions:
	1. Run the makefile to compile MChatClient.c, TChatClient.c, TMGServer.c

		make -f Makefile clean
		make -f Makefile
		
		(If you get warning messages kindly ignore it and move ahead to run the executable)

	2. Run the TCP Multicast group server executable in one of the following hosts.

		./TMGServer

	3. Run the Multicast Chat Client executable in any of the following hosts {atria, sirius, something, somethingmore, somethingelse}

                ./MChatClient

	4. Run the TCP Chat Client executable in any of the following hosts {atria, sirius, something, somethingmore, somethingelse}
		
		./TChatClient
