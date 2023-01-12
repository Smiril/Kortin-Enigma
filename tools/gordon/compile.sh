#!/usr/bin/env bash

echo "Make Binary? Do that? [Y,n]"
read input
if [[ $input == "Y" || $input == "y" ]]; then
        echo "doing that ..."
        cd src/
        make
        cd ../
else
        echo "don't doing that ..."
fi
echo "Make Install Binary? Do that? [Y,n]"
read input
if [[ $input == "Y" || $input == "y" ]]; then
        echo "doing that ..."
        sudo make -C src/ install
        make -C src/ clean
else
        echo "don't doing that ..."
fi
exit 0
