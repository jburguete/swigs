# To work in:
#   - Debian Linux 7.8
#   - Debian kFreeBSD 7.8
#   - Debian Hurd 7.8
#	- DragonFly BSD 4.0.3
#   - FreeBSD 10.1
#   - Microsoft Windows 7 32 bits + MSYS2
#   - Microsoft Windows 7 64 bits + MSYS2
#   - Microsoft Windows 8.1 64 bits + MSYS2
#   - NetBSD 6.1.5 + modular xorg
#
# aclocal
# autoconf
# automake --add-missing
# ./configure --with-gtk
# make

# To work in OpenBSD 5.6 prepend:
#
# export AUTOCONF_VERSION=2.69 AUTOMAKE_VERSION=1.14

AC_PREREQ([2.67])
AC_INIT([SWIGS], [1.3.10])

# Checks for operative systems
AC_CANONICAL_HOST
AM_INIT_AUTOMAKE
AC_MSG_NOTICE([Build: $build])
AC_MSG_NOTICE([Host: $host])
if test $host = "x86_64-w64-mingw32"; then
	AC_MSG_NOTICE([Microsoft Windows 64 bits])
	AC_SUBST(bindir, [../1.2.0/win64/bin/])
	AC_SUBST(etcdir, [../1.2.0/win64/etc/])
	AC_SUBST(libdir, [../1.2.0/win64/lib/])
	AC_SUBST(localdir, [../1.2.0/win64/share/locale/])
	AC_SUBST(sysdir, [/mingw/])
	AC_SUBST(basedir, [../1.2.0/win64/])
	AC_SUBST(icon, [logo.o])
	AC_SUBST(ccflags, "-march=corei7 -mfpmath=sse -msse3")
	AC_SUBST(libgcc, "libgcc_s_seh-1")
	win=1
elif test $host = "i686-pc-win32"; then
	AC_MSG_NOTICE([Microsoft Windows 32 bits])
	AC_SUBST(bindir, [../1.2.0/win32/bin/])
	AC_SUBST(etcdir, [../1.2.0/win32/etc/])
	AC_SUBST(libdir, [../1.2.0/win32/lib/])
	AC_SUBST(localdir, [../1.2.0/win32/share/locale/])
	AC_SUBST(sysdir, [/mingw/])
	AC_SUBST(basedir, [../1.2.0/win32/])
	AC_SUBST(icon, [logo.o])
	AC_SUBST(ccflags, "-march=pentium4 -mfpmath=sse -msse2")
	AC_SUBST(libgcc, "libgcc_s_dw2-1")
	win=1
elif test `uname -s` = "NetBSD"; then
	AC_MSG_NOTICE([NetBSD])
	LDFLAGS="$LDFLAGS -L/usr/pkg/lib"
	CPPFLAGS="$CPPFLAGS -I/usr/pkg/include"
	PATH="/usr/pkg/gcc48/bin:$PATH"
	win=0
elif test `uname -s` = "OpenBSD"; then
	AC_MSG_NOTICE([OpenBSD])
	LDFLAGS="$LDFLAGS -L/usr/X11R6/lib"
	CPPFLAGS="$CPPFLAGS -I/usr/X11R6/include"
	win=0
elif test `uname -s` = "DragonFly"; then
	AC_MSG_NOTICE([DragonFly BSD])
	LDFLAGS="$LDFLAGS -L/usr/local/lib"
	CPPFLAGS="$CPPFLAGS -I/usr/local/include"
	win=0
else
	AC_MSG_NOTICE([Unix type system])
	win=0
fi

# Checks for programs
AC_PROG_CC([gcc49 gcc48 egcc gcc clang])
AC_PROG_MAKE_SET
AC_LANG([C])
PKG_PROG_PKG_CONFIG

# Checking -march=native compiler flag
compiler=$CC
CC="$CC -march=native"
AC_TRY_LINK([],
	[int main(int argc, char **argv){return 0;}], [arch=1], [arch=0])
CC=$compiler
if test $arch = 0; then
	AC_MSG_NOTICE([checking for -march=native... no])
	AC_SUBST(ARCH, "")
else
	AC_MSG_NOTICE([checking for -march=native... yes])
	AC_SUBST(ARCH, "-march=native")
fi

# Checking -flto compiler flag
compiler=$CC
CC="$CC -flto"
AC_TRY_LINK([], [int main(int argc, char **argv){return 0;}], [lto=1], [lto=0])
CC=$compiler
if test $lto = 0; then
	AC_MSG_NOTICE([checking for -flto... no])
	AC_SUBST(LTO, "")
else
	AC_MSG_NOTICE([checking for -flto... yes])
	AC_SUBST(LTO, "-flto")
fi

# Checking -std compiler flag
compiler=$CC
CC="$CC -std=gnu11"
AC_TRY_LINK([], [int main(int argc, char **argv){return 0;}], [std=1], [std=0])
CC=$compiler
if test $std = 0; then
	AC_MSG_NOTICE("checking for -std=gnu11... no")
	CC="$CC -std=gnu99"
	AC_TRY_LINK([], [int main(int argc, char **argv){return 0;}], [std=1],
		[std=0])
	CC=$compiler
	if test $std = 0; then
		AC_MSG_NOTICE("checking for -std=gnu99... no")
		AC_SUBST(STD, "")
	else
		AC_MSG_NOTICE("checking for -std=gnu99... yes")
		AC_SUBST(STD, "-std=gnu99")
	fi
else
	AC_MSG_NOTICE("checking for -std=gnu11... yes")
	AC_SUBST(STD, "-std=gnu11")
fi

# Checks for libraries.
if test $win = 0; then
	AC_SUBST(bindir, [./])
	AC_SUBST(localdir, [po/])
	AC_SUBST(ccflags, [-march=native])
	AC_SUBST(latex, "pdflatex -shell-escape")
else
	AC_CHECK_TOOL(WINDRES, windres)
	AC_SUBST(latex, "pdflatex -enable-write18")
fi
AC_SEARCH_LIBS([sqrt], [m])
AC_SEARCH_LIBS([pow], [m])
AC_SEARCH_LIBS([rint], [m])
AC_SEARCH_LIBS([textdomain], [intl])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GTHREAD], [gthread-2.0])
PKG_CHECK_MODULES([XML], [libxml-2.0])
AS_IF([test -d "jb"],
	  AC_MSG_NOTICE([jb installed]),
	  AC_MSG_ERROR([jb not installed]))

# Checks for GTK+ to build the GUI version
AC_ARG_WITH([gtk], AS_HELP_STRING([--with-gtk], [Build with GTK+ interface]))
AS_IF([test "x$with_gtk" == "xyes"],
	PKG_CHECK_MODULES([PNG], [libpng])
	PKG_CHECK_MODULES([GTK], [gtk+-3.0])
	AC_SEARCH_LIBS([glEnd], [GL opengl32])
	AC_SEARCH_LIBS([glutInit], [freeglut glut])
	AC_SUBST(swigs, "swigs"))

# Checks for header files.
AC_CHECK_HEADERS([locale.h unistd.h libintl.h GL/freeglut.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([setlocale])

# Output
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
