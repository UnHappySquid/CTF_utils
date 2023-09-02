#!/bin/bash
od $1  -A n -t x1 | tr -d '\n' | sed  "s/.*00 00 00 00 49 45 4e 44 ae 42 60 82//" | xxd -r -p > $2 
