Part2: Explanation

Here in this case trudy is chatting from somethingmore machine. 
Trudy connects to somethingelse machine with port number 10252 and uses 
pipe to connect to multicast address 224.0.0.1 with port number 10253.
Alice and bob are chatting in somethingelse machine and they are 
connected to multicast address 224.0.0.1 with port number 10253. 
If Trudy chats, Alice and Bob think that Trudy is chatting from the somethingelse 
machine even though he is typing messages from somethingmore machine. In this way,
Trudy uses piping to hide his location from Alice and bob.


lsof to traceback: Explanation

1. We want to traceback from which host trudy is chatting even though it appears that all three are chatting on same host i.e somethingelse.
We can achieve this using lsof on somethingelse machine. First of all, we need to look which are the processes using pipe with the current login. This can be done using following command
lsof -u shavanur | grep pipe

Now we noticed that there is a sock process with ID 20653 in write mode.When we looked up for this process using lsof by giving following command
lsof | grep 20653



Now we see that somethingelse is connected to somethingmore with port number 44042. When we look up for this port number using lsof, its not connected to some other host or there are are no further pipe process.
Thus we arrive at conclusion that Trudy is sending message from somethingmore not somethingelse.

