#! /bin/sh
export JDK=/usr/lib/jvm/jdk1.8.0_131
echo "Building Java Activation Samples"
if [ "$JDK" = "" ]; then
echo "Error:please set the variable JDK first"
exit 0
fi

if [ -d "$JDK" ]; then
	rm -f ./classes/*.class
else
	echo "JDK is not installed"
	exit 0
fi
$JDK/bin/javac -classpath ./classes:hasp-srm-api.jar:commons-logging-1.2.jar:log4j-1.2-api-2.17.1.jar:log4j-api-2.17.1.jar:log4j-core-2.17.1.jar -d ./classes  ./src/*.java
