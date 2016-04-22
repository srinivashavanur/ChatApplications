CC              =       gcc 
RM              = rm -rf 

# libraries and includes
CFLAGS=$(NORMCFLAGS)
#Solaris
#LIBS       = -lsocket -lnsl 
LIBS=-lnsl 

STD=-std=gnu99

PROGS = MChatClient MUSTServer TChatClient QClient SClient UDPClient

PTH = -pthread

all: $(PROGS)

MUSTServer:  MUSTServer.o
	$(CC) $(CFLAGS) -o MUSTServer MUSTServer.c -pthread  $(LIBS) $(STD) 
	#gcc -o TMGServer TMGServer.c -pthread

TChatClient: TChatClient.o
	$(CC) $(CFLAGS) -o TChatClient TChatClient.c -pthread $(LIBS) $(STD)

MChatClient:  MChatClient.o
	$(CC) $(CFLAGS) -o MChatClient MChatClient.c -pthread $(LIBS) $(STD)

QClient: QClient.o
	$(CC) $(CFLAGS) -o QClient QClient.c -pthread $(LIBS) $(STD)

SClient: SClient.o
	$(CC) $(CFLAGS) -o SClient SClient.c -pthread $(LIBS) $(STD)

UDPClient: UDPClient.o
	$(CC) $(CFLAGS) -o UDPClient UDPClient.c -pthread $(LIBS) $(STD)
clean:;
	rm -f $(PROGS) core *.o
