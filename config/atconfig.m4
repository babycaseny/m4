## ----------------------##                              -*- Autoconf -*-
## Prepare for testing.  ##
## ----------------------##

#serial 6

# Copyright 2000, 2001 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.


# AT_CONFIG(TEST-DIRECTORY, [AUTOTEST-PATH = .])
# ----------------------------------------------
# Configure the test suite.
#
# AUTOTEST-PATH must help the test suite to find the executables,
# i.e., if the executables are in `src/', pass `src'.  If there are
# also executables in the source tree, use `src:$top_srcdir/src'.

AC_DEFUN([AT_CONFIG],
[AC_CONFIG_COMMANDS([$1/atconfig],
[cat >$1/atconfig <<EOF
@%:@ Configurable variable values for building test suites.
@%:@ Generated by $[0]
@%:@ Copyright 2000, 2001 Free Software Foundation, Inc.

at_package='$at_package'
at_version='$at_version'
at_bugreport='$at_bugreport'

at_n='$at_n'
at_c='$at_c'

at_testdir='$1'
at_topbuild_2_topsrc='$at_topbuild_2_topsrc'

AUTOTEST_PATH='m4_default([$2], [.])'

SHELL=\${CONFIG_SHELL-'$at_shell'}
PATH_SEPARATOR='$at_path_separator'

EOF
],
[at_package='$PACKAGE_NAME'
at_version='$PACKAGE_VERSION'
at_bugreport='$PACKAGE_BUGREPORT'

at_n='$ECHO_N'
at_c='$ECHO_C'

# Path from top_build to top_src
at_topbuild_2_topsrc=$srcdir

at_shell='$SHELL'
at_path_separator='$PATH_SEPARATOR'
])

if test -f $srcdir/$1/atlocal.in; then
  AC_CONFIG_FILES([$1/atlocal])
fi
])# AT_CONFIG
