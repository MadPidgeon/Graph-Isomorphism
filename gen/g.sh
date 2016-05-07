#!/bin/bash
geng -c -q $1 -d$2 -D$2 g2~.txt
showg -a g2~.txt g2.txt
