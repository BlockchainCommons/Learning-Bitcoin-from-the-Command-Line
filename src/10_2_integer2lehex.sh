#!/bin/bash

if [ -z $1 ];
then
    echo "You must include an integer as an argument.";
    exit;
fi

if (( $1 > "2147483647" )) || (( $1 < "-2147483647" ));
then
    echo "Your number ($1) must be between -2147483647 and 2147483647";
    exit;
fi

if [ $1 -lt 0 ];
then
    integer=$(echo $((-$1)));
    negative=1;
else
    integer=$1;
    negative=0;
fi

hex=$(printf '%08x\n' $integer | sed 's/^\(00\)*//');

hexfirst=$(echo $hex | cut -c1)
[[ 0x$hexfirst -gt 0x7 ]] && hex="00"$hex

lehex=$(echo $hex | tac -rs .. | echo "$(tr -d '\n')");

if [ "$negative" -eq "1" ];
then
   lehex=$(printf '%x\n' $((0x$lehex | 0x80)))
fi

size=$(echo -n $lehex | wc -c | awk '{print $1/2}');
hexcodeprefix=$(printf '%02x\n' $size);

echo "Integer: $1";
echo "LE Hex: $lehex";
echo "Length: $size bytes";
echo "Hexcode: $hexcodeprefix$lehex";
