# Copyright (C) 2012-2024 Simon Josefsson

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

TESTS = tst_pskctool.sh tst_hyphen.sh tst_roundtrip.sh tst_libexamples.sh \
	tst_sign.sh
dist_check_SCRIPTS = $(TESTS)

EXTRA_DIST = pskc-figure2.xml pskc-figure3.xml pskc-figure4.xml	\
	pskc-figure5.xml pskc-figure6.xml pskc-figure7.xml	\
	pskc-figure8.xml pskc-figure9.xml pskc-figure10.xml

EXTRA_DIST += pskc-all.xml pskc-all-signed.xml pskc-all-human.txt

EXTRA_DIST += pskc-ns.xml pskc-ocra.xml pskc-invalid.xml

EXTRA_DIST += pskc-root-key.pem pskc-root-crt.pem \
	pskc-ee-key.pem pskc-ee-crt.pem

# http://download.gooze.eu/otp/seeds/
EXTRA_DIST += 20120919-test001-4282.xml

# Schema validation needs the XML Schemas, use our non-installed catalog.
TESTS_ENVIRONMENT = \
	XML_CATALOG_FILES=$(top_builddir)/../libpskc/schemas/catalog-pskc.xml \
	PSKCTOOL=../pskctool$(EXEEXT)
