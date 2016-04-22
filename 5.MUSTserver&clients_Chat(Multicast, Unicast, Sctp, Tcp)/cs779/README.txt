NAME: SRINIVAS JAYATHEERTH HAVANUR

CS779 HW #5


Implement the following programs using:

      Any language (e.g., C, C++, Python)

      Any of the following approaches:
	fork  & shared memory
	select
	threads 
 
This is an expansion of A2 to handle Unicast and SCTP (stream and seqpacket) clients in addition to Multicast and TCP clients.
Also it handles Signals to list current clients and gracefully terminates the chat session.
 
NAME

MUSTserver                 Chat Server.
[M|U|S|T|Q]client         Chat Clients.


SYNOPSIS
MUSTserver           [<port>]
[M|U|S|T|Q]client   [<port>]     

DESCRIPTION
 
MUSTserver: 
       If no <port> argument, it uses 10xyz, where xyz is the last 3 digits of your ODU UIN.
       Displays  Lists of  all current clients when a user types CTRL-C.
       Exits when the user types CTRL-/ after sending  the message “By By, no more Chatting..” to all clients.
 
Xclient:  where X is either:
 
M (Multicast)
U (Unicast)
S   SCTP Stream / Q  seQpackets
T  (TCP)


Include files:
	MChatClient.c
	TChatClient.c
	MUSTServer.c
	SClient.c
	QClient.c
	UDPClient.c
	Makefile
	def.h

Include executables: 
	MChatClient
	TChatClient
	MUSTServer
	SClient
	QClient
	UDPClient

Compile Instructions:
	1. Run the makefile to compile MChatClient.c, TChatClient.c, MUSTServer.c,QClient, SClient, UDPClient

		make -f Makefile clean
		make -f Makefile
		
		(If you get warning messages kindly ignore it and move ahead to run the executable)

	2. Run the MUST server executable in one of the following hosts(something,somethingelse,somethingmore).

		./MUSTServer or ./MUSTServer <port>

	3. Run the Multicast Chat Client executable in any of the following hosts {something, somethingmore, somethingelse}

                ./MChatClient or ./MChatClient <port>

	4. Run the TCP Chat Client executable in any of the following hosts {something, somethingmore, somethingelse}
		
		./TChatClient or ./TChatClient <port>
	5. Run the UDPClient, SClient, QClient in any of the following hosts {something, somethingmore, somethingelse}
		./UDPClient or ./UDPClient <port>
		./SClient or ./SClient <port>
		./QClient or ./QClient <port>
