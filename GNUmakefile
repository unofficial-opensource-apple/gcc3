# Apple GCC Compiler Makefile for use by buildit.  
#
# This makefile is intended only for use with B&I buildit. For "normal"
# builds use the conventional FSF top-level makefile.
#
# Usage:
#
#  1. Set BUILDIT_DIR to where you want the build to go.  Buildit uses
#     this to set up DSTROOT, OBJROOT, etc.
#
#     examples: 
#       export BUILDIT_DIR=/tmp/build_goes_here   [sh or bash]
#       setenv BUILDIT_DIR /tmp/build_goes_here   [csh or tcsh]
#
#  2. Run buildit.  Here's a brief general description generated by
#     running buildit --help (reformatted to fit in here):
#
#     Usage: buildit projectSourceDirectory [build options] \
#                    [-- other arguments]
#
#     Mandatory argument:
#       projectSourceDirectory      The root directory of project source
#                                   tree (can be absolute or relative
#                                   path to the project source tree)
#
#     Build options:
#       -release                    Use RC_CFLAGS (compiler flags) from
#                                   specified release (use release name
#                                   like Cheetah, Puma, Jaguar, ...)
#       -arch <archName>            Build with specified architectures(s)
#                                   (-arch can be multiply specified:
#                                    -arch ppc -arch i386)
#                                   (default is native architecture)
#       -noclean                    Disables deletion of {OBJ,SYM,DST}ROOT
#                                   before start of build
#       -noinstall                  Disables invocation of 'install' target
#       -noinstallhdrs              Disables invocation of 'installhdrs' target
#       -noinstallsrc               Disables invocation of 'installsrc' target
#       -noverify                   Disables verification of project DSTROOT
#                                   after build
#       -nosum                      Disables before-and-after checksumming
#                                   of project source
#       -noprebind                  Disables prebinding
#       -noperms                    Disables permissions setting on DSTROOT
#                                   after build
#       -merge <mergeDir>           Merges resulting DSTROOT into "mergeDir"
#       -target <buildTarget>       Specifies an alternate Make or Jam build
#                                   target (instead of "install")
#       -othercflags <cflags>       Specified cflags are appended to RC_CFLAGS
#                                   for build
#
#     Other arguments:
#       Other arguments can be passed to the makefile by putting them
#       at the end of the buildit argument list following a double dash ("--").
#      Arguments that contain whitespace should be enclosed in quotes.
#
#     examples: (assume $gcc3 is shell variable with gcc3 dir in it)
#
#       Creating a new root:
#         mkdir /tmp/my-build
#         cd /tmp/my-build
#         export BUILDIT_DIR=`pwd`
#         ~rc/bin/buildit -noverify -nosum -noinstallsrc -noinstallhdrs \
#                         -arch ppc -arch i386 $gcc3 BOOTSTRAP=
#
#       Rebuilding the previous root (assumes BUILDIT_DIR unchanged):
#         ~rc/bin/buildit -noverify -nosum -noinstallsrc -noinstallhdrs \
#                         -noclean -arch ppc -arch i386 $gcc3
#
# You can specify BOOTSTRAP= to stop the normal bootstapping of the
# compiler during it's build.
#
# You can specify TARGETS=ppc (or i386) to limit the build to just one
# target.  The default is for ppc and i386. 
#
#######################################################################
#
# ** AUTOMATIC BUILDS **
#
# Automatic builds strictly follow the B&I makefile API standards.
#

# Include the set of standard Apple makefile definitions.
ifndef CoreOSMakefiles
CoreOSMakefiles = $(MAKEFILEPATH)/CoreOS
endif
include $(CoreOSMakefiles)/Standard/Standard.make

# Enable Apple extensions to (gnu)make.
USE_APPLE_PB_SUPPORT = all

HOSTS = ppc i386 # `arch`
targets = echo $${TARGETS:-'ppc i386'}
TARGETS := $(shell $(targets))

RC_ARCHS = $(HOSTS)

SRCROOT = .

SRC = `cd $(SRCROOT) && pwd | sed s,/private,,`
OBJROOT = $(SRC)/obj
SYMROOT = $(OBJROOT)/../sym
DSTROOT = $(OBJROOT)/../dst

# Set BOOTSTRAP to null (either here on on the buildit cmd line) to
# disable a bootstrap build.

BOOTSTRAP = --bootstrap

# Set ENABLE_CHECKING to null (either here or on the buildit cmd line)
# to disable generating a compiler the has tree and rtl access checking
# code in it.

#ifneq ($(RC_RELEASE),)
#    ENABLE_CHECKING = --disable-checking
#else
#    ENABLE_CHECKING = --enable-checking
#endif
ENABLE_CHECKING=
ifeq ($(ENABLE_CHECKING),)
    ENABLE_CHECKING1 = --disable-checking
else
    ifeq ($(ENABLE_CHECKING),--disable-checking)
	ENABLE_CHECKING1 = --disable-checking
    else
	ENABLE_CHECKING1 = --enable-checking
    endif
endif

VERSION = 3.1

ARCH = `arch`

PREFIX = /usr

std_include_dir = $(DSTROOT)/usr/include
gcc_hdr_dir     = $(std_include_dir)/gcc/darwin/$(VERSION)

# Order of buildit events.
# 1. installsrc
# 2. clean
# 3. installhdrs: installs $DSTROOT.hdrDst/ $DSTROOT.hdrObj/ $DSTROOT.hdrSym/
# 4. install: $DSTROOT.dst/ $DSTROOT.obj/ $DSTROOT.sym/

# DO_SYMLINKS set to 'yes' will all the full gcc3 installation which
# includes generating sym links in various places.  These sym links
# are in the same places with the same names as those generated by
# gcc2.  We may not want to clobber gcc2's sym links.  Thus setting
# DO_SYMLINKS to 'no' will suppress them.

DO_SYMLINKS = no

# DO_HDR_SYMLINKS is similar to DO_SYMLINKS but only controls the
# generation of the sym links for the headers in /usr/include.

DO_HDR_SYMLINKS = no

#######################################################################

install: installhdrs build install_no_src # libkeymgr
	@echo
	@echo ++++++++++++++
	@echo + Installing +
	@echo ++++++++++++++
	@echo
	if [ "$(DO_SYMLINKS)" = yes ]; then \
	  cd $(DSTROOT)/usr/libexec/gcc/darwin && \
	  for dir in *; do \
	    cd $(DSTROOT)/usr/libexec/gcc/darwin/$$dir && \
	    rm -f default && \
	    ln -s $(VERSION) default; \
	  done && \
	  cd $(DSTROOT)/usr/lib/gcc/darwin && \
	  rm -f default && \
	  ln -s $(VERSION) default; \
	fi
	
installhdrs: DSTROOT #cplusplus_hdrs
	@echo
	@echo ++++++++++++++++++++++
	@echo + Installing headers +
	@echo ++++++++++++++++++++++
	@echo
	mkdir -p $(gcc_hdr_dir)
	mkdir -p $(gcc_hdr_dir)/machine
	if [ "$(DO_HDR_SYMLINKS)" = yes ]; then \
	  mkdir -p $(std_include_dir)/machine && \
	  rm -f $(std_include_dir)/gcc/darwin/default && \
	  ln -s $(VERSION) $(std_include_dir)/gcc/darwin/default; \
	fi
	for file in gcc/ginclude/stdarg.h  \
	    	    gcc/ginclude/varargs.h \
	    	    gcc/ginclude/stdbool.h \
	    	    `ls more-hdrs/*.h`; do \
	    install -c -m 444 $$file $(gcc_hdr_dir) && \
	    if [ "$(DO_HDR_SYMLINKS)" = yes ]; then \
	      file1=$${file##*/} && \
	      rm -f $(std_include_dir)/$$file1 && \
	      ln -s gcc/darwin/default/$$file1 $(std_include_dir); \
	    fi \
	done
	rm -f /tmp/float.$$$$ && \
	more-hdrs/synthesize-float $(SRC) /tmp/float.$$$$ $(RC_RELEASE) && \
	install -c -m 444 /tmp/float.$$$$ $(gcc_hdr_dir)/float.h && \
	if [ "$(DO_HDR_SYMLINKS)" = yes ]; then \
	  rm -f $(std_include_dir)/float.h && \
	  ln -s gcc/darwin/default/float.h $(std_include_dir); \
	fi && \
	rm -f /tmp/float.$$$$ && \
	rm -f /tmp/limits.$$$$ && \
	more-hdrs/synthesize-limits $(SRC) /tmp/limits.$$$$ && \
	install -c -m 444 /tmp/limits.$$$$ $(gcc_hdr_dir)/machine/limits.h && \
	rm -f /tmp/limits.$$$$
	if [ "$(DO_HDR_SYMLINKS)" = yes ]; then \
	  rm -f $(std_include_dir)/machine/limits.h && \
	  ln -s ../gcc/darwin/default/machine/limits.h $(std_include_dir)/machine; \
	fi
	
# Note for future reference: Relative symlinks like the one above are aways
# relative to the sym link.  So in the above ln -s $(std_include_dir)/machine
# is "in" the machine dir.  So we need to go "up" to $(std_include_dir) and
# then down to the actual machine/limits.

build: OBJROOT SYMROOT
	@echo
	@echo +++++++++
	@echo + build +
	@echo +++++++++
	@echo
	APPLE_CC=`if echo $(SRCROOT) | grep '[0-9]$$' >/dev/null; then \
		    vers_string -f cc 2>/dev/null | sed -e 's/[-A-Za-z_]*//' \
			| sed -e 's/\.[0-9.]*//'; \
		  else date +%Y%m%d%H%M%S; \
		  fi`; \
	export APPLE_CC; \
	./build_gcc --thins \
		--srcroot=$(SRC) \
		--dstroot=$(DSTROOT) \
		--objroot=$(OBJROOT) \
		--symroot=$(SYMROOT) \
		--cflags="$(RC_CFLAGS) $(OTHER_CFLAGS) -g" \
		--hosts="$(RC_ARCHS)" \
		--targets="$(TARGETS)" \
		--prefix="$(PREFIX)" \
		--symlinks=$(DO_SYMLINKS) \
		$(ENABLE_CHECKING1) \
		$(BOOTSTRAP)

install_no_src:
	@echo
	@echo ++++++++++++++++++
	@echo + install_no_src +
	@echo ++++++++++++++++++
	@echo
	APPLE_CC=`if echo $(SRCROOT) | grep '[0-9]$$' >/dev/null; then \
		    vers_string -f cc 2>/dev/null | sed -e 's/[-A-Za-z_]*//' \
			| sed -e 's/\.[0-9.]*//'; \
		  else date +%Y%m%d%H%M%S; \
		  fi`; \
	export APPLE_CC; \
	./build_gcc --fats \
	    --srcroot=$(SRC) \
	    --dstroot=$(DSTROOT) \
	    --objroot=$(OBJROOT) \
	    --symroot=$(SYMROOT) \
	    --cflags="$(RC_CFLAGS) $(OTHER_CFLAGS)" \
	    --hosts="$(RC_ARCHS)" \
	    --targets="$(TARGETS)" \
	    --prefix="$(PREFIX)" \
	    --symlinks=$(DO_SYMLINKS)

installsrc: SRCROOT
	@echo
	@echo ++++++++++++++++++++++
	@echo + Installing sources +
	@echo ++++++++++++++++++++++
	@echo
	if [ $(SRCROOT) != . ]; then \
	  $(PAX) -rw . $(SRCROOT); \
	fi
	find -d "$(SRCROOT)" \( -type d -a -name CVS -o \
	                        -type f -a -name .DS_Store \) \
	  -exec rm -rf {} \;

#######################################################################
# C++-related targets
#
#   Headers go into $(cpp_hdr_dir), set below to
#       /usr/include/gcc/darwin/$(VERSION)/g++
#
# For now, be explicit instead of theoretically "FSF-correct"

# Most of libstdc++
LIBSTDCPP_HDRS = cassert cctype cerrno cfloat ciso646 \
    climits clocale cmath complex complex.h csetjmp \
    csignal cstdarg cstddef cstdio cstdlib cstring ctime \
    cwchar cwctype fstream iomanip iosfwd iostream \
    stdexcept stl.h string strstream

LIBSTDCPP_STL_HDRS = algo.h algobase.h algorithm alloc.h bvector.h	\
    defalloc.h deque deque.h function.h functional hash_map hash_map.h	\
    hash_set hash_set.h hashtable.h heap.h iterator iterator.h list	\
    list.h map map.h memory multimap.h multiset.h numeric pair.h	\
    pthread_alloc pthread_alloc.h queue rope rope.h ropeimpl.h set	\
    set.h slist slist.h stack stack.h stl_algo.h stl_algobase.h		\
    stl_alloc.h stl_bvector.h stl_config.h stl_construct.h stl_deque.h	\
    stl_function.h stl_hash_fun.h stl_hash_map.h stl_hash_set.h		\
    stl_hashtable.h stl_heap.h stl_iterator.h stl_list.h stl_map.h	\
    stl_multimap.h stl_multiset.h stl_numeric.h stl_pair.h stl_queue.h	\
    stl_raw_storage_iter.h stl_relops.h stl_rope.h stl_set.h		\
    stl_slist.h stl_stack.h stl_tempbuf.h stl_tree.h			\
    stl_uninitialized.h stl_vector.h tempbuf.h tree.h type_traits.h	\
    utility vector vector.h

LIBSTDCPP_STD_HDRS = bastring.cc bastring.h complext.cc complext.h	\
    dcomplex.h fcomplex.h ldcomplex.h straits.h

# Some of the standard headers (e.g., <fstream>, <iomanip>)
# just #include these <*.h> versions from ./libio.
LIBIO_HDRS = fstream.h iomanip.h iostream.h \
  libio.h streambuf.h strfile.h strstream.h

# DO WE NEED THE OTHERS?  These are all of the *other* .h
#   files in libio.  I suspect we'll need to install some
#   of them.  Any that we find we need, just move from here
#   into LIBIO_HDRS.
#
LIBIO_OTHER_H = PlotFile.h SFile.h builtinbuf.h editbuf.h \
  floatio.h indstream.h iolibio.h iostdio.h iostreamP.h \
  istream.h libioP.h ostream.h parsestream.h pfstream.h \
  procbuf.h stdiostream.h stream.h

cpp_hdr_dir=$(gcc_hdr_dir)/g++

cplusplus_hdrs: DSTROOT
	mkdir -p $(cpp_hdr_dir)
	for file in exception new new.h typeinfo; do \
	  install -c -m 444 gcc/cp/inc/$$file $(cpp_hdr_dir); \
	done
	for file in $(LIBSTDCPP_HDRS); do \
	  install -c -m 444 libstdc++/$$file $(cpp_hdr_dir); \
	done
	for file in $(LIBSTDCPP_STL_HDRS); do \
	  install -c -m 444 libstdc++/stl/$$file $(cpp_hdr_dir); \
	done
	for file in $(LIBSTDCPP_STD_HDRS); do \
	  install -c -m 444 libstdc++/std/$$file $(cpp_hdr_dir); \
	done
	for file in $(LIBIO_HDRS); do \
	  install -c -m 444 libio/$$file $(cpp_hdr_dir); \
	done
	install -c -m 444 _G_config.h $(cpp_hdr_dir)
	cd $(cpp_hdr_dir) && rm -f std && ln -s . std

#######################################################################

# keymgr build into system framework.
# Since keymgr must be built 3 times (debug, profile, optimized)
# and named according to the option set used (libxxx.a, libxxx_debug.a,
# libxxx_profile.a), this build directly places the build result in the dst
# directory.

SYS_FRAMEWORK_DST = $(DSTROOT)/usr/local/lib/system
KEYMGR_OBJ = $(OBJROOT)/keymgr
KEYMGR_DEFINES = -DMACOSX -DPART_OF_SYSTEM_FRAMEWORK
CFLAGS = -I$(SRCROOT)/gcc/config/apple $(RC_CFLAGS) $(OTHER_CFLAGS) $(KEYMGR_DEFINES)
CC = $(DSTROOT)/usr/bin/cc -no-cpp-precomp

libkeymgr: $(KEYMGR_OBJ)/libkeymgr.a $(KEYMGR_OBJ)/libkeymgr_debug.a \
	   $(KEYMGR_OBJ)/libkeymgr_profile.a
	install -d $(SYS_FRAMEWORK_DST)
	install -c -m 444 $(KEYMGR_OBJ)/libkeymgr.a \
			  $(KEYMGR_OBJ)/libkeymgr_debug.a \
			  $(KEYMGR_OBJ)/libkeymgr_profile.a \
		$(SYS_FRAMEWORK_DST)

$(KEYMGR_OBJ)/libkeymgr.a: keymgr.c $(SRCROOT)/gcc/config/apple/keymgr.h \
			   KEYMGR_OBJ
	$(CC) $(CFLAGS) -O2 -c -o $(KEYMGR_OBJ)/keymgr.o keymgr.c
	libtool -static -o $@ $(KEYMGR_OBJ)/keymgr.o

$(KEYMGR_OBJ)/libkeymgr_debug.a: keymgr.c $(SRCROOT)/gcc/config/apple/keymgr.h\
				 KEYMGR_OBJ
	$(CC) $(CFLAGS) -g -c -o $(KEYMGR_OBJ)/keymgr.o keymgr.c
	libtool -static -o $@ $(KEYMGR_OBJ)/keymgr.o

$(KEYMGR_OBJ)/libkeymgr_profile.a: keymgr.c \
				   $(SRCROOT)/gcc/config/apple/keymgr.h \
				   KEYMGR_OBJ
	$(CC) $(CFLAGS) -pg -c -o $(KEYMGR_OBJ)/keymgr.o keymgr.c
	libtool -static -o $@ $(KEYMGR_OBJ)/keymgr.o
				
#######################################################################

clean:
	@echo
	@echo ++++++++++++
	@echo + Cleaning +
	@echo ++++++++++++
	@echo
	@if [ -d $(OBJROOT) -a "$(OBJROOT)" != / ]; then \
	  echo '*** DELETING ' $(OBJROOT); \
	  rm -rf $(OBJROOT); \
	fi
	@if [ -d $(SYMROOT) -a "$(SYMROOT)" != / ]; then \
	  echo '*** DELETING ' $(SYMROOT); \
	  rm -rf $(SYMROOT); \
	fi
	@if [ -d $(DSTROOT) -a "$(DSTROOT)" != / ]; then \
	  echo '*** DELETING ' $(DSTROOT); \
	  rm -rf $(DSTROOT); \
	fi

#######################################################################

OBJROOT SYMROOT DSTROOT KEYMGR_OBJ:
	mkdir -p $($@)

#######################################################################

SRCROOT:
	@if [ -n "$($@)" ]; \
	then \
		exit 0; \
	else \
		echo Must define $@; \
		exit 1; \
	fi
