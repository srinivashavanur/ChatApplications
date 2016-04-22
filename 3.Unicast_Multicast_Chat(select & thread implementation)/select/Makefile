CC              =       gcc 
RM              = rm -rf 

# libraries and includes
CFLAGS=$(NORMCFLAGS)
#Solaris
#LIBS       = -lsocket -lnsl 
LIBS=-lnsl 

STD=-std=gnu99

PROGS = MChatClient TMGServer TChatClient

all: $(PROGS)

MGServer:  TMGServer.o
	$(CC) $(CFLAGS) -o TMGServer TMGServer.c $(LIBS)

TChatClient: TChatClient.o
	$(CC) $(CFLAGS) -o TChatClient TChatClient.c $(LIBS)

MChatClient:  MChatClient.o
	$(CC) $(CFLAGS) -o MChatClient MChatClient.c $(LIBS)
clean:;
	rm -f $(PROGS) core *.o
