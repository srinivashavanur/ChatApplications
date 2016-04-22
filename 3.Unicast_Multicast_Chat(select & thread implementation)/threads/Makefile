CC              =       gcc 
RM              = rm -rf 

# libraries and includes
CFLAGS=$(NORMCFLAGS)
#Solaris
#LIBS       = -lsocket -lnsl 
LIBS=-lnsl 

STD=-std=gnu99

PROGS = MChatClient TMGServer TChatClient

PTH = -pthread

all: $(PROGS)

TMGServer:  TMGServer.o
	$(CC) $(CFLAGS) -o TMGServer TMGServer.c -pthread  $(LIBS) $(STD) 
	#gcc -o TMGServer TMGServer.c -pthread

TChatClient: TChatClient.o
	$(CC) $(CFLAGS) -o TChatClient TChatClient.c -pthread $(LIBS) $(STD)

MChatClient:  MChatClient.o
	$(CC) $(CFLAGS) -o MChatClient MChatClient.c -pthread $(LIBS) $(STD)
clean:;
	rm -f $(PROGS) core *.o
