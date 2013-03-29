#!/bin/bash
for i in {1..59}
do
   wget -q -U Mozilla -O $i.mp3 "http://translate.google.com/translate_tts?ie=UTF-8&tl=en&q=$i"
done
