NAME: SRINIVAS JAYATHEERTH HAVANUR

CS779 HW #3



Implement Assignment #2  in any language using:

 

Part 1:     SELECT    OR    POLL

Part 2:                THREADS

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
