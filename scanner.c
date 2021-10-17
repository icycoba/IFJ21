/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Jakub Ryšánek (xrysan05)
 * 
 * Soubor:
 *      scanner.c
 * 
 * Popis:
 *      @brief Modul pro provedení lexikální analýzy
 */

#include "scanner.h"

// TODO ostatní cases, dokončit a otestovat -xhlins01
int getToken(string *attribute){
    // Počáteční stav je S_START
    int state = S_START;

    while(1){
        // Načte znak ze stdin (TODO? -xhlins01)
        char c = fgetc(stdin);

        switch(state){
            case S_START:
                if (isspace(c))                 state = S_START;
                else if (c == '.')              state = S_DOT;
                else if (c == '\"')             state = S_STRSTART;
                else if (c >= '0' && c <= '9')  state = S_INT;
                else if (c == '#')              state = S_LEN;
                else if (c == '-')              state = S_SUB;
                else if (c == '/')              state = S_DIV;
                else if (c == '>')              state = S_GT;
                else if (c == '<')              state = S_LT;
                else if (c == '=')              state = S_ASSIGN;
                else if (c == '~')              state = S_NEQ;

                else if (c == ',')              state = COMMA;    
                else if (c == ':')              state = DOUBLEDOT;
                else if((c >= 'a' && c <= 'z')
                      ||(c >= 'A' && c <= 'Z')) state = ID;
                else if (c == '+')              state = ADD;
                else if (c == '*')              state = MUL;
                else if (c == '(')              state = LBR;
                else if (c == ')')              state = RBR;
                else if (c == '[')              state = LSBR;
                else if (c == ']')              state = RSBR;
                else if (c == '{')              state = LCBR;
                else if (c == '}')              state = RCBR;
            default:
                break;
        }
    }
    return 0;
}