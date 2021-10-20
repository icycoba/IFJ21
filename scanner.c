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
int getToken(/*string *attribute*/){
    // Počáteční stav je S_START
    int state = S_START;

    int i = 0;

    int line, row = 0;
    while(1){
        // Načte znak ze stdin (TODO? -xhlins01)
        i++;
        char c = fgetc(stdin);
        //printf("%c\n", c);
        row++;
        printf("%c: %s\n", c, printState(state));
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
                else            errorMessage(ERR_LEXICAL, ("Objevil se neočekávaný znak\n"));
                break;
            case S_STRSTART:
                if (c == '\\')      state = S_STR1;
                else if (c > 31 && c != '\"')    state = S_STRSTART; //TODO append to attribute
                else if (c == '\"') {state = STREND; return STREND;}
                else                errorMessage(ERR_LEXICAL, ("Objevil se neočekávaný znak\n"));
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
                state = S_START;
                break;
            case S_EXP1:
                //TODO
                state = S_START;
                break;
            case ID:
                //TODO
                state = S_START;
                break;
            case S_SUB:
                if (c == '-')   state = S_COMM_LINE;
                else            {ungetc(c, stdin); return SUB;}
                break;
            case S_COMM_LINE:
                if (c == '\n')      return COMM_LINE_END;
                else if (c == '[')  state = S_BLOCK1;
                else                state = S_COMM_LINE;
                break;
            case S_BLOCK1:
                if (c == '\n')      return COMM_LINE_END;
                else if (c == '[')  state = S_BLOCK;
                else                state = S_COMM_LINE;
                break;
            case S_BLOCK:
                if (c == ']')       state = S_BLOCK_END1;
                else                state = S_BLOCK;
                break;
            case S_BLOCK_END1:
                if (c == ']')       return BLOCK_END;
                else                state = S_BLOCK;
                break;
            case S_DIV:
                if (c == '/')   return DIV_WHOLE;
                else            {ungetc(c, stdin); return DIV;}
                break;
            case S_GT:
                if (c == '=')   return GTE;
                else            {ungetc(c, stdin); return GT;}
                break;
            case S_LT:
                if (c == '=')   return LTE;
                else            {ungetc(c, stdin); return LT;}
                break;
            case S_ASSIGN:
                if (c == '=')   return EQUAL;
                else            {ungetc(c, stdin); return ASSIGN;}
                break;
            case S_NEQ:
                if (c == '=')   return NEQUAL;
                else            errorMessage(ERR_LEXICAL, ("Objevil se neočekávaný znak\n"));
                break;
            default:
                break;
        }
        
    }
    return 0;
}

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