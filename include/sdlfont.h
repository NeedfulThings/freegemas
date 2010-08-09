/**
 * @file customFont.h
 * 
 * @author José Tomás Tocino García
 * @date 2010
 *
 * Class to use custom fonts in Gosu & Linux
 * 
 * Copyright (C) 2010 José Tomás Tocino García <theom3ga@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */


#ifndef _CUSTOMFONT_H_
#define _CUSTOMFONT_H_

#include "Gosu/Gosu.hpp"

#include <boost/scoped_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>

#include <SDL_ttf.h>

#include <map>
#include <cstdio>

class SDLFont : boost::noncopyable
{
    TTF_Font* font;
    
    class SDLSurface : boost::noncopyable
    {
	SDL_Surface* surface;
        
    public:
	SDLSurface(TTF_Font* font, const std::wstring& text, Gosu::Color c)
	    {
		SDL_Color color = { c.red(), c.green(), c.blue() };
		surface = TTF_RenderUTF8_Blended(font, Gosu::wstringToUTF8(text).c_str(), color);
		if (!surface)
		    throw std::runtime_error("Could not render text " 
					     + Gosu::wstringToUTF8(text));
	    }
            
	~SDLSurface()
	    {
		SDL_FreeSurface(surface);
	    }
            
	unsigned height() const
	    {
		return surface->h;
	    }
            
	unsigned width() const
	    {
		return surface->w;
	    }
            
	const void* data() const
	    {
		return surface->pixels;
	    }
    };
        
    Gosu::Graphics & graphics;
    boost::unordered_map<wchar_t, boost::shared_ptr<Gosu::Image> > fontGlyphs;

    boost::shared_ptr<Gosu::Image> getGlyph(const wchar_t & w){
	if(fontGlyphs.find(w) == fontGlyphs.end()){

	    const std::wstring text(1, w);

	    SDLSurface S(font, text, 0xffffffff);

	    Gosu::Bitmap B;
	    B.resize(S.width(), S.height());
	    std::memcpy(B.data(), S.data(), S.width() * S.height() * 4);

	    fontGlyphs[w].reset(new Gosu::Image(graphics, B));	    
	}

	return fontGlyphs[w];	 
    }
    
public:
    SDLFont(Gosu::Graphics & G, const std::wstring& fontName, unsigned fontHeight)
	: graphics(G)
	{
	    static int initResult = TTF_Init();
	    if (initResult < 0)
		throw std::runtime_error("Could not initialize SDL_TTF");

	    // Try to open the font at the given path
	    font = TTF_OpenFont(Gosu::wstringToUTF8(fontName).c_str(), fontHeight);

	    if (!font)
		throw std::runtime_error("Could not open TTF file " 
					 + Gosu::wstringToUTF8(fontName));
	    
	    /*
	      cout << "FHeight: " << TTF_FontHeight(font) << endl
	      << "FAscent: " << TTF_FontAscent(font) << endl
	      << "FDescen: " << TTF_FontDescent(font) << endl
	      << "FSkip:   " << TTF_FontLineSkip(font) << endl; 
	    //*/
		
	}
        
    ~SDLFont()
	{
	    TTF_CloseFont(font);
	}

    unsigned textWidth(const std::wstring& text){
	return SDLSurface(font, text, 0xffffff).width();
    }

    void draw(const std::wstring & text, int x, int y, Gosu::ZPos z, int fx = 1, int fy = 1, Gosu:: Color color = 0xffffffff){
	int acumX = 0;

	BOOST_FOREACH(const wchar_t & w, text){
	    boost::shared_ptr<Gosu::Image> currentGlyph = getGlyph(w);
	    currentGlyph-> draw(x + acumX, y, z, 1, 1, color);
	    
	    acumX += currentGlyph -> width();
	}
    }

    void drawText(Gosu::Bitmap& bmp, const std::wstring& text, int x, int y, Gosu::Color c) {
	SDLSurface surf(font, text, c);

	Gosu::Bitmap temp;
	cout << surf.width() << " " << surf.height() << endl;

	temp.resize(surf.width(), surf.height());
	std::memcpy(temp.data(), surf.data(), temp.width() * temp.height() * 4);
	    
	if(bmp.height() < temp.height()){
	    bmp.resize(surf.width(), temp.height());
	}
	    
	bmp.insert(temp, x, y);
    }
};

#endif /* _CUSTOMFONT_H_ */
