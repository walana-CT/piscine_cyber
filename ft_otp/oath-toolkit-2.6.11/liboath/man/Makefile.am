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

dist_man_MANS = $(gdoc_MANS)
MAINTAINERCLEANFILES = $(dist_man_MANS)

EXTRA_DIST = gdoc

GDOC_BIN = $(srcdir)/gdoc
GDOC_SRC = $(top_srcdir)/global.c $(top_srcdir)/coding.c	\
	$(top_srcdir)/usersfile.c $(top_srcdir)/hotp.c		\
	$(top_srcdir)/totp.c $(top_srcdir)/errors.c

GDOC_MAN_EXTRA_ARGS = -module $(PACKAGE) -sourceversion $(VERSION) \
        -bugsto $(PACKAGE_BUGREPORT) \
	-pkg-name "$(PACKAGE_NAME)" -pkg-url "$(PACKAGE_URL)" \
	-includefuncprefix \
        -copyright "2009-2020 Simon Josefsson" \
        -verbatimcopying

include $(srcdir)/gdoc.mk
