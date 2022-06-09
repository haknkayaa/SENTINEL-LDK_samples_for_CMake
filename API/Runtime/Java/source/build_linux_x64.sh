#! /bin/sh

echo "Building HASP Java native library"
if [ "$JDK" = "" ]; then
echo "Error: please set the variable JDK first"
exit 0
fi

rm -f HASPJava.o
rm -f libHASPJava_x86_64.so

gcc -I$JDK/include -I$JDK/include/linux -I../../C -DSUN_JNI -c -O2 -Wall -fPIC -o HASPJava.o HASPJava.c
gcc -shared -o libHASPJava_x86_64.so HASPJava.o ../../C/x86_64/libhasp_linux_x86_64_demo.a
