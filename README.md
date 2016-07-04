SWIGS (1.3.14 version)
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

This software has been built and tested in the following operative systems.
Probably, it can be built in other systems, distributions, or versions but it
has not been tested.

Debian 8 (Linux, kFreeBSD or Hurd)
____________________________________
DragonFly BSD 4.4
___________________
FreeBSD 10.3
____________
NetBSD 7.0 (with modular xorg)
________________________________

1. Download the latest [JB library](https://github.com/jburguete/jb) doing on
a terminal:
> $ git clone https://github.com/jburguete/jb.git

2. Download this repository:
> $ git clone https://github.com/jburguete/swigs.git

3. Link the latest JB library to jb:
> $ cd swigs/1.3.14
>
> $ ln -s ../../jb/1.14.2 jb

4. Build doing on a terminal:
> $ ./build

Microsoft Windows 7 (with MSYS2)
________________________________
Microsoft Windows 8.1 (with MSYS2)
__________________________________

1. Install [MSYS2](http://sourceforge.net/projects/msys2) and the required
libraries and utilities. You can follow detailed instructions in
[install-unix]
(https://github.com/jburguete/install-unix/blob/master/tutorial.pdf)

2. Then, in a MSYS2 terminal, follow steps 1 to 4 of the previous Debian 8.2
section.

OpenBSD 5.9
___________

1. Select adequate versions:
> $ export AUTOCONF_VERSION=2.69 AUTOMAKE_VERSION=1.15

2. Then, in a terminal, follow steps 1 to 4 of the previous Debian 8 section.
