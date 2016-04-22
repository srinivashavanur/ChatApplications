CC              =       gcc 
RM              = rm -rf 
CFLAGS          = $(NORMCFLAGS)
LIBS       = -lnsl 

PROGS = MChatClient  MGServer

all: $(PROGS)

MChatClient:  MChatClient.o
	$(CC) $(CFLAGS) -o MChatClient MChatClient.c $(LIBS)
MGServer:  MGServer.o
	$(CC) $(CFLAGS) -o MGServer MGServer.c $(LIBS)
clean:;
	rm -f $(PROGS) core *.o
