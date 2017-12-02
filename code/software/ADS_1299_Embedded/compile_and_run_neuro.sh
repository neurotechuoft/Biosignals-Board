#!/bin/bash

make

if [ -e bootup.log ]; then
	rm -rf bootup.log
fi;

sudo ./bin/neuro > bootup.log

#vim bootup.log
