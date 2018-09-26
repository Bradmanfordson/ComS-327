CC = gcc
CXX = g++
ECHO = echo
RM = rm -f

CFLAGS = -Wall -Werror -ggdb -funroll-loops -pg
CXXFLAGS = -Wall -Werror -ggdb -funroll-loops                                           
LDFLAGS = 

BIN = dungeon
OBJS = dungeon.o heap.o

all: $(BIN) etags

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CC) $^ -o $@ $(LDFLAGS)

-include $(OBJS:.o=.d)

%.o: %.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) -MMD -MF $*.d -c $<

%.o: %.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -MMD -MF $*.d -c $<

.PHONY: all clean clobber etags

clean:
	@$(ECHO) Removing all generated files
	@$(RM) *.o $(BIN) *.d TAGS core vgcore.* gmon.out
	#remove this before submission
	clear

clobber: clean
	@$(ECHO) Removing backup files
	@$(RM) *~ \#* *pgm

etags:
	@$(ECHO) Updating TAGS
	@etags *.[ch]

# Brad's Makefile additions.
sub:
	mkdir roberts_allen.assignment-1.03/
	cp src/* roberts_allen.assignment-1.03/
	tar cvfz roberts_allen.assignment-1.03.tar.gz roberts_allen.assignment-1.03

testsub:
	make sub
	mkdir tmp/
	cp roberts_allen.assignment-1.03.tar.gz tmp/
	gzip -dc tmp/roberts_allen.assignment-1.03.tar.gz | tar xvf -

removesub:
	rm -rf tmp/
	rm -rf roberts_allen.assignment-1.03
	rm roberts_allen.assignment-1.03.tar.gz
