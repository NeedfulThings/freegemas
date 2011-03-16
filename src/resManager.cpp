#include "resManager.h"
#include "log.h"

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

#include <algorithm>

//boost::unordered_map<string, boost::shared_ptr<Gosu::Image> > ResourceManager::loadedImages;
//boost::unordered_map<string, boost::shared_ptr<SDLFont> > ResourceManager::loadedFonts;

boost::shared_ptr<ResourceManager> ResourceManager::pointerInstance;

boost::shared_ptr<ResourceManager> ResourceManager::instance(){
    if(pointerInstance == 0)
	pointerInstance.reset(new ResourceManager);

    return pointerInstance;
}

void ResourceManager::init(Gosu::Graphics & g_){
    g = &g_;
}

boost::shared_ptr<Gosu::Image> ResourceManager::getImage(wstring path){
    imgMapIterator it = loadedImages.find(path);
    if(it == loadedImages.end()){
//	lDEBUG << "Fetching " << Gosu::narrow(path) << "... NOT loaded. Loading now.";

        loadedImages[path].reset(new Gosu::Image(*g, path));	
    }else{
//	lDEBUG << "Fetching " << Gosu::narrow(path) << "... previously loaded.";
    }

    collectGarbage(path);
    return loadedImages[path];
}

boost::shared_ptr<Gosu::Font> ResourceManager::getFont(wstring path, int size){
    wstring key = path + boost::lexical_cast<wstring>(size);

    if(loadedFonts.find(key) == loadedFonts.end()){
//	lDEBUG << "Fetching " << Gosu::narrow(key) << "... NOT loaded. Loading now.";

        loadedFonts[key].reset(new Gosu::Font(*g, path, size, 0));
    }else{
//	lDEBUG << "Fetching " << Gosu::narrow(key) << "... previously loaded.";
    }

    collectGarbage(key);
    return loadedFonts[key];
}

ResourceManager::ResourceManager(){ 
}

void ResourceManager::collectGarbage(wstring bypass){
    for(imgMapIterator it = loadedImages.begin(); it != loadedImages.end(); ){
	if(it -> second.unique() && it -> first != bypass){
//	    lDEBUG << "Purguing: " << Gosu::narrow(it -> first);
	    loadedImages.erase(it++);
	}else{
	    ++it;
	}
    }

    for(fontMapIterator it = loadedFonts.begin(); it != loadedFonts.end(); ){
	if(it -> second.unique() && it -> first != bypass){
//	    lDEBUG << "Purguing: " << Gosu::narrow(it -> first);
	    loadedFonts.erase(it++);
	}else{
	    ++it;
	}
    }
    
}
