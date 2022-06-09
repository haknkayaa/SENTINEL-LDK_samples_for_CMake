#! /bin/sh

echo "Building HASP Java native library"
if [ "$JDK" = "" ]; then
echo "Error: please set the variable JDK first"
exit 0
fi

rm -f HASPJava.o
rm -f libHASPJava_armhf.so

arm-linux-gnueabihf-gcc -I$JDK/include -I$JDK/include/linux -I../../C -DSUN_JNI -c -O2 -Wall -fPIC -o HASPJava.o HASPJava.c
arm-linux-gnueabihf-gcc -shared -o libHASPJava_armhf.so HASPJava.o ../../C/armhf/libhasp_linux_armhf_demo.a

