# Copyright (C) 2012-2024 Simon Josefsson

# This library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of the
# License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301 USA

AC_INIT([libpskc],
  m4_esyscmd([../build-aux/oath-toolkit-version ../.tarball-version]),
  [oath-toolkit-help@nongnu.org],,
  [https://www.nongnu.org/oath-toolkit/])

VERSION_NUMBER=`"$srcdir"/../build-aux/oath-toolkit-version "$srcdir"/../.tarball-version vn`
AC_SUBST(VERSION_NUMBER)

# Library code modified:                              REVISION++
# Interfaces changed/added/removed:   CURRENT++       REVISION=0
# Interfaces added:                             AGE++
# Interfaces removed:                           AGE=0
AC_SUBST(LT_CURRENT, 0)
AC_SUBST(LT_REVISION, 1)
AC_SUBST(LT_AGE, 0)

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.13 -Wall foreign])

AC_PROG_CC
gl_EARLY
AM_PROG_AR
LT_INIT([win32-dll])
gl_INIT

# https://developer.gnome.org/gtk-doc-manual/stable/settingup_autoconf.html.en
GTK_DOC_CHECK([1.14],[--flavour no-tmpl])

AC_ARG_WITH(xmlsec-crypto-engine,
  AS_HELP_STRING([--with-xmlsec-crypto-engine=ENGINE],
                 [Use specified non-dynamic xmlsec crypto engine]),
  [xmlsecengine="-${withval}"])
AC_MSG_NOTICE([xmlsec crypto engine: $xmlsecengine])

PKG_CHECK_MODULES(XML2, libxml-2.0,, [AC_MSG_ERROR([Cannot find libxml2])])
PKG_CHECK_MODULES(XMLSEC, xmlsec1$xmlsecengine, [xmlsec=yes], [xmlsec=no])
AC_PATH_PROG(XMLCATALOG, xmlcatalog)
if test -z "$ac_cv_path_XMLCATALOG"; then
  xmlsec=no
fi
if test "$xmlsec" != "no" ; then
  AC_DEFINE(USE_XMLSEC, 1, [Define to 1 if you want to use xmlsec.])
fi
AC_MSG_CHECKING([if xmlsec should be used])
AC_MSG_RESULT($xmlsec)

save_LIBS=$LIBS
save_CFLAGS=$CFLAGS
LIBS="$LIBS $XMLSEC_LIBS"
CFLAGS="$CFLAGS $XMLSEC_CFLAGS"
AC_MSG_CHECKING([for xmlSecCryptoAppKeyLoadEx])
AC_LINK_IFELSE([AC_LANG_PROGRAM([#include <xmlsec/crypto.h>],
                                [xmlSecCryptoAppKeyLoadEx(NULL, 0, 0, NULL, NULL, NULL);])],
               [AC_MSG_RESULT(yes)
                AC_DEFINE([HAVE_XMLSECCRYPTOAPPKEYLOADEX], 1,
		          [If xmlsec has xmlSecCryptoAppKeyLoadEx])],
               [AC_MSG_RESULT(no)])
LIBS=$save_LIBS
CFLAGS=$save_CFLAGS

AC_SUBST(pskcschemadir, "\$(datarootdir)/xml/pskc")
AC_ARG_WITH(pskc-schema-dir,
  AS_HELP_STRING([--with-pskc-schema-dir=DIR],
    [Put PSKC schemas in DIR [[PREFIX/share/xml/pskc]]]),
  [case "${withval}" in
    /*)        pskcschemadir="${withval}";;
    ./*|../*)  AC_MSG_ERROR([Bad value for --with-pskc-schema-dir]);;
    *)         pskcschemadir="\$(datarootdir)/xml/${withval}";;
  esac])
AC_MSG_NOTICE([putting PSKC schema files in $pskcschemadir])

AC_ARG_ENABLE([gcc-warnings],
  [AS_HELP_STRING([--enable-gcc-warnings],
                  [turn on lots of GCC warnings (for developers)])],
  [case $enableval in
     yes|no) ;;
     *)      AC_MSG_ERROR([bad value $enableval for gcc-warnings option]) ;;
   esac
   gl_gcc_warnings=$enableval],
  [gl_gcc_warnings=no]
)

if test "$gl_gcc_warnings" = yes; then
  gl_WARN_ADD([-Werror], [WERROR_CFLAGS])
  AC_SUBST([WERROR_CFLAGS])
  gl_MANYWARN_ALL_GCC([warnings])

  # Set up the list of the pointless, undesired warnings.
  nw=
  nw="$nw -Wsystem-headers"       # Don't let system headers trigger warnings
  nw="$nw -Wundef"                # All compiler preprocessors support #if UNDEF
  nw="$nw -Wtraditional"          # All compilers nowadays support ANSI C
  nw="$nw -Wconversion"           # These warnings usually don't point to mistakes.
  nw="$nw -Wsign-conversion"      # Likewise.
  nw="$nw -Wtraditional-conversion" # Too many warnings for now
  nw="$nw -Wlogical-op"           # any use of strchr provokes this
  nw="$nw -Wunreachable-code"     # any use of strchr provokes this
  nw="$nw -Wc++-compat"           # We don't care about C++ compilers
  nw="$nw -Wcast-qual"            # Triggers oath_hotp_validate, how to fix?
  nw="$nw -Wpadded"               # Padding internal structs is useless.
  nw="$nw -Wsuggest-attribute=pure" # Is it worth using attributes?
  nw="$nw -Wsuggest-attribute=const" # Is it worth using attributes?

  # Enable all GCC warnings not in this list.
  gl_MANYWARN_COMPLEMENT([warnings], [$warnings], [$nw])
  for w in $warnings; do
    gl_WARN_ADD([$w])
  done
  gl_WARN_ADD([-fdiagnostics-show-option])
  gl_WARN_ADD([-Wno-error=overlength-strings])
  gl_WARN_ADD([-Wno-error=tautological-constant-out-of-range-compare])
  AC_SUBST([WARN_CFLAGS])

  AC_DEFINE([_FORTIFY_SOURCE], [2],
    [enable compile-time and run-time bounds-checking, and some warnings])
  AC_DEFINE([GNULIB_POSIXCHECK], [1], [enable some gnulib portability checks])
fi

AC_CONFIG_FILES([
  Makefile
  examples/Makefile
  gl/Makefile
  gtk-doc/Makefile
  gtk-doc/version.xml
  include/Makefile
  include/pskc/Makefile
  include/pskc/version.h
  libpskc.pc
  man/Makefile
  schemas/Makefile
  tests/Makefile
])
AC_OUTPUT

AC_MSG_NOTICE([summary of build options:

  Libxml2   LIBS:  $XML2_LIBS
  Libxml2 CFLAGS:  $XML2_CFLAGS
  XMLSEC enabled:  $xmlsec crypto engine: $xmlsecengine
  Xmlsec    LIBS:  $XMLSEC_LIBS
  Xmlsec  CFLAGS:  $XMLSEC_CFLAGS
])
