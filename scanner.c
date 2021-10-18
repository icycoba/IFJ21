/**
 * Projekt:
 *      Implementace překladače imperativního jazyka IFJ21
 * 
 * Autoři:
 *      @author Jakub Ryšánek   (xrysan05)
 *      @author Martin Hlinský  (xhlins01)
 * Soubor:
 *      @file scanner.c
 * 
 * Popis:
 *      @brief Modul pro provedení lexikální analýzy
 */

#include "scanner.h"

// TODO ostatní cases, dokončit a otestovat -xhlins01
int getToken(string *attribute){
    // Počáteční stav je S_START
    int state = S_START;

    int line, row = 0;
    while(1){
        // Načte znak ze stdin (TODO? -xhlins01)
        char c = fgetc(stdin);
        row++;
        switch(state){
            case S_START:
                if (isspace(c))                 {state = S_START; if (c == '\n') line++;}
                else if (c == '.')              {state = S_DOT;}
                else if (c == '\"')             {state = S_STRSTART;}
                else if (c >= '0' && c <= '9')  {state = S_INT;}
                else if (c == '-')              {state = S_SUB;}
                else if (c == '/')              {state = S_DIV;}
                else if (c == '>')              {state = S_GT;}
                else if (c == '<')              {state = S_LT;}
                else if (c == '=')              {state = S_ASSIGN;}
                else if (c == '~')              {state = S_NEQ;}

                else if (c == ',')              return COMMA;    
                else if (c == ':')              return DOUBLEDOT;
                else if((c >= 'a' && c <= 'z')
                      ||(c >= 'A' && c <= 'Z')) {state = ID;}
                else if (c == '+')              return ADD;
                else if (c == '*')              return MUL;
                else if (c == '(')              return LBR;
                else if (c == ')')              return RBR;
                else if (c == '[')              return LSBR;
                else if (c == ']')              return RSBR;
                else if (c == '{')              return LCBR;
                else if (c == '}')              return RCBR;
                else if (c == '#')              return LEN;
                break;
            case S_DOT:
                if (c == '.')   return DOT;
                else            errorMessage(ERR_LEXICAL, ("%d:%d Objevil se neočekávaný znak %c\n", line, row, c));
                break;
            case S_STRSTART:
                if (c == '\\')      state = S_STR1;
                else if (c > 31)    state = S_STRSTART; //TODO append to attribute
                else if (c == '\"') return STREND;
                else                errorMessage(ERR_LEXICAL, ("%d:%d Objevil se neočekávaný znak %c\n", line, row, c));
                break;
            case S_STR1:
                //TODO
                break;
            case S_INT:
                if (c >= '0' && c <= '9')       state = S_INT;
                else if (c == '.')              state = S_DOUBLE1;
                else if (c == 'e' || c == 'E')  state = S_EXP1;
                else                            {ungetc(c, stdin); return INT;}
                break;
            case S_DOUBLE1:
                //TODO
                break;
            case S_EXP1:
                //TODO
                break;
            case S_SUB:
                //TODO comment
                break;
            case S_DIV:
                if (c == '/')   return DIV_WHOLE;
                else            {ungetc(c, stdin); return DIV;}
            case S_GT:
                if (c == '=')   return GTE;
                else            {ungetc(c, stdin); return GT;}
            case S_LT:
                if (c == '=')   return LTE;
                else            {ungetc(c, stdin); return LT;}
            case S_ASSIGN:
                if (c == '=')   return EQUAL;
                else            {ungetc(c, stdin); return ASSIGN;}
            case S_NEQ:
                if (c == '=')   return NEQUAL;
                else            errorMessage(ERR_LEXICAL, ("%d:%d Objevil se neočekávaný znak %c\n", line, row, c));
            default:
                break;
        }
    }
    return 0;
}