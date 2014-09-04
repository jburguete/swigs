SWIGS (0.1.0 version)
====================

SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with solute transport in channels, channel networks
and rivers.

AUTHORS
-------

* Javier Burguete Tolosa (jburguete@eead.csic.es)

REQUIRED
--------

* gcc or clang (to compile the source code)
* autoconf (to generate the Makefile in different systems)
* automake (to detect the system)
* pkg-config (to find the libraries to compile)
* gettext (to work with different international locales and languages)
* gthreads (to use multicores in shared memory machines)
* glib (extended utilities of C to work with data, lists, random numbers, ...)
* png (to work with PNG files)
* gtk+ (to make the interface windows)
* freeglut (interaction with OpenGL to draw graphics)
* [jb](https://github.com/jburguete/jb.git) (utility library of J. Burguete)
* doxygen (optional: standard comments format to generate documentation)
* latex (optional: to build the PDF manuals)

FILES
-----

* configure.ac: configure generator.
* Makefile.in: Makefile generator.
* TODO: List of tasks TO DO (in spanish)
* config.h.in: config header generator.
* *.h: Header files.
* *.c: Source files.
* *.png: Diagram and logo files.
* swigs.ico: icon file.
* swigs.rc: resources file.
* Doxyfile: configuration file to generate doxygen documentation.
* po/es/LC_MESSAGES/*.po: spanish language files.
* po/fr/LC_MESSAGES/*.po: french language files.
* test/*.xml: test input files.
* test/test-*: test result files.
* manual/*: Manual files.

BUILDING INSTRUCTIONS
---------------------

Debian Linux 7.6
________________
Debian kFreeBSD 7.6
___________________
Debian Hurd 7.6
_______________
FreeBSD 10.0
___________
NetBSD 6.1.3 (with modular xorg)
____________

* download the latest [JB library](https://github.com/jburguete/jb)
* cd 0.1.0
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.4.2 jb)
* aclocal
* autoconf
* automake --add-missing
* ./configure
* make
* strip swigs swigsbin (optional: to make a final version)

Microsoft Windows 7 32 bits
___________________________
Microsoft Windows XP 32 bits
____________________________

* install [MSYS/MinGW](http://www.mingw.org) and the required libraries and
utilities. You can follow detailed instructions in
[MinGW-64-Make](https://github.com/jburguete/MinGW-64-Make)
* download the latest [JB library](https://github.com/jburguete/jb)
* cd 0.1.0
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.4.2 jb)
* aclocal
* autoconf
* automake --add-missing
* make
* strip win32/bin/*.exe (optional: to make a final version)

Microsoft Windows 7 64 bits
___________________________
Microsoft Windows XP 64 bits
____________________________

* install [MSYS/MinGW](http://www.mingw.org) and the required libraries and
utilities. You can follow detailed instructions in
[MinGW-64-Make](https://github.com/jburguete/MinGW-64-Make)
* download the latest [JB library](https://github.com/jburguete/jb)
* cd 0.1.0
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.4.2 jb)
* aclocal
* autoconf
* automake --add-missing
* configure --host=x86_64-w64-mingw32
* make
* strip win64/bin/*.exe (optional: to make a final version)

OpenBSD 5.4
__________

* download the latest [JB library](https://github.com/jburguete/jb)
* cd 0.1.0
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.4.2 jb)
* export AUTOMAKE_VERSION=1.13 AUTOCONF_VERSION=2.69
* aclocal
* autoconf
* automake --add-missing
* ./configure
* make
* strip swigs swigsbin (optional: to make a final version)
