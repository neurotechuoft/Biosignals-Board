#!/bin/bash

make

if [ -e bootup.log ]; then
	rm -rf bootup.log
fi;

sudo ./bin/neuro | tee bootup.log | less

