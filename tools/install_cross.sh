#!/bin/bash

#=======TOOL-CHAIN SETUP======================================================#
# This script installs gcc and binutils alognside their dependencies. It
# currently only works in ubuntu since it refers to apt package names.
# Adapting it to fit other Linux distributions should be an easy task.
# The script does not install the tools to $PATH.
#
# The script builds gcc version 7.3 and binutils version 2.30 to target
# i686-elf. For more information on how to build a cross tool-chain refer to:
# https://wiki.osdev.org/GCC_Cross-Compiler
#
# For more information on successful builds, refer to:
# https://wiki.osdev.org/Cross-Compiler_Successful_Builds
#
# And lastly, for more information on configuration parameters for building
# gcc, refer to:
# https://gcc.gnu.org/install/configure.html
#=============================================================================#


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
# @TODO: Make the installation target and the Linux distribution parameters
#        of the script.
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#

#________PARAMETERS___________________________________________________________#

CROSS="`pwd`/cross"
GCC=gcc-9.2.0
BINUTILS=binutils-2.33.1
PREFIX=$CROSS
TARGET=i686-elf
PROG_PREFIX=ia32-

GCC_CONFIG="--enable-languages=c,c++ --disable-nls --disable-shared \
	--with-gnu-as --with-gnu-ld --disable-threads --disable-libssp"
BINUTILS_CONFIG="--disable-nls"

#_______INSTALL DEPENDENCIES__________________________________________________#

# @OBS: ubuntu specific
echo installing gcc dependencies
sudo apt install -y bison
sudo apt install -y flex
sudo apt install -y libgmp3-dev
sudo apt install -y libmpc-dev
sudo apt install -y libmpfr-dev
sudo apt install -y texinfo

#________CONFIRM INSTALLATION_________________________________________________#

if [ -d "`pwd`/cross/bin" ]; then
	read -r -p "it looks like you already have some stuff installed under
tools/cross, would you like to continue anyways? [y/N] " response
	case "$response" in
		[yY][eE][sS]|[yY])
			echo continuing with the installation ...
			;;
		*)
			exit 0
			;;
	esac
fi

#_______DOWNLOAD SOURCE_______________________________________________________#

TMP=`mktemp -d`
cd $TMP
echo building toolchain inside `pwd` ...

echo downloading gcc and binutils src ...
wget https://ftp.gnu.org/gnu/gcc/gcc-7.3.0/$GCC.tar.gz
wget https://ftp.gnu.org/gnu/binutils/$BINUTILS.tar.gz

echo extracting gcc and binutils ...
tar xvzf $GCC.tar.gz
tar xvzf $BINUTILS.tar.gz

#_______INSTALL BINUTILS______________________________________________________#

echo building binutils ...
cd $TMP
mkdir build-binutils
cd build-binutils
../$BINUTILS/configure --target=$TARGET --prefix="$PREFIX" \
	--program-prefix=$PROG_PREFIX $BINUTILS_CONFIG
make -j8
make install -j8

#_______INSTALL GCC___________________________________________________________#

echo building gcc ...
cd $TMP
mkdir build-gcc
cd build-gcc
../$GCC/configure --target=$TARGET --prefix="$PREFIX" \
	--program-prefix=$PROG_PREFIX $GCC_CONFIG
make all-gcc -j8
make all-target-libgcc -j8
make install-gcc -j8
make install-target-libgcc -j8

rm -rf $TMP
