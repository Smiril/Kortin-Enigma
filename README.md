# Kortin-Enigma
 Enigma Simulation and Turing Machine (Bombe)
### Enigma 0.0.2 - T.E.D - The Enemy Dail - "KOENIG MARTIN" ![alt text](images/Apple_m1.jpeg "MADE with Love for MacOS")


```
* Author: Smiril <sonar@gmx.com>
* Based in the code of:
*  The hashcat Team @hashcat.net
*  capi_x <capi_x@haibane.org>
*  Harald Schmidl <hjschmidl@acm.org>
*  Fauzan Mirza's <fauzan@dcs.rhbnc.ac.uk>

```

## Source

* **[main.c](enigma/src/main.c)** 
* **[ext_metal.m](enigma/src/ext_metal.m)** 
* **[event.c](enigma/src/event.c)** 
* **[thread.c](enigma/src/thread.c)** 
* **[bitmap.c](enigma/src/bitmap.c)** 
* **[add.h](enigma/src/add.h)** 
* **[ext_metal.h](enigma/src/ext_metal.h)** 
* **[event.h](enigma/src/event.h)** 
* **[thread.h](enigma/src/thread.h)**
* **[bitmap.h](enigma/src/bitmap.h)**
* **[types.h](enigma/src/types.h)**
* **[inc_types.h](enigma/src/inc_types.h)**
* **[memory.h](enigma/src/memory.h)**
* **[timer.h](enigma/src/timer.h)**
* **[common.h](enigma/src/common.h)**
* **[air.h](enigma/src/air.h)**

 5 Rotor Enigma Simulation with Bruteforce Message function

 6 versions of one Enigma without Source change

The bombe is based on [known-plaintext attack (KPA)](http://en.wikipedia.org/wiki/Known-plaintext_attack) for this we need a Dictonary.

This attack is assisted for stadistical and heuristics to help the KPA.

## Options

* --option-[1-6]         <--- the main functions
* --option-[1-6][a/b]    <--- Basic Bruteforce
* --help                 <--- Help Page
* --version              <--- The Version

![alt text](images/enigma.jpeg "Enigma")


## Cloning

```
git clone https://github.com/Smiril/Kortin-Enigma.git
```

## Building

```
cd Kortin-Enigma/enigma
bash autoconf.sh
./configure
make
```
## Wordlists (Dictonary)

```
https://github.com/kkrypt0nn/wordlists.git

```
