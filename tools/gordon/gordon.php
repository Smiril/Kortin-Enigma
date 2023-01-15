<?php

function rotor($arr) {
    foreach ($arr as $v) {
        $chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
        // convert to array
        $arrx = str_split($chars, 1);
        // shuffle the array
        shuffle($arrx);
        // array to chars with 26 chars
        echo "\n";
        echo substr(implode('', $arrx), 0, 26);
    }
}
function ref($arr) {
    foreach ($arr as $v) {
        $chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
        // convert to array
        $arrxx = str_split($chars, 1);
        // shuffle the array
        shuffle($arrxx);
        // array to chars with 26 chars
        echo "\n";
        echo substr(implode('', $arrxx), 0, 26);
    }
}
function notch($arr) {
    foreach ($arr as $v) {
        $chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
        // convert to array
        $arrxxx = str_split($chars, 1);
        // shuffle the array
        shuffle($arrxxx);
        // array to chars with 26 chars
        echo "\n";
        echo substr(implode('', $arrxxx), 0, 5);
    }
}

$outerArrx = [0, 1, 2, 3, 4];
rotor($outerArrx);
$outerArrxx = [0];
ref($outerArrxx);
$outerArrxxx = [0];
notch($outerArrxxx);
echo "\n";
?>
