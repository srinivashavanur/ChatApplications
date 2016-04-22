NAME: SRINIVAS JAYATHEERTH HAVANUR
Assignment 4


Directories Include:
			part1
			part2
			part3

PART I:    Multicast Chatting

	Three persons (Alice, Bob  & Trudy) wanted to use  sock  for 	multicast chatting using one host
	(e.g., something). The first message typed by each person P 	is: "I am P".
     Show the snip screen capture of the 3 persons after typing 	their first messages.
     Use tcpdump to capture the first three typed messages.

Files include: 
		mchat_capture.jpg
		tcpdump_capture.png


PART II:    Piping to Hide

	Trudy wanted to chat with Alice and Bob but he wants to use 	piping to hide his location.
     Show how he can use sock to enable him to chat with Alice 	and Bob from his hiding location
     (e.g., somethingelse) where he can type and receive the chat 	messages.
     Use lsof  command to trace back Trudy and proof that he is 	using a host different from the host used by Alice and Bob.

Files include: 
		lsof_readme.txt
		lsof_capture.jpg
		piping_hide_chat_capture.jpg
 
PART III    Simultaneous Open & Close
 
	We have presented two programs that produces TCP 	simultaneous open and TCP simultaneous close.
	Combine these two programs into a single program that 	produces a TCP connection via simultaneous open and
	that connection is closed via simulations close.
     Use the tcpdump to capture:
		The SYN segments of the simultaneous TCP connection 			establishment 
		The FIN segments of the simultaneous TCP connection 			close.

Files include:
		SimOpenClose.c
		def
		Makefile
		SimOpenClose
		SimOpenClose_capture.jpg
		tcpdump_capture.jpg

How to run the file:
	
		1. Run the SimulOpenClose executable in two hosts for 	example( Something and Somethingelse) as shown

	SimOpenClose somethingelse 10253 something 10252 "Hello Two"
	SimOpenClose something 10252 somethingelse 10253 "Hello one"

		2. Before running the SimulOpenClose make sure to run 	tcpdump to capture FIN and SYN segments as shown.

sudo tcpdump -n port 10253 and port 10252 and 'tcp[13:1] &3 !=0'

		3. We will get output similar to the one that is shown 	in tcpdump_capture.jpg file.
