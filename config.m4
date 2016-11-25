dnl $Id$
dnl config.m4 for extension wgs

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(wgs, for wgs support,
Make sure that the comment is aligned:
[  --with-wgs             Include wgs support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(wgs, whether to enable wgs support,
dnl Make sure that the comment is aligned:
dnl [  --enable-wgs           Enable wgs support])

if test "$PHP_WGS" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-wgs -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/wgs.h"  # you most likely want to change this
  dnl if test -r $PHP_WGS/$SEARCH_FOR; then # path given as parameter
  dnl   WGS_DIR=$PHP_WGS
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for wgs files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       WGS_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$WGS_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the wgs distribution])
  dnl fi

  dnl # --with-wgs -> add include path
  dnl PHP_ADD_INCLUDE($WGS_DIR/include)

  dnl # --with-wgs -> check for lib and symbol presence
  dnl LIBNAME=wgs # you may want to change this
  dnl LIBSYMBOL=wgs # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $WGS_DIR/$PHP_LIBDIR, WGS_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_WGSLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong wgs lib version or lib not found])
  dnl ],[
  dnl   -L$WGS_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(WGS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(wgs, wgs.c, $ext_shared)
fi
