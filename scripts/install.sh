#!/bin/bash

PREFIX=/usr/local
LIBNAME=libvdfc.a
INCLUDE=include

OS=""


_detect_os() {
	local name=$(uname -s)
	case "$name" in
		Linux)
			OS="Linux"
			;;
		Darwin)
			OS="macOS"
			;;
		MINGW64_NT*)
			OS="Windows"
			;;
		MSYS_NT*)
			OS="Windows"
			;;
		CYGWIN_NT*)
			OS="Windows"
			;;
		*)
			echo "Error: Unsupported OS: $name" >&2
			return 1
	esac
	return 0
}

_install() {
	if [ $# -ge 1 ]; then
		PREFIX="$1"
	# TODO: implement os specific prefix
	# elif
		# case "$OS" in
		# 	macOS) PREFIX= ;;
		# 	Windows) PREFIX= ;;
		# esac
	fi
	if ! make -re; then
		echo "Error: build failed." >&2
		return 1
	fi
	if [ ! -f "$LIBNAME" ]; then
		echo "Error: $LIBNAME not present." >&2
		return 1
	fi
	echo "Insert password when asked to install $LIBNAME to $PREFIX/lib."
	sudo install -d "$PREFIX/lib" || return 1
	sudo install -m 644 "$LIBNAME" "$PREFIX/lib" || return 1
	sudo install -d "$PREFIX/include/vdfc" || return 1
	sudo install -m 644 "$INCLUDE"/vdfc/*.h "$PREFIX/include/vdfc" || return 1
	echo "Successfully installed $LIBNAME to $PREFIX."
}

_pop_install() {
	echo "Insert password when asked to uninstall $LIBNAME from $PREFIX/lib."
	sudo rm -rf "$PREFIX/lib/$LIBNAME"
	sudo rm -rf "$PREFIX/include/vdfc"
	echo "Uninstalled $LIBNAME from $PREFIX."
}

_detect_os
