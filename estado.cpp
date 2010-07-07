#include "estado.h"

#include "log.h"

Estado::Estado(Juego * p):padre(p){
    lDEBUG << Log::CON("Estado");
}

Estado::~Estado(){
    lDEBUG << Log::DES("Estado");
}
