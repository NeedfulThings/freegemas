CC     := gcc
CXX    := g++
LD     := g++
AR     := ar rc
RANLIB := ranlib

CXXFLAGS  := -Wall -g
CXXFLAGS  += -I. -Igosu `gosu/bin/gosu-config --cxxflags`

LDFLAGS   := -Wall
LDFLAGS   += `gosu/bin/gosu-config --libs --cxxflags` 

LIBS      := gosu/lib/libgosu.a

OUTPUT += programa

all: $(OUTPUT)

SRCS += main.cpp log.cpp game.cpp state.cpp stateGame.cpp board.cpp

OBJS += $(addsuffix .o,$(basename $(SRCS)))

$(OUTPUT): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS) 

libgosu:
	cd gosu/linux ; make clean ; ./configure && make

regosu:
	make -C gosu/linux

clean:
	rm $(OBJS) $(OUTPUT) *~ -rf

log.o: log.h
game.o: game.h log.h state.h stateGame.h
state.o: state.h log.h stateGame.h
stateGame.o: stateGame.h state.h game.h log.h board.h
board.o: board.h log.h