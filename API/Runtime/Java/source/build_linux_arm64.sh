#! /bin/sh

echo "Building HASP Java native library"
if [ "$JDK" = "" ]; then
echo "Error: please set the variable JDK first"
exit 0
fi

rm -f HASPJava.o
rm -f libHASPJava_arm64.so

aarch64-linux-gnu-gcc  -I$JDK/include -I$JDK/include/linux -I../../C -DSUN_JNI -c -O2 -Wall -fPIC -o HASPJava.o HASPJava.c
aarch64-linux-gnu-gcc  -shared -o libHASPJava_arm64.so HASPJava.o ../../C/arm64/libhasp_linux_arm64_demo.a

