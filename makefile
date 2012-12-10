.SUFFIXES: .h .o .cpp

CC = main.cpp naiveBayes.cpp
CC = g++
#CXXFLAGS = -O2
CXXFLAGS = -g
socklibs = -lnsl
LIBPTHREAD = -lpthread
LIBS = 
OBJS = main.o naiveBayes.o
CCEXEC = emotion

all:	$(CCEXEC)

$(CCEXEC):	$(OBJS) makefile
	@echo Linking $@ . . .
	$(CC) $(CXXFLAGS) $(socklibs) $(LIBPTHREAD) $(OBJS) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) $(CXXFLAGS) $<

run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC)
	rm -f core
