# FreeGemas for Mac & iOS

Gosu needs a nice example project to get people started with iOS development. Because this open source game is a pretty and reasonable porting candidate, I've decided to clean it up on OS X and then later port it to iOS. (It is not eligible for release on the App Store, this is all just for documenting the process.)

# Work in Progress. TODO:

 * Fix all the #ifdef GOSU_IS_IPHONE stuff - shouldn't be so ugly!
 * Recreate iOS Xcode4 project and document it for the Gosu Wiki
 * Document what was necessary to port this typical 4:3 game to iOS
 * Fix all remaining Font inconsistencies
 * Find/add gettext->Strings.plist converter for localization
 * Flesh out this README

# Licensing

The original game has been licensed under the GPLv2. If it should ever be re-licensed under a more liberal license (MIT, BSD, zlib) all of my commits can be re-licensed without asking. E.g., to make an App Store version possible.

-- Julian Raschke, <julian@raschke.de>

# Original README by TheOm3ga:

Freegemas: open source clone of the famous Bejewelked for GNU/Linux and Windows.

Dependencies:
--------------
In a deb-based system you can install the dependencies using the following command:

    $ sudo apt-get install g++ libgl1-mesa-dev libpango1.0-dev libboost-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev

Compiling and running:
-----------------------

    $ make libgosu
    $ make
    $ ./freegemas

Chimpón
