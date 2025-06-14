#!/bin/sh

# tst_hyphen.sh - test that man page part of pskctool.h2m is lintian compliant
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

set -e

srcdir="${srcdir:-.}"

! tail -n +2 $srcdir/../pskctool.h2m | egrep '[^a-z\\]-'
