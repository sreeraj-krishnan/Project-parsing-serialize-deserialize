CC   = g++  
CFLAGS = -std=c++11 -Ofast
INCLUDES = FileProcessor.h  NewRecord.h  Parser.h  QuoteNewRecord.h  Record.h  SignalNewRecord.h  TradeNewRecord.h 
SRCS = FileProcessor.cpp  main.cpp  NewRecord.cpp  Parser.cpp  QuoteNewRecord.cpp  SignalNewRecord.cpp  TradeNewRecord.cpp
OBJS = $(SRCS:.c=.o)
MAIN = main.exe

.PHONY: depend clean

all:	$(MAIN)
	
$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) 

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@


clean:
	rm -f *.o $(MAIN) *.binary *.txt *.stackdump output.csv;

depend: $(SRCS)
	makedepend $(INCLUDES) $^
	
rmf:
	rm *.binary output.csv
run:
	./main.exe
