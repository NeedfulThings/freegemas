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

SRCS += main.cpp log.cpp juego.cpp estado.cpp estadoJuego.cpp tablero.cpp

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
juego.o: juego.h log.h estado.h estadoJuego.h
estado.o: estado.h log.h estadoJuego.h
estadoJuego.o: estadoJuego.h estado.h juego.h log.h tablero.h
tablero.o: tablero.h log.h