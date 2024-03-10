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

C_FLAGS="-std=c99 ${OPTIMIZATION} -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-missing-field-initializers -o conway.exe"

COMPILER='gcc'

COMMAND="${COMPILER} ${C_FLAGS} src/main.c
          -IC:/libs/raylib/include
          -LC:/libs/raylib/lib
          ${HIDE_CONSOLE}
          -lraylib -lwinmm -lgdi32"

ESC=$(printf '\033')
S='[Compile]'
echo ${ESC}[36m${S}${ESC}[m ${COMMAND}

${COMMAND}
