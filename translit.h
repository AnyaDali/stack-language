#pragma once 
#ifndef TRANSLIT_H
#include <iostream>

enum SymbolicTokenClass
{
    Character,
    Digit,
    Sign,
    Attitude,
    Space,
    LF,
    Comment,
    Comma,
    LBracket,
    RBracket,
    Error,
    EndOfFile
};

struct SymbolicToken
{
    SymbolicTokenClass token_class; 
    int value;                   
};

bool flag_end = 0;

SymbolicToken transliterator(int ch)
{
    SymbolicToken s;
    s.value = 0;

    if (ch == EOF)
    {
        flag_end = 1;
        s.token_class = EndOfFile;
        return s;
    }
    else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
    {
        s.token_class = Character;
        s.value = ch;
    }
    else if (ch >= '0' && ch <= '9')
    {
        s.token_class = Digit;
        s.value = ch - '0';
    }
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
    {
        s.token_class = Sign;
        s.value = ch;
    }
    else if (ch == '=' || ch == '>' || ch == '<' || ch == '!')
    {
        s.token_class = Attitude;
        s.value = ch;
    }
    else if (ch == ';')
    {
        s.token_class = Comment;
        s.value = ch;
    }
    else if (ch == ',')
    {
        s.token_class = Comma;
        s.value = ch;
    }
    else if (ch == '[')
    {
        s.token_class = LBracket;
        s.value = ch;
    }
    else if (ch == ']')
    {
        s.token_class = RBracket;
        s.value = ch;
    }
    else if (ch == '\n')
    {
        s.value = ch;
        s.token_class = LF;
    }
    else if (ch == ' ' || ch == '\t')
    {
        s.value = ch;
        s.token_class = Space;
    }
    else
    {
        s.token_class = Error;
    }
    return s;
}



#endif