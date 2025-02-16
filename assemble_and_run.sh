#!/bin/bash

MARS=Mars4_5.jar
TMPFILE_IN=$(mktemp)
cat $(dirname "${1}")/startup.s $(dirname "${1}")/libio.s $1 > $TMPFILE_IN 
TMPFILE_TEXT=$(mktemp)
TMPFILE_DATA=$(mktemp)
TMPFILE_HEX=$(mktemp)
java -jar $MARS a we nc mc Default $TMPFILE_IN dump .text HEX $TMPFILE_TEXT dump .data HEX $TMPFILE_DATA
echo ".text" > $TMPFILE_HEX
cat $TMPFILE_TEXT >> $TMPFILE_HEX
echo ".data" >> $TMPFILE_HEX
cat $TMPFILE_DATA >> $TMPFILE_HEX
make run FILE=$TMPFILE_HEX
rm $TMPFILE_IN $TMPFILE_TEXT $TMPFILE_DATA $TMPFILE_HEX
