SWIGS (1.3.7 version)
====================

SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with (or without) solute transport in channels,
channel networks and rivers.

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
* TODO: List of tasks TO DO.
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

Debian 7.8 (Linux, kFreeBSD or Hurd)
____________________________________
DragonFly BSD 4.0.3
___________________
FreeBSD 10.1
____________
NetBSD 6.1.5 (with modular xorg)
________________________________

* download the latest [JB library](https://github.com/jburguete/jb)
* cd 1.3.7
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.14.1 jb)
* aclocal
* autoconf
* automake --add-missing
* ./configure --with-gtk
* make
* strip swigs swigsbin (optional: to make a final version)

Microsoft Windows 7 32 bits
___________________________
Microsoft Windows 7 64 bits
___________________________
Microsoft Windows 8.1 64 bits
___________________________

* install [MSYS2](http://sourceforge.net/projects/msys2) and the required
libraries and utilities.
* download the latest [JB library](https://github.com/jburguete/jb)
* cd 1.3.7
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.14.1 jb)
* aclocal
* autoconf
* automake --add-missing
* ./configure --with-gtk
* make
* strip win*/bin/*.exe (optional: to make a final version)

OpenBSD 5.6
___________

* download the latest [JB library](https://github.com/jburguete/jb)
* cd 1.3.7
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.14.1 jb)
* export AUTOCONF_VERSION=2.69 AUTOMAKE_VERSION=1.14
* aclocal
* autoconf
* automake --add-missing
* ./configure --with-gtk
* make
* strip swigs swigsbin (optional: to make a final version)
