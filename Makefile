# Uncomment lines below if you have problems with $PATH
#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)

all:
	platformio -f -c vim run --target upload --environment nodemcuv2_ota
	#platformio -f -c vim run

#upload:
	#platformio -f -c vim run --target upload --environment nodemcuv2_ota

#clean:
	#platformio -f -c vim run --target clean

#program:
	#platformio -f -c vim run --target program

#uploadfs:
	#platformio -f -c vim run --target uploadfs

#update:
	#platformio -f -c vim update