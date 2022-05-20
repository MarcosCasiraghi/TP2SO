//
// Created by Gayba on 5/20/2022.
//

#include "include/kb.h"

#include <kb.h>
#include <naiveConsole.h>

static char scancodeLToAscii[] = {

        0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9',  '0', '-', '=',
        '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',  '[', ']',
        '\n',    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', ''', '`',
        0, '\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',    0, '*',
        0,  ' ',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,
        0,    0,   0,   0, '-',   0,   0,   0, '+',   0,   0,   0,    0,   0,
        0,    0,   0,   0,   0,   0
};

char getKey(){
    char myChar= kbFlag();
    return scancodeLToAscii[myChar];
}