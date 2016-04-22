Name: SRINIVAS JAYATHEERTH HAVANUR

CS779 HW #1

Write the following programs in any language (e.g., C, C++, Python):

NAME

MGServer  -   Multicast Group  Server.
MChatClient  -    Multicast-Chat Client.

SYNOPSIS
MGServer     
MChatClient      

DESCRIPTION

MGServer: 
	Creates a TCP server socket and binds it to sport  =10xyz, where xyz is the last 3 digits of your ODU UIN.
	Generates a random multicast IP address I and a random port P :
	I  is in the range: (224.0.0.0 to 239.255.255.255) &  P in the range (10000 to 11000)
	Accepts connections from MChatClients.
	For each client, it displays its <hostname, Port> and sends it  I and  P then  close the connection.


MChatClient:
	Creates a TCP socket, connects it to the MGServer  and reads the values of  I and P.
	The client contact the server which is running at any of the following hosts:
	{atria, sirius, something, somethingmore, somethingelse}and listening at sport.
	Creates a UDP socket U,   binds it to P  to receive chat messages sent to the multicast group (I,P).
	Reads any message typed by the user and sends it to (I,P) out of U.
	Receive any message from U  and display it proceeded by the sender’s hostname.
	Exits when the user types CTRL-D.

Include files: 
	MChatClient.c
	MGServer.c
	def.h
        Makefile

Include Executables:
	MChatClient
	MGServer	

Compile Instructions:

	1. Run the makefile to Compile MGServer.c and MChatClient.c. 
	
		make -f Makefile clean
		make -f Makefile

	2. Run the Multicast Group Server executable

		./MGServer

	3. Run the Multicast Chat Client executable in any of the following hosts {atria, sirius, something, somethingmore, somethingelse}

		./MChatClient  
		
