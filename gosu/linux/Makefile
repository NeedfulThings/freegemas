PKG_CONFIG = pkg-config

GOSU_DIR = /home/jose/freegemas/trunk/gosu/linux/..

BASE_OBJS = Math.o Utility.o IO.o FileUnix.o TimingUnix.o WindowX.o	\
	    Graphics/Bitmap.o InputX.o TextInputX.o			\
	    Graphics/BitmapUtils.o Graphics/Color.o			\
	    Graphics/TexChunk.o Graphics/Graphics.o Graphics/Image.o	\
	    Graphics/RotFlip.o Graphics/BlockAllocator.o		\
	    Graphics/Texture.o Graphics/LargeImageData.o		\
	    Graphics/BitmapPNG.o Graphics/Font.o Graphics/BitmapBMP.o	\
	    Graphics/TextUnix.o Graphics/Text.o	Graphics/Transform.o		\
	    Graphics/BitmapColorKey.o DirectoriesUnix.o

NET_OBJS = Sockets/CommSocket.o Sockets/ListenerSocket.o	\
	   Sockets/Socket.o Sockets/MessageSocket.o

SDLMIXER_OBJ = Audio/AudioSDL.o

OBJS = $(BASE_OBJS) $(NET_OBJS) $(SDLMIXER_OBJ)

XPLS = $(GOSU_DIR)/examples/Tutorial $(GOSU_DIR)/examples/TextInput

CXXFLAGS = -fPIC -I.. -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -Wall -pthread -I/usr/include/pango-1.0 -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include    \
	   -DHAVE_SYS_STAT_H \
	   -DNDEBUG -O3 -pipe -fomit-frame-pointer \
	   -I/usr/include/SDL

LIBS = -L/usr/lib -lSDL -lSDL_mixer -L -lX11 -pthread -lpangoft2-1.0 -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0   -lpng -lstdc++ -lGL -lSDL_ttf #-lboost_thread

all: directories ../lib/libgosu.a ../lib/libgosu.so examples ../bin/gosu-config

directories:
	if [ \! -d ../bin ]; then mkdir ../bin; fi
	if [ \! -d ../lib ]; then mkdir ../lib; fi
	if [ \! -d Graphics ]; then mkdir Graphics; fi
	if [ \! -d Sockets ]; then mkdir Sockets; fi
	if [ \! -d Audio ]; then mkdir Audio; fi

../lib/libgosu.a: $(OBJS)
	ar -r ../lib/libgosu.a $(BASE_OBJS) $(NET_OBJS) $(SDLMIXER_OBJ)

../lib/libgosu.so: 
	g++ -shared -o ../lib/libgosu.so ${LIBS} $(BASE_OBJS) $(NET_OBJS) $(SDLMIXER_OBJ)

install: cpp_install

uninstall: cpp_uninstall

cpp_install:
	cp -p ../lib/libgosu.a /usr/lib/
	cp -p ../lib/libgosu.so /usr/lib/
	cp -p ../bin/gosu-config /usr/bin/
	cp -pr ../Gosu /usr/include
	ldconfig

cpp_uninstall:
	rm /usr/lib/libgosu.a
	rm /usr/lib/libgosu.so
	rm /usr/bin/gosu-config
	rm -rf /usr/include/Gosu
	ldconfig

clean:
	rm -f $(OBJS) $(XPLS) ../lib/libgosu* ../lib/gosu?* ../bin/gosu-config
	rmdir Audio Graphics Sockets

$(OBJS): %.o: $(GOSU_DIR)/GosuImpl/%.cpp
	$(CXX) $(GOSU_DIR)/GosuImpl/$*.cpp -o $@ $(CXXFLAGS) -c

examples: $(XPLS) ../lib/libgosu.a

$(XPLS): %: %.cpp ../lib/libgosu.a
	$(CXX) $@.cpp -o $@ $(CXXFLAGS) $(LIBS) ../lib/libgosu.a

../bin/gosu-config:
	echo "#!/bin/bash" > ../bin/gosu-config
	echo "while test \$$# -gt 0; do" >> ../bin/gosu-config
	echo "    case \$$1 in" >> ../bin/gosu-config
	echo "    --libs)" >> ../bin/gosu-config
	echo '    echo "$(LIBS)";;' >> ../bin/gosu-config
	echo "    --cxxflags)" >> ../bin/gosu-config
	echo '    echo "$(CXXFLAGS)";;' >> ../bin/gosu-config
	echo "    esac" >> ../bin/gosu-config
	echo "    shift" >> ../bin/gosu-config
	echo "done" >> ../bin/gosu-config
	chmod u+x ../bin/gosu-config

