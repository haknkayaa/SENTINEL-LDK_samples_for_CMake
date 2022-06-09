#! /bin/sh
export LD_LIBRARY_PATH=.
java -cp ./classes:hasp-srm-api.jar:commons-logging-1.2.jar:log4j-1.2-api-2.17.1.jar:log4j-api-2.17.1.jar:log4j-core-2.17.1.jar ActivationSample
