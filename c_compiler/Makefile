CXX		= g++
CXXFLAGS	= -g -Wall -ansi -pedantic
EXTRAS		= lexer.cpp
OBJS		= Scope.o Symbol.o Tree.o Type.o \
		  allocator.o generator.o checker.o lexer.o machine.o parser.o
PROG		= scc


all:		$(PROG)

$(PROG):	$(EXTRAS) $(OBJS)
		$(CXX) -o $(PROG) $(OBJS)

clean:;		$(RM) $(PROG) core *.o

clobber:;	$(RM) $(EXTRAS) $(PROG) core *.o

lexer.cpp:	lexer.l
		$(LEX) $(LFLAGS) -t lexer.l > lexer.cpp
