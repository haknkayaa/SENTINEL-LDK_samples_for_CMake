#!/bin/bash -x
set -e

PYINSTALLER="/usr/local/bin/pyinstaller"
CYTHON="/usr/bin/cython"
PYTHON3_INCLUDE_DIR="/usr/include/python3.7"

LDK_LINUXENV="../../../../../VendorTools/Envelope/linuxenv"
ENV_SETTINGS="--vcf:../../../../../VendorCodes/DEMOMA.hvc --fid:0"

#Location of your application:
APP_DIR="../sample_app"
START_SCRIPT="main.py"

#Output directory:
OUT_DIR="./out_python3"

#Preparation:
rm -rf $OUT_DIR
mkdir  $OUT_DIR

#Obtain script's filename without extension:
APP_NAME=$(basename -- "$START_SCRIPT")
APP_NAME="${APP_NAME%.*}"
echo "APP_NAME: $APP_NAME" # APP_NAME="main"

mkdir -p "$OUT_DIR/temp/pyinstaller/"
cp -a $APP_DIR/* "$OUT_DIR/temp/pyinstaller/"

#Run pyinstaller:
cd "$OUT_DIR/temp/pyinstaller/"
#We need '-d noarchive' so we can replace pyinstaller's collected bytecode modules with the shared objects.
$PYINSTALLER -d noarchive --log-level ERROR $START_SCRIPT
cd "../../.."

#INFO: It is important that cython sees the files with extension pyx instead of py!
mkdir "$OUT_DIR/temp/cythonize"
cp "$APP_DIR/moduleA.py" "$OUT_DIR/temp/cythonize/moduleA.pyx"
cp "$APP_DIR/moduleB.py" "$OUT_DIR/temp/cythonize/moduleB.pyx"
cd "$OUT_DIR/temp/cythonize"

#Build moduleA.pyx as a shared object (moduleA.so):
$CYTHON -3 --no-docstrings moduleA.pyx
gcc -fPIC -pthread -fwrapv -O2 -Wall -fno-strict-aliasing -I$PYTHON3_INCLUDE_DIR -c moduleA.c
gcc -pthread -shared moduleA.o -o moduleA.so

#Build moduleB.pyx as a shared object (moduleB.so):
$CYTHON -3 --no-docstrings moduleB.pyx
gcc -fPIC -pthread -fwrapv -O2 -Wall -fno-strict-aliasing -I$PYTHON3_INCLUDE_DIR -c moduleB.c
gcc -pthread -shared moduleB.o -o moduleB.so

cd ../.. #Current working directory is now $OUT_DIR
cp "./temp/cythonize/moduleA.so" "./moduleA.so.plain"
cp "./temp/cythonize/moduleB.so" "./moduleB.so.plain"

#Envelope protect the modules:
chmod u+x $LDK_LINUXENV
$LDK_LINUXENV $ENV_SETTINGS moduleA.so.plain moduleA.so
$LDK_LINUXENV $ENV_SETTINGS moduleB.so.plain moduleB.so

rm "./temp/pyinstaller/dist/$APP_NAME/moduleA.pyc"
cp "./moduleA.so" "./temp/pyinstaller/dist/$APP_NAME"

rm "./temp/pyinstaller/dist/$APP_NAME/moduleB.pyc"
cp "./moduleB.so" "./temp/pyinstaller/dist/$APP_NAME"

cp -a "./temp/pyinstaller/dist/$APP_NAME" "."

set +x #Disables cmd-echoing.
printf "\n############################################################################################\n"
printf " To distribute your application copy the folder '$OUT_DIR/$APP_NAME' to the target system.\n"
printf " To start your application run '$APP_NAME/$APP_NAME'\n"
printf "############################################################################################\n"
