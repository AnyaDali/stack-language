#pragma once 
#ifndef INITIALIZE_TABLE_H
#define INITIALIZE_TABLE_H
#include "translit.h"
#include "functionSM.h"

// размерность таблицы 
const int state_number = 24; // кол-во состояний   
const int class_number = 11; // кол-во символьных лексем 

//объявление таблицы конечного преобр.
typedef int (*function_pointer)();
function_pointer table_funtion[state_number][class_number];


void initialize_table()
{
    // заполняем таблицу функцией ошибок 
    for (int i = 0; i < state_number; i++)
        for (int j = 0; j < class_number; j++)
            table_funtion[i][j] = Error1;
    
    table_funtion[s_A1][Character] = B1a;
    table_funtion[s_A1][Sign] = C1a;
    table_funtion[s_A1][Attitude] = D1a;
    table_funtion[s_A1][Space] = A1;
    table_funtion[s_A1][LF] = A1b;
    table_funtion[s_A1][Comment] = I1a;

    table_funtion[s_A2][Character] = B1a;
    table_funtion[s_A2][Sign] = C1a;
    table_funtion[s_A2][Attitude] = D1a;
    table_funtion[s_A2][Space] = A2;
    table_funtion[s_A2][LF] = A2a;
    table_funtion[s_A2][Comment] = I2a;
    table_funtion[s_A2][EndOfFile] = Exit1;

    table_funtion[s_B1][Character] = M1;

    table_funtion[s_C1][Space] = C1;
    table_funtion[s_C1][LF] = A2a;
    table_funtion[s_C1][Comment] = I2a;
    table_funtion[s_C1][EndOfFile] = Exit1;

    table_funtion[s_D1][Attitude] = C1h;
    table_funtion[s_D1][Space] = C1g;
    table_funtion[s_D1][LF] = A2e;
    table_funtion[s_D1][Comment] = I2d;
    table_funtion[s_D1][Character] = Error1;
    table_funtion[s_D1][EndOfFile] = Exit1;

    table_funtion[s_E1][Character] = Error1;
    table_funtion[s_E1][Space] = F1;

    table_funtion[s_E2][Space] = F2;

    table_funtion[s_E3][Space] = F3;

    table_funtion[s_F1][Character] = H1a;
    table_funtion[s_F1][Digit] = G1a;
    table_funtion[s_F1][Space] = F1;
    table_funtion[s_F1][LBracket] = P1;

    table_funtion[s_F2][Digit] = G1a;
    table_funtion[s_F2][Space] = F2;

    table_funtion[s_F3][Character] = H1a;
    table_funtion[s_F3][Space] = F3;

    table_funtion[s_G1][Digit] = G1b;
    table_funtion[s_G1][Space] = C1e;
    table_funtion[s_G1][LF] = A2c;
    table_funtion[s_G1][Comment] = I2b;
    table_funtion[s_G1][EndOfFile] = Exit2;

    table_funtion[s_H1][Character] = H1b;
    table_funtion[s_H1][Digit] = H1b;
    table_funtion[s_H1][Space] = C1f;
    table_funtion[s_H1][LF] = A2d;
    table_funtion[s_H1][Comment] = I2c;
    table_funtion[s_H1][EndOfFile] = Exit3;

    table_funtion[s_I1][Character] = I1;
    table_funtion[s_I1][Digit] = I1;
    table_funtion[s_I1][Sign] = I1;
    table_funtion[s_I1][Attitude] = I1;
    table_funtion[s_I1][Space] = I1;
    table_funtion[s_I1][LF] = A1a;
    table_funtion[s_I1][Comment] = I1;
    table_funtion[s_I1][Comma] = I1;
    table_funtion[s_I1][RBracket] = I1;
    table_funtion[s_I1][LBracket] = I1;
    table_funtion[s_I1][Comment] = I1;
    table_funtion[s_I1][Error] = I1;

    table_funtion[s_I2][Character] = I2;
    table_funtion[s_I2][Digit] = I2;
    table_funtion[s_I2][Sign] = I2;
    table_funtion[s_I2][Attitude] = I2;
    table_funtion[s_I2][Space] = I2;
    table_funtion[s_I2][LF] = A2a;
    table_funtion[s_I2][Comma] = I2;
    table_funtion[s_I2][RBracket] = I2;
    table_funtion[s_I2][LBracket] = I2;
    table_funtion[s_I2][Comment] = I2;
    table_funtion[s_I2][Error] = I2;
    table_funtion[s_I2][EndOfFile] = Exit1;

    table_funtion[s_J1][Character] = J1;
    table_funtion[s_J1][Digit] = J1;
    table_funtion[s_J1][Sign] = J1;
    table_funtion[s_J1][Attitude] = J1;
    table_funtion[s_J1][Space] = J1;
    table_funtion[s_J1][LF] = A2a;
    table_funtion[s_J1][Comment] = J1;
    table_funtion[s_J1][Comma] = J1;
    table_funtion[s_J1][RBracket] = J1;
    table_funtion[s_J1][LBracket] = J1;
    table_funtion[s_J1][Error] = J1;
    table_funtion[s_J1][EndOfFile] = Exit1;

    table_funtion[s_P1][Digit] = Q1a;

    table_funtion[s_Q1][RBracket] = S1a;
    table_funtion[s_Q1][Digit] = Q1b;
    table_funtion[s_Q1][Comma] = Q2;

    table_funtion[s_Q2][Digit] = Q1a;

    table_funtion[s_S1][Space] = S1b;
    table_funtion[s_S1][LF] = A2a;
}
#endif