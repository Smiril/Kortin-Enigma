# Kortin-Enigma
 Enigma Simulation and Turing Machine (Bombe)
### Enigma 0.0.3 - T.E.D - The Enemy Dail - "KOENIG MARTIN"

<p align="center">
  <a href="//github.com/Smiril/Kortin-Enigma"><img src="https://img.shields.io/github/repo-size/Smiril/Kortin-Enigma"></a>
  <a href="//github.com/Smiril/Kortin-Enigma/commits"><img src="https://img.shields.io/github/last-commit/Smiril/Kortin-Enigma"></a>
  <a href="//github.com/Smiril/Kortin-Enigma/contributors"><img src="https://img.shields.io/github/contributors/Smiril/Kortin-Enigma"></a>
  <a href="https://github.com/Smiril/Kortin-Enigma/actions/workflows/build.yml"><img src="https://github.com/Smiril/Kortin-Enigma/actions/workflows/build.yml/badge.svg"></a>
</p>

```
* Author: Smiril <sonar@gmx.com>
* Based in the code of:
*  Hashcat Team @hashcat.net
*  capi_x <capi_x@haibane.org>
*  Harald Schmidl <hjschmidl@acm.org>
*  Fauzan Mirza's <fauzan@dcs.rhbnc.ac.uk>

```

 5 Rotor Enigma Simulation with Bruteforce Message function

 different versions of one Enigma without Source change

The bombe is based on [known-plaintext attack (KPA)](http://en.wikipedia.org/wiki/Known-plaintext_attack) for this we need a Dictonary.

This attack is assisted for stadistical and heuristics to help the KPA.

## Options

* --option-[1-3]                 <--- the main functions
* --option-[1-3][a/b] -T[8-64] -D[1/2]    <--- Basic Bruteforce
* --help                         <--- Help Page
* --version                      <--- The Version

![alt text](images/enigma.jpeg "Enigma")


## Cloning

```
git clone https://github.com/Smiril/Kortin-Enigma.git
```

## Building
first ...
```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)
brew install autoconf@2.69
brew install clang
brew install gcc
brew install cc
brew install clang++
brew install g++
brew install c++
xcode-select --install

```
then ...
```
cd Kortin-Enigma/enigma_v3
./autoconf.sh
./configure
make
```
## Wordlists (Dictonary)

```
https://github.com/kkrypt0nn/wordlists.git

```
