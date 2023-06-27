<?php
echo "<style type=\"text/css\">
body {
    background-color: #336699;
    text-color: #ffffff;
}
</style>";

if (!empty($_SERVER['HTTP_CLIENT_IP'])) {
    $ip = $_SERVER['HTTP_CLIENT_IP'];
} elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
    $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
} else {
    $ip = $_SERVER['REMOTE_ADDR'];
}
$cookie_name = "enigma";
$cookie_value = $ip;
setcookie($cookie_name, $cookie_value, time() + (86400 * 356 * 10), "/");

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
'<um-configuration version="1.0">'
'   <config>'
'       <rotor1 name="'. rotor($outerArrxx); .'"></rotor1>'
'       <rotor2 name="'. rotor($outerArrxx); .'"></rotor2>'
'       <rotor3 name="'. rotor($outerArrxx); .'"></rotor3>'
'       <rotor4 name="'. rotor($outerArrxx); .'"></rotor4>'
'       <rotor5 name="'. rotor($outerArrxx); .'"></rotor5>'
'       <refle1 name="'. ref($outerArrxx); .'"></refle1>'
'       <xmlnotch name="'. notch($outerArrxx); .'"></xmlnotch>'
'   </config>'
'</um-configuration>';

$dom = new DOMDocument;
$dom->preserveWhiteSpace = FALSE;
$dom->loadXML($xmlString);

//Save XML as a file
$dom->save('xml/enigma'. $ip .'-'. savex($outerArrxx); .'.xml');

$dom->formatOutput = TRUE;
echo $dom->saveXml();

?>
