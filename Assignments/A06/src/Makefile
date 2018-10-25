CC = gcc
CXX = g++
ECHO = echo
RM = rm -f

CFLAGS = -Wall -Werror -ggdb3 -funroll-loops
CXXFLAGS = -Wall -Werror -ggdb3 -funroll-loops
LDFLAGS = -lncurses

BIN = rlg327
OBJS = rlg327.o heap.o dungeon.o path.o utils.o pc.o \
       npc.o move.o event.o character.o io.o

all: $(BIN) etags clean

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ $(LDFLAGS)
	make clean
	./rlg327 -n 5


-include $(OBJS:.o=.d)

# %.o: %.c
# 	@$(ECHO) Compiling $<
# 	@$(CC) $(CFLAGS) -MMD -MF $*.d -c $<

%.o: %.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -MMD -MF $*.d -c $<

.PHONY: all clean clobber etags

clean:
	@$(ECHO) Removing all generated files
	@$(RM) *.o *.d TAGS core vgcore.* gmon.out
	# @$(RM) $(BIN)

clobber: clean
	@$(ECHO) Removing backup files
	@$(RM) *~ \#* *pgm

etags:
	@$(ECHO) Updating TAGS
	@etags *.[ch]
