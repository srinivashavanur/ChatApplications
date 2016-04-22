CC              =       gcc 
RM              = rm -rf 

# libraries and includes
CFLAGS          = $(NORMCFLAGS)
#Solaris
#LIBS       = -lsocket -lnsl 
LIBS       = -lnsl 

PROGS = MChatClient  TMGServer TChatClient

all: $(PROGS)

TChatClient: TChatClient.o
	$(CC) $(CFLAGS) -o TChatClient TChatClient.c $(LIBS)

MChatClient:  MChatClient.o
	$(CC) $(CFLAGS) -o MChatClient MChatClient.c $(LIBS)
MGServer:  TMGServer.o
	$(CC) $(CFLAGS) -o TMGServer TMGServer.c $(LIBS)
clean:;
	rm -f $(PROGS) core *.o
