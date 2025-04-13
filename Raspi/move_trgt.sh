#!/bin/bash
if [ -z "$1" ];then
	echo "insert step forward (+x) or backward (-x)"
	echo "./move_trgt +x" 
	exit 1
fi


echo -e "H300J$1:;" > /dev/ttyUSB0
