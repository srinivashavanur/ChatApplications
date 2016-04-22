CC              =       gcc 
RM              = rm -rf 

# libraries and includes
CFLAGS          = $(NORMCFLAGS)
#Solaris
#LIBS       = -lsocket -lnsl 
LIBS       = -lnsl 

PROGS = SimOpenClose

all: $(PROGS)

SimOpenClose:  SimOpenClose.o
	$(CC) $(CFLAGS) -o SimOpenClose SimOpenClose.c $(LIBS)
clean:;
	rm -f $(PROGS) core *.o
