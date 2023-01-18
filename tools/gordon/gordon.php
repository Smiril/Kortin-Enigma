<?php
echo "<style type=\"text/css\">
body {
    background-color: #336699;
    text-color: #ffffff;
}
</style>";

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

function savex($arr) {
    foreach ($arr as $v) {
        $chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890';
        // convert to array
        $arrxxcx = str_split($chars, 1);
        // shuffle the array
        shuffle($arrxxcx);
        // array to chars with 26 chars
        echo "\n";
        echo substr(implode('', $arrxxcx), 0, 45);
    }
}

$outerArrxx = [0];
echo "\n";

$xmlString = '<?xml version="1.0" encoding="utf-8"?>'
'    <config>
'        <xmlConfig rotor1="'<?php rotor($outerArrxx) ?>'"/>'
'        <xmlConfig rotor2="'<?php rotor($outerArrxx) ?>'"/>'
'        <xmlConfig rotor3="'<?php rotor($outerArrxx) ?>'"/>'
'        <xmlConfig rotor4="'<?php rotor($outerArrxx) ?>'"/>'
'        <xmlConfig rotor5="'<?php rotor($outerArrxx) ?>'"/>'
'        <xmlref ref="'<?php ref($outerArrxx) ?>'"/>'
'        <xmlnotch notch="'<?php notch($outerArrxx) ?>'"/>'
'    </config>';

$dom = new DOMDocument;
$dom->preserveWhiteSpace = FALSE;
$dom->loadXML($xmlString);

//Save XML as a file
$dom->save('xml/enigma'<?php savex($outerArrxx) ?>'.xml');

$dom->formatOutput = TRUE;
echo $dom->saveXml();

?>
