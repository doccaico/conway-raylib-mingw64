#!/bin/sh

# ./make.sh [debug, release]

if [ $# -lt 1 ]; then
  echo "need a parameter [debug, release]"
  exit 0
fi

if [ $1 = "debug" ]; then
  OPTIMIZATION="-O0"
  HIDE_CONSOLE=""
elif [ $1 = "release" ]; then 
  OPTIMIZATION="-O2"
  HIDE_CONSOLE="-mwindows"
else
  echo "unknown parameter: $1"
  exit 0
fi

clear

C_FLAGS="-std=c99 ${OPTIMIZATION} -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-missing-field-initializers"

COMPILER='gcc'

COMMAND="${COMPILER} ${C_FLAGS} src/main.c -o conway.exe
          -IC:/libs/raylib-5.0_win64_mingw-w64/include
          -LC:/libs/raylib-5.0_win64_mingw-w64/lib
          ${HIDE_CONSOLE}
          -lraylib -lwinmm -lgdi32"

ESC=$(printf '\033')
S='[Compile]'
echo ${ESC}[36m${S}${ESC}[m ${COMMAND}

${COMMAND}
