# Kortin-Enigma
 Enigma Simulation and Turing Machine (Bombe)
### Enigma 1.8 - T.E.D - The Enemy Dail - "KOENIG MARTIN" ![alt text](images/Apple_m1.jpeg "MADE with Love for MacOS")


```
* Author: Smiril <sonar@gmx.com>
* Based in the code of:
*  capi_x <capi_x@haibane.org>
*  Harald Schmidl <hjschmidl@acm.org>
*  Fauzan Mirza's <fauzan@dcs.rhbnc.ac.uk>

```

 5 Rotor Enigma Simulation with Bruteforce Message function

 6 versions of one Enigma without Source change

The bombe is based on [known-plaintext attack (KPA)](http://en.wikipedia.org/wiki/Known-plaintext_attack) for this we need a Dictonary.
This attack is assisted for stadistical and heuristics to help the KPA.


## Options

* --option-[1-6]         <--- the main functions
* --option-[1-6][a/b]    <--- Basic Bruteforce
* --help                 <--- Help Page
* --version              <--- The Version

![alt text](images/enigma.jpeg "4 Rotor Enigma")


## Cloning

```
git clone https://github.com/Smiril/Kortin-Enigma.git
```

## Instalation

```
cd Kortin-Enigma/bin
tar -xvzf enigma.tar.gz
sudo cp ./enigma /usr/local/bin/enigma
sudo mkdir -p /usr/local/share/enigma/
sudo cp *.txt /usr/local/share/enigma/

```
## Wordlists (Dictonary)

```
https://github.com/kkrypt0nn/wordlists.git

```
