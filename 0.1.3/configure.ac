# To work in:
#   - Debian Linux 7.6
#   - Debian kFreeBSD 7.6
#   - Debian Hurd 7.6
#   - FreeBSD 10.0
#   - NetBSD 6.1.5 (with modular xorg)
#   - Windows XP 32 bits with MinGW/MSYS
#   - Windows 7 32 bits with MinGW/MSYS
#
# aclocal
# autoconf
# automake --add-missing
# ./configure
# make

# To work in Microsoft Windows (64 bits) with MinGW/MSYS:
# aclocal -I /mingw/share/aclocal
# autoconf
# CPPFLAGS='-I/mingw/include -I/mingw/x86_64-w64-mingw32/include' configure --host=x86_64-w64-mingw32
# make

# To work in OpenBSD:
# AUTOCONF_VERSION=2.69 aclocal-1.11
# autoconf-2.69
# CPPFLAGS='-I/usr/local/include -I/usr/X11R6/include' LDFLAGS='-L/usr/local/lib -L/usr/X11R6/lib' ./configure
# make

# To work in DragonFly BSD:
# setenv PATH ${PATH}:/usr/pkg/gcc47/bin
# setenv CPPFLAGS '-I/usr/pkg/include -I/usr/pkg/gcc47/include -I/usr/pkg/include/gettext'
# setenv CFLAGS '-I/usr/pkg/include -I/usr/pkg/gcc47/include -I/usr/pkg/include/gettext'
# setenv LDFLAGS '-L/usr/pkg/lib -L/usr/pkg/gcc47/lib'
# aclocal
# autoconf
# ./configure --host=x86_64-pc-dragonfly
# make

AC_PREREQ([2.67])
AC_INIT([SWIGS], [0.1.3])

# Checks for operative systems
AC_CANONICAL_HOST
AM_INIT_AUTOMAKE
AC_MSG_NOTICE([Build: $build])
AC_MSG_NOTICE([Host: $host])
if test $host = "x86_64-w64-mingw32"; then
	AC_MSG_NOTICE([Microsoft Windows 64 bits])
	AC_SUBST(bindir, [../0.0.0/win64/bin/])
	AC_SUBST(etcdir, [../0.0.0/win64/etc/])
	AC_SUBST(libdir, [../0.0.0/win64/lib/])
	AC_SUBST(localdir, [../0.0.0/win64/share/locale/])
	AC_SUBST(sysdir, [/mingw/])
	AC_SUBST(basedir, [../0.0.0/win64/])
	AC_SUBST(icon, [logo.o])
	AC_SUBST(ccflags, "-march=corei7 -mfpmath=sse -msse3")
	AC_SUBST(libgcc, "libgcc_s_seh-1")
	win=1
elif test $host = "i686-pc-win32"; then
	AC_MSG_NOTICE([Microsoft Windows 32 bits])
	AC_SUBST(bindir, [../0.0.0/win32/bin/])
	AC_SUBST(etcdir, [../0.0.0/win32/etc/])
	AC_SUBST(libdir, [../0.0.0/win32/lib/])
	AC_SUBST(localdir, [../0.0.0/win32/share/locale/])
	AC_SUBST(sysdir, [/mingw/])
	AC_SUBST(basedir, [../0.0.0/win32/])
	AC_SUBST(icon, [logo.o])
	AC_SUBST(ccflags, "-march=pentium4 -mfpmath=sse -msse2")
	AC_SUBST(libgcc, "libgcc_s_dw2-1")

	win=1
else
	AC_MSG_NOTICE([Unix type system])
	AC_SUBST(bindir, [./])
	AC_SUBST(localdir, [po/])
	AC_SUBST(ccflags, [-march=native])
	win=0
fi

# Checks for programs
AC_PROG_CC([gcc49 gcc48 gcc47 gcc46 egcc gcc clang])
AC_PROG_MAKE_SET
AC_LANG([C])
PKG_PROG_PKG_CONFIG

# Checks for libraries.
AC_MSG_NOTICE([GTK+ version 3])
PKG_CHECK_MODULES([GTK], [gtk+-3.0], gtk3=1, gtk3=0)
if test $gtk3 == 0; then
	AC_MSG_NOTICE([GTK+ version 2])
	PKG_CHECK_MODULES([GTK], [gtk+-2.0])
fi
if test $win = 0; then
	AC_SUBST(latex, "pdflatex -shell-escape")
else
	AC_CHECK_TOOL(WINDRES, windres)
	AC_SUBST(latex, "pdflatex -enable-write18")
fi
AC_SEARCH_LIBS([glutInit], [freeglut glut])
AC_SEARCH_LIBS([glEnd], [GL opengl32])
PKG_CHECK_MODULES([PNG], [libpng])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GTHREAD], [gthread-2.0])
PKG_CHECK_MODULES([XML], [libxml-2.0])
AC_SEARCH_LIBS([textdomain], [intl])
AC_SEARCH_LIBS([sqrt], [m])
AC_SEARCH_LIBS([pow], [m])
AC_SEARCH_LIBS([rint], [m])

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
