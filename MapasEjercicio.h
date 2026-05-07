#pragma once

#include <iostream>
#define AnchoM 31
#define AltoM 11


using namespace std;
using namespace System;

void cursor(int x, int y) {
    Console::SetCursorPosition(x, y);
}

int mapaMenu[AltoM][AnchoM] =
{
{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}

};

void coloresMenu(int n) {
    switch (n) {
    case 1:  Console::ForegroundColor = ConsoleColor::White; break;
    case 3:  Console::ForegroundColor = ConsoleColor::Green; break;
    }
}

void DibujarMapaM() {

    for (int i = 0; i < AltoM; i++) {
        for (int j = 0; j < AnchoM; j++) {

            coloresMenu(mapaMenu[i][j]);

            cursor(j, i);

            cout << (char)219;
        }
    }

    Console::ResetColor();
}

void escribirEnMapaM(string texto, int x, int y, int c) {

    switch (c) {
    case 1:
        Console::ForegroundColor = ConsoleColor::Black; break;
    case 2:
        Console::ForegroundColor = ConsoleColor::White; break;
    case 3:
        Console::ForegroundColor = ConsoleColor::Green; break;
    case 4:
        Console::ForegroundColor = ConsoleColor::Red; break;
    }



    for (int i = 0; i < texto.length(); i++) {
        int actualX = x + i;

        if (actualX >= AnchoM || y >= AltoM) break;

        if (mapaMenu[y][actualX] == 3) {
            Console::BackgroundColor = ConsoleColor::Green;
        }
        else if (mapaMenu[y][actualX] == 1) {
            Console::BackgroundColor = ConsoleColor::White;
        }

        cursor(actualX, y);
        cout << texto[i];
    }

    Console::ResetColor();
}
