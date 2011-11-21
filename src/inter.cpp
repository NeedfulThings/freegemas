#include "inter.h"
#include <Gosu/Platform.hpp>

#ifdef GOSU_IS_MAC
#import <Foundation/Foundation.h>
const char* _(const char* x) {
    return [NSLocalizedString([NSString stringWithUTF8String:x], nil) UTF8String];
}
#else
#include <libintl.h>
#include <locale.h>
const char* _(const char* x) {
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        setlocale(LC_ALL, "");
        bindtextdomain("freegemas", "lang" );
        textdomain("freegemas");    
    }
    return gettext(x);
}
#endif
