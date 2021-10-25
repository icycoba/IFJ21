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

unsigned int line, col;

/**
 * @brief Funkce, která znak po znaku zpracovává standartní vstup na token.
 * 
 * @param attribute Parametr, do kterého se ukládají hodnoty pro další analýzu.
*/
int getToken(string *attribute){
    // Počáteční stav je S_START
    int state = S_START;

    char *eMessage;
    while(1){
        char c = fgetc(stdin);
        col++;
        switch(state){
            case S_START:
                if (isspace(c))                 {state = S_START; col--; if (c == '\n') {line++; col = 0;}}
                else if (c == '.')              {state = S_DOT;}
                else if (c == '\"')             {state = S_STRSTART; strAddChar(attribute, c);}
                else if (c >= '1' && c <= '9')  {state = S_INT; strAddChar(attribute, c);}
                else if (c == '0')              {state = S_ZERO; strAddChar(attribute, c);}
                else if (c == '-')              {state = S_SUB;}
                else if (c == '/')              {state = S_DIV;}
                else if (c == '>')              {state = S_GT;}
                else if (c == '<')              {state = S_LT;}
                else if (c == '=')              {state = S_ASSIGN;}
                else if (c == '~')              {state = S_NEQ;}
                else if (isalpha(c))            {state = S_ID; strAddChar(attribute, c);}

                else if (c == ',')              return COMMA;    
                else if (c == ':')              return DOUBLEDOT;
                else if (c == '+')              return ADD;
                else if (c == '*')              return MUL;
                else if (c == '(')              return LBR;
                else if (c == ')')              return RBR;
                else if (c == '[')              return LSBR;
                else if (c == ']')              return RSBR;
                else if (c == '{')              return LCBR;
                else if (c == '}')              return RCBR;
                else if (c == '#')              return LEN;
                else if (c == EOF)              return EOFILE;
                else                            return UNKNOWN;
                break;
            case S_DOT:
                col++;
                if (c == '.')   return CONCAT;
                else            {ungetc(c, stdin); return DOT;}
                break;
            case S_STRSTART:
                col++;
                if (c == '\\')                  {state = S_STR1; strAddChar(attribute, c);}
                else if (c > 31 && c != '\"')   {state = S_STRSTART; strAddChar(attribute, c);}
                else if (c == '\"')             {state = STREND; strAddChar(attribute, c); return STREND;}
                else                            {sprintf(eMessage, "[%d: %d] Objevil se neočekávaný znak %c", line, col, c); errorMessage(ERR_LEXICAL, eMessage);}
                break;
            case S_STR1:
                col++;
                if (c == '\\' || c == '\"'
                 || c == 't'  || c == 'n')      {state = S_STRSTART; strAddChar(attribute, c);}
                else if (c >= 0 && c <= 2)      {state = S_STR2; strAddChar(attribute, c);}
                else                            {sprintf(eMessage, "[%d: %d] Objevil se neočekávaný znak %c", line, col, c); errorMessage(ERR_LEXICAL, eMessage);}
                break;
            case S_STR2:
                col++;
                if (c >= 0 && c <= 5)           {state = S_STR3; strAddChar(attribute, c);}
                else                            {sprintf(eMessage, "[%d: %d] Objevil se neočekávaný znak %c", line, col, c); errorMessage(ERR_LEXICAL, eMessage);}
                break;
            case S_STR3:
                col++;
                if (c >= 1 && c <= 5)           {state = S_STRSTART; strAddChar(attribute, c);}
                else                            {sprintf(eMessage, "[%d: %d] Objevil se neočekávaný znak %c", line, col, c); errorMessage(ERR_LEXICAL, eMessage);}
                break;
            case S_INT:
                col++;
                if (c >= '0' && c <= '9')       {state = S_INT; strAddChar(attribute, c);}
                else if (c == '.')              {state = S_DOUBLE1; strAddChar(attribute, c);}
                else if (c == 'e' || c == 'E')  {state = S_EXP1; strAddChar(attribute, c);}
                else                            {ungetc(c, stdin); return INT;}
                break;
            case S_ZERO:
                col++;
                if (c == '.')                   {state = S_DOUBLE1; strAddChar(attribute, c);}
                else if (c == 'e' || c == 'E')  {state = S_EXP1; strAddChar(attribute, c);}
                else                            {ungetc(c, stdin); return ZERO;} // Naimplementovat jako lexikální, nebo syntaktickou chybu? -xhlins01
                break;
            case S_DOUBLE1:
                col++;
                if (c >= '0' && c <= '9')       {state = S_DOUBLE; strAddChar(attribute, c);}
                else                            {sprintf(eMessage, "[%d: %d] Objevil se neočekávaný znak %c", line, col, c); errorMessage(ERR_LEXICAL, eMessage);}
                break;
            case S_DOUBLE:
                col++;
                if (c >= '0' && c <= '9')       {state = S_DOUBLE; strAddChar(attribute, c);}
                if (c == 'e' || c == 'E')       {state = S_EXP1; strAddChar(attribute, c);}
                else                            {ungetc(c, stdin); return DOUBLE;}
                break;
            case S_EXP1:
                col++;
                if (c == '+' || c == '-')       {state = S_EXP2; strAddChar(attribute, c);}
                else if (c >= '0' && c <= '9')  {state = S_EXP; strAddChar(attribute, c);}
                else                            {sprintf(eMessage, "[%d: %d] Objevil se neočekávaný znak %c", line, col, c); errorMessage(ERR_LEXICAL, eMessage);}
                break;
            case S_EXP2:
                col++;
                if (c >= '0' && c <= '9')       {state = S_EXP; strAddChar(attribute, c);}
                else                            {sprintf(eMessage, "[%d: %d] Objevil se neočekávaný znak %c", line, col, c); errorMessage(ERR_LEXICAL, eMessage);}
                break;
            case S_EXP:
                col++;
                if (c >= '0' && c <= '9')       {state = S_EXP; strAddChar(attribute, c);}
                else                            {ungetc(c, stdin); return EXP;}
            case S_ID:
                col++;
                if  (isalnum(c))                {state = S_ID; strAddChar(attribute, c);}
                else                            {ungetc(c, stdin); state = S_KW;}
                break;
            case S_KW:
                ungetc(c, stdin);
                if (strCmpConstStr(attribute, "do") == 0)               return KW_DO;
                else if (strCmpConstStr(attribute, "else") == 0)        return KW_ELSE;
                else if (strCmpConstStr(attribute, "end") == 0)         return KW_END;
                else if (strCmpConstStr(attribute, "function") == 0)    return KW_FUNC;
                else if (strCmpConstStr(attribute, "global") == 0)      return KW_GLOBAL;
                else if (strCmpConstStr(attribute, "if") == 0)          return KW_IF;
                else if (strCmpConstStr(attribute, "integer") == 0)     return KW_INT;
                else if (strCmpConstStr(attribute, "local") == 0)       return KW_LOCAL;
                else if (strCmpConstStr(attribute, "nil") == 0)         return KW_NIL;
                else if (strCmpConstStr(attribute, "number") == 0)      return KW_NUM;
                else if (strCmpConstStr(attribute, "require") == 0)     return KW_REQUIRE;
                else if (strCmpConstStr(attribute, "return") == 0)      return KW_RETURN;
                else if (strCmpConstStr(attribute, "string") == 0)      return KW_STR;
                else if (strCmpConstStr(attribute, "then") == 0)        return KW_THEN;
                else if (strCmpConstStr(attribute, "while") == 0)       return KW_WHILE;
                else return ID;
                break;
            case S_SUB:
                col++;
                if (c == '-')                   state = S_COMM_LINE;
                else                            {ungetc(c, stdin); return SUB;}
                break;
            case S_COMM_LINE:
                col++;
                if (c == '\n')                  return COMM_LINE_END;
                else if (c == '[')              state = S_BLOCK1;
                else                            state = S_COMM_LINE;
                break;
            case S_BLOCK1:
                col++;
                if (c == '\n')                  return COMM_LINE_END;
                else if (c == '[')              state = S_BLOCK;
                else                            state = S_COMM_LINE;
                break;
            case S_BLOCK:
                col++;
                if (c == ']')                   state = S_BLOCK_END1;
                else                            state = S_BLOCK;
                break;
            case S_BLOCK_END1:
                col++;
                if (c == ']')                   return BLOCK_END;
                else                            state = S_BLOCK;
                break;
            case S_DIV:
                col++;
                if (c == '/')                   return DIV_WHOLE;
                else                            {ungetc(c, stdin); return DIV;}
                break;
            case S_GT:
                col++;
                if (c == '=')                   return GTE;
                else                            {ungetc(c, stdin); return GT;}
                break;
            case S_LT:
                col++;
                if (c == '=')                   return LTE;
                else                            {ungetc(c, stdin); return LT;}
                break;
            case S_ASSIGN:
                col++;
                if (c == '=')                   return EQUAL;
                else                            {ungetc(c, stdin); return ASSIGN;}
                break;
            case S_NEQ:
                col++;
                if (c == '=')                   return NEQUAL;
                else                            {sprintf(eMessage, "[%d: %d] Objevil se neočekávaný znak %c", line, col, c); errorMessage(ERR_LEXICAL, eMessage);}
                break;
            default:
                break;
        }
    }
    return 0;
}

/**
 * @brief Funkce, která vypisuje všechny tokeny podle enum ve scanner.h
 * 
 * @param state Stav, ve kterém se momentálně nachází program.
*/
const char *printState(int state){
    switch(state){
    // Nekoncové stavy
    case S_START:
        return "S_START";
        break;
    case S_DOT:
        return "S_DOT";
        break;
    case S_STRSTART:
        return "S_STRSTART";
        break;
    case S_STR1:
        return "S_STR1";
        break;
    case S_STR2:
        return "S_STR2";
        break;
    case S_STR3:
        return "S_STR3";
        break;
    case S_INT:
        return "S_INT";
        break;
    case S_DOUBLE1:
        return "S_DOUBLE1";
        break;
    case S_DOUBLE:
        return "S_DOBULE";
        break;
    case S_EXP1:
        return "S_EXP1";
        break;
    case S_EXP2:
        return "S_EXP2";
        break;
    case S_EXP:
        return "S_EXP";
        break;
    case S_ID:
        return "S_ID";
        break;
    case S_SUB:
        return "S_SUB";
        break;
    case S_DIV:
        return "S_DIV";
        break;
    case S_GT:
        return "S_GT";
        break;
    case S_LT:
        return "S_LT";
        break;
    case S_ASSIGN:
        return "S_ASSIGN";
        break;
    case S_NEQ:
        return "S_NEQ";
        break;
    case S_COMM_LINE:
        return "S_COMM_LINE";
        break;
    case S_BLOCK1:
        return "S_BLOCK1";
        break;
    case S_BLOCK:
        return "S_BLOCK";
        break;
    case S_BLOCK_END1:
        return "S_BLOCK_END1";
        break;
    case S_ZERO:
        return "S_ZERO";
        break;
    case S_KW:
        return "S_KW";
        break;

    //Koncové stavy
    case COMMA:
        return "COMMA";
        break;
    case DOT:
        return "DOT";
        break;
    case CONCAT:
        return "CONCAT";
        break;
    case DOUBLEDOT:
        return "DOUBLEDOT";
        break;
    case STREND:
        return "STREND";
        break;
    case INT:
        return "INT";
        break;
    case DOUBLE:
        return "DOUBLE";
        break;
    case EXP:
        return "EXP";
        break;
    case ID:
        return "ID";
        break;
    case ADD:
        return "ADD";
        break;
    case SUB:
        return "SUB";
        break;
    case MUL:
        return "MUL";
        break;
    case DIV:
        return "DIV";
        break;
    case DIV_WHOLE:
        return "DIV_WHOLE";
        break;
    case GT:
        return "GT";
        break;
    case GTE:
        return "GTE";
        break;
    case LT:
        return "LT";
        break;
    case LTE:
        return "LTE";
        break;
    case ASSIGN:
        return "ASSIGN";
        break;
    case EQUAL:
        return "EQUAL";
        break;
    case NEQUAL:
        return "NEQUAL";
        break;
    case LBR:
        return "LBR";
        break;
    case RBR:
        return "RBR";
        break;
    case LSBR:
        return "LSBR";
        break;
    case RSBR:
        return "RSBR";
        break;
    case LCBR:
        return "LCBR";
        break;
    case RCBR:
        return "RCBR";
        break;
    case LEN:
        return "LEN";
        break;
    case COMM_LINE_END:
        return "COMM_LINE_END";
        break;
    case BLOCK_END:
        return "BLOCK_END";
        break;
    case ZERO:
        return "BLOCK_END";
        break;
    case EOFILE:
        return "EOF";
        break;

    //Klíčová slova
    case KW_DO:
        return "KW_DO";
        break;
    case KW_ELSE:
        return "KW_ELSE";
        break;
    case KW_END:
        return "KW_END";
        break;
    case KW_FUNC:
        return "KW_FUNC";
        break;
    case KW_GLOBAL:
        return "KW_GLOBAL";
        break;
    case KW_IF:
        return "KW_IF";
        break;
    case KW_INT:
        return "KW_INT";
        break;
    case KW_LOCAL:
        return "KW_LOCAL";
        break;
    case KW_NIL:
        return "KW_NIL";
        break;
    case KW_NUM:
        return "KW_NUM";
        break;
    case KW_REQUIRE:
        return "KW_REQUIRE";
        break;
    case KW_RETURN:
        return "KW_RETURN";
        break;
    case KW_STR:
        return "KW_STR";
        break;
    case KW_THEN:
        return "KW_THEN";
        break;
    case KW_WHILE:
        return "KW_WHILE";
        break;
    default:
        return "UNKNOWN STATE";
        break;
    }
}