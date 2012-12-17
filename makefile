.SUFFIXES: .h .o .cpp

CC = main.cpp naiveBayes.cpp database.cpp nGram.cpp viterbi.cpp
CC = g++
#CXXFLAGS = -O2
CXXFLAGS = -g
LIBS = 
OBJS = main.o naiveBayes.o database.o nGram.o viterbi.o
CCEXEC = emotion
NGRAMTEST = ngram_test
NGRAMFILE = ngramTest.cpp

all:	$(CCEXEC) $(NGRAMTEST)

$(CCEXEC):	$(OBJS) makefile
	@echo Linking $@ . . .
	$(CC) $(CXXFLAGS) $(socklibs) $(LIBPTHREAD) $(OBJS) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) $(CXXFLAGS) $<

$(NGRAMTEST):$(NGRAMFILE) naiveBayes.o database.o nGram.o
	$(CC) $(NGRAMFILE) naiveBayes.o database.o nGram.o -o $(NGRAMTEST)
run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC)
	rm -f core
