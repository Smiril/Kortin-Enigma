#!/usr/bin/env bash

echo "Make Binary? Do that? [Y,n]"
read input
if [[ $input == "Y" || $input == "y" ]]; then
        echo "doing that ..."
        cd src/
        clang main.c -o notch
        cd ../
else
        echo "don't doing that ..."
fi
echo "Make Install Binary? Do that? [Y,n]"
read input
if [[ $input == "Y" || $input == "y" ]]; then
        echo "doing that ..."
        sudo cp ./src/notch /usr/local/bin/notch
        rm -f ./src/notch
else
        echo "don't doing that ..."
fi
exit 0
