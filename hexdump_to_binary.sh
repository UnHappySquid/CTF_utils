#!/bin/bash

if [[ $# -lt 2 ]]
then
	echo "Wrong number of arguments";
	echo "Usage : $0 <input_file_path> <output_file_path>";
	exit 1;
fi	
cat $1 | sed -E 's/^[0-9a-f]{4}//' | sed -E 's/^\s+//' | sed -E 's/\s\s/ /' | xxd -p -r > $2

