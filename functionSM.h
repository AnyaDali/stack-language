#pragma once 
#ifndef FUNCTION_SM_H
#define FUNCTION_SM_H
#include <vector>
#include <string>
#include "hash_mset.h"
using namespace std;

struct classToken
{
    int class_register; // ������� ������
    int type;           //0 - �����, 1 - ������
    hash_set li;
    int const_flag; // ���� ���������: 1 - ���������, 0 - ���������� 3 - ���������� ��������� � ���������
    string name;    // ��� ����������
    int value;      // �������� ���������
    int str;        // ������ ����� ������� ������ � ���������
};

SymbolicToken s;        
int class_register;     
int number_register;    // регистр числа для составления константы   
int relat_register;      
int str_register = 1;   //регистр строки 
int discovery_register; //регистр распознавания ключевых слов 
bool flag_Error = 0;    
string name; // регистр названия переменнй

vector<classToken> obj;

enum ProcedureTokenClass
{
    OPERATION,
    ATTITUDE,
    PUSH,
    POP,
    READ,
    WRITE,
    JI,
    JMP,
    END,
    LIREAD,
    UNION,
    INTERSEC,
    DIFF,
    SYMM,
    ADD,
    ITEM,
    POWER,
    ERROR,
    COMMENT
};

enum States
{
    s_A1,
    s_A2,
    s_B1,
    s_C1,
    s_D1,
    s_E1,
    s_E2,
    s_E3,
    s_F1,
    s_F2,
    s_F3,
    s_F4,
    s_G1,
    s_H1,
    s_I1,
    s_I2,
    s_J1,
    s_M1,
    s_P1,
    s_P2,
    s_Q1,
    s_Q2,
    s_S1,
    s_Error,
    s_Stop
};

enum COMP
{
    LESS,
    MORE,
    LESS_EQUAL,
    MORE_EQUAL,
    EQUAL,
    INEQUAL
};

int Error1();

int A1()
{
    return s_A1;
}

int A1a()
{
    str_register += 1;
    return s_A1;
}

int A1b()
{
    str_register += 1;
    return s_A1;
}

int A2()
{
    return s_A2;
}

int A2a()
{
    str_register += 1;
    return s_A2;
}

int A2c()
{
    (obj.end() - 1)->value = number_register;
    str_register += 1;
    return s_A2;
}

int A2d()
{
    std::cout << "A2d" << std::endl;
    (obj.end() - 1)->name = name;
    if (!name.empty())
        name.clear();
    str_register += 1;
    return s_A2;
}

int A2e()
{
    if (relat_register == 1)
        (obj.end() - 1)->value = LESS;
    else if (relat_register == 2)
        (obj.end() - 1)->value = MORE;
    else if (relat_register == 3)
        (obj.end() - 1)->value = EQUAL;
    else
        Error1();

    str_register += 1;
    return s_A2;
}

int B1()
{
    return s_B1;
}

// функция распознавания ключевых слов 
int B1a()
{
    discovery_register = 0;
    char c = s.value;
    switch (c)
    {
    case 'a':
        discovery_register = 27;
        break;
    case 'd':
        discovery_register = 29;
        break;
    case 'e':
        discovery_register = 1;
        break;
    case 'i':
        discovery_register = 39;
        break;
    case 'j':
        discovery_register = 3;
        break;
    case 'l':
        discovery_register = 18;
        break;
    case 'p':
        discovery_register = 6;
        break;
    case 'r':
        discovery_register = 11;
        break;
    case 's':
        discovery_register = 32;
        break;
    case 'u':
        discovery_register = 35;
        break;
    case 'w':
        discovery_register = 14;
        break;
    }
    if (discovery_register == 0)
        return Error1();
    else
        return s_B1;
}

int B1b()
{
    discovery_register += 1;
    return s_B1;
}

int C1()
{
    return s_C1;
}

int C1a()
{
    classToken t;
    t.const_flag = 3;
    obj.push_back(t);
    (obj.end() - 1)->class_register = OPERATION; //
    (obj.end() - 1)->value = s.value;            //
    (obj.end() - 1)->str = str_register;         //
    return s_C1;
}

int C1b()
{
    classToken k;
    k.const_flag = 3;
    obj.push_back(k);
    (obj.end() - 1)->str = str_register;
    (obj.end() - 1)->class_register = END;
    return s_C1;
}

int C1c()
{
    classToken k;
    k.const_flag = 3;
    k.type = 0;
    obj.push_back(k);
    (obj.end() - 1)->str = str_register;
    (obj.end() - 1)->class_register = READ;
    return s_C1;
}

int C1d()
{
    classToken k;
    k.const_flag = 3;
    obj.push_back(k);
    (obj.end() - 1)->str = str_register;
    (obj.end() - 1)->class_register = WRITE;
    return s_C1;
}

int C1e()
{
    (obj.end() - 1)->value = number_register;
    return s_C1;
}

int C1f()
{
    (obj.end() - 1)->name = name;
    if (!name.empty())
        name.clear();
    return s_C1;
}

int C1g()
{
    if (relat_register == 1)
        (obj.end() - 1)->value = LESS;
    else if (relat_register == 2)
        (obj.end() - 1)->value = MORE;
    else if (relat_register == 3)
        (obj.end() - 1)->value = EQUAL;
    else
        Error1();

    return s_C1;
}

int C1h()
{
    char cur_ch = s.value;
    if (relat_register == 1 && cur_ch == '=')
        (obj.end() - 1)->value = LESS_EQUAL;
    else if (relat_register == 2 && cur_ch == '=')
        (obj.end() - 1)->value = MORE_EQUAL;
    else if (relat_register == 4 && cur_ch == '=')
        (obj.end() - 1)->value = INEQUAL;
    else
        return Error1();

    return s_C1;
}

int C1i()
{
    classToken k;
    k.const_flag = 3;
    k.type = 1;
    obj.push_back(k);
    (obj.end() - 1)->class_register = LIREAD;
    (obj.end() - 1)->str = str_register;
    return s_C1;
}

int C1k()
{
    classToken k;
    k.const_flag = 3;
    k.type = 1;
    obj.push_back(k);
    (obj.end() - 1)->class_register = ADD;
    (obj.end() - 1)->str = str_register;
    return s_C1;
}

int C1l()
{
    classToken k;
    k.const_flag = 3;
    k.type = 1;
    obj.push_back(k);
    (obj.end() - 1)->class_register = DIFF;
    (obj.end() - 1)->str = str_register;
    return s_C1;
}

int C1m()
{
    classToken k;
    k.const_flag = 3;
    k.type = 1;
    obj.push_back(k);
    (obj.end() - 1)->class_register = SYMM;
    (obj.end() - 1)->str = str_register;
    return s_C1;
}

int C1n()
{
    classToken k;
    k.const_flag = 3;
    k.type = 1;
    obj.push_back(k);
    (obj.end() - 1)->class_register = UNION;
    (obj.end() - 1)->str = str_register;
    return s_C1;
}

int C1o()
{
    classToken k;
    k.const_flag = 3;
    k.type = 1;
    obj.push_back(k);
    (obj.end() - 1)->class_register = INTERSEC;
    (obj.end() - 1)->str = str_register;
    return s_C1;
}

int C1p()
{
    classToken k;
    k.const_flag = 3;
    k.type = 1;
    obj.push_back(k);
    (obj.end() - 1)->class_register = POWER;
    (obj.end() - 1)->str = str_register;
    return s_C1;
}

int D1a()
{
    classToken t;
    t.const_flag = 3;
    obj.push_back(t);
    (obj.end() - 1)->class_register = ATTITUDE;
    (obj.end() - 1)->str = str_register;
    switch (s.value)
    {
    case '<':
        relat_register = 1;
        break;
    case '>':
        relat_register = 2;
        break;
    case '=':
        relat_register = 3;
        break;
    case '!':
        relat_register = 4;
        break;
    }
    return s_D1;
}

int E1a()
{
    classToken k;
    obj.push_back(k);
    (obj.end() - 1)->class_register = PUSH;
    (obj.end() - 1)->str = str_register;
    return s_E1;
}

int E1c()
{
    classToken k;
    k.type = 1;
    obj.push_back(k);
    (obj.end() - 1)->str = str_register;
    (obj.end() - 1)->class_register = ITEM;
    return s_E1;
}

int E2a()
{
    classToken k;
    k.const_flag = 1;
    obj.push_back(k);
    (obj.end() - 1)->str = str_register;
    (obj.end() - 1)->class_register = JI;
    return s_E2;
}

int E2b()
{
    classToken k;
    k.const_flag = 1;
    obj.push_back(k);
    (obj.end() - 1)->str = str_register;
    (obj.end() - 1)->class_register = JMP;
    return s_E2;
}

int E3a()
{
    classToken k;
    obj.push_back(k);
    (obj.end() - 1)->class_register = POP;
    (obj.end() - 1)->str = str_register;
    return s_E3;
}

int F1()
{
    return s_F1;
}

int F2()
{
    return s_F2;
}

int F3()
{
    return s_F3;
}

int G1a()
{
    (obj.end() - 1)->type = 0;
    (obj.end() - 1)->const_flag = 1;
    number_register = s.value;
    return s_G1;
}

int G1b()
{
    number_register = number_register * 10 + s.value;
    return s_G1;
}

int H1a()
{
    (obj.end() - 1)->const_flag = 0;
    name += s.value;
    return s_H1;
}

int H1b()
{
    char c = s.value;
    name += c;
    return s_H1;
}

int I1()
{
    return s_I1;
}

int I1a()
{
    return s_I1;
}

int I2()
{
    return s_I2;
}

int I2a()
{
    classToken k;
    k.const_flag = 3;
    obj.push_back(k);
    (obj.end() - 1)->class_register = COMMENT;
    (obj.end() - 1)->str = str_register;
    return s_I2;
}

int I2b()
{
    (obj.end() - 1)->value = number_register;

    classToken k;
    k.const_flag = 3;
    obj.push_back(k);
    (obj.end() - 1)->class_register = COMMENT;
    (obj.end() - 1)->str = str_register;
    return s_I2;
}

int I2c()
{
    (obj.end() - 1)->name = name;
    if (!name.empty())
        name.clear();

    classToken k;
    obj.push_back(k);
    (obj.end() - 1)->class_register = COMMENT;
    (obj.end() - 1)->str = str_register;
    return s_I2;
}

int I2d()
{
    if (relat_register == 1)
        (obj.end() - 1)->value = LESS;
    else if (relat_register == 2)
        (obj.end() - 1)->value = MORE;
    else if (relat_register == 3)
        (obj.end() - 1)->value = EQUAL;

    classToken k;
    obj.push_back(k);
    (obj.end() - 1)->class_register = COMMENT;
    (obj.end() - 1)->str = str_register;
    return s_I2;
}

int J1()
{
    if (s.token_class == LF)
        return A2a();
    return s_J1;
}


// final
int M1()
{
    if (discovery_register == 1 && s.value == 'n')
        return B1b();
    else if (discovery_register == 2 && s.value == 'd')
        return C1b();
    else if (discovery_register == 3 && s.value == 'i')
        return E2a();
    else if (discovery_register == 4 && s.value == 'm')
        return B1b();
    else if (discovery_register == 5 && s.value == 'p')
        return E2b();
    else if (discovery_register == 6 && s.value == 'o')
        return B1b();
    else if (discovery_register == 7 && s.value == 'p')
        return E3a();
    else if (discovery_register == 8 && s.value == 'u')
        return B1b();
    else if (discovery_register == 9 && s.value == 's')
        return B1b();
    else if (discovery_register == 10 && s.value == 'h')
        return E1a();
    else if (discovery_register == 11 && s.value == 'e')
        return B1b();
    else if (discovery_register == 12 && s.value == 'a')
        return B1b();
    else if (discovery_register == 13 && s.value == 'd')
        return C1c();
    else if (discovery_register == 14 && s.value == 'r')
        return B1b();
    else if (discovery_register == 15 && s.value == 'i')
        return B1b();
    else if (discovery_register == 16 && s.value == 't')
        return B1b();
    else if (discovery_register == 17 && s.value == 'e')
        return C1d();
    else if (discovery_register == 18 && s.value == 'i')
        return B1b();
    else if (discovery_register == 19 && s.value == 'r')
        return B1b();
    else if (discovery_register == 20 && s.value == 'e')
        return B1b();
    else if (discovery_register == 21 && s.value == 'a')
        return B1b();
    else if (discovery_register == 22 && s.value == 'd')
        return C1i();
    else if (discovery_register == 27 && s.value == 'd')
        return B1b();
    else if (discovery_register == 28 && s.value == 'd')
        return C1k();
    else if (discovery_register == 29 && s.value == 'i')
        return B1b();
    else if (discovery_register == 30 && s.value == 'f')
        return B1b();
    else if (discovery_register == 31 && s.value == 'f')
        return C1l();
    else if (discovery_register == 32 && s.value == 'y')
        return B1b();
    else if (discovery_register == 33 && s.value == 'm')
        return B1b();
    else if (discovery_register == 34 && s.value == 'm')
        return C1m();
    else if (discovery_register == 35 && s.value == 'n')
        return B1b();
    else if (discovery_register == 36 && s.value == 'i')
        return B1b();
    else if (discovery_register == 37 && s.value == 'o')
        return B1b();
    else if (discovery_register == 38 && s.value == 'n')
        return C1n();
    else if (discovery_register == 39 && s.value == 'n')
        return B1b();
    else if (discovery_register == 40 && s.value == 't')
        return B1b();
    else if (discovery_register == 41 && s.value == 'e')
        return B1b();
    else if (discovery_register == 42 && s.value == 'r')
        return B1b();
    else if (discovery_register == 43 && s.value == 's')
        return B1b();
    else if (discovery_register == 44 && s.value == 'e')
        return B1b();
    else if (discovery_register == 45 && s.value == 'c')
        return C1o();
    else if (discovery_register == 48 && s.value == 'e')
        return B1b();
    else if (discovery_register == 49 && s.value == 'm')
        return E1c();
    else if (discovery_register == 50 && s.value == 'e')
        return B1b();
    else if (discovery_register == 51 && s.value == 'r')
        return C1p();
    else if (discovery_register == 7 && s.value == 'w')
    {
        discovery_register = 49;
        return B1b();
    }
    else if (discovery_register == 39 && s.value == 't')
    {
        discovery_register = 47;
        return B1b();
    }
    else if (discovery_register == 19 && s.value == 'w')
    {
        discovery_register = 22;
        return B1b();
    }
    else if (discovery_register == 3 && s.value == 'm')
    {
        discovery_register = 4;
        return B1b();
    }
    else if (discovery_register == 6 && s.value == 'u')
    {
        discovery_register = 8;
        return B1b();
    }
    else
        return Error1();
}

// парсим список 
int P1()
{
    if ((obj.end() - 1)->class_register != PUSH)
        return Error1();
    (obj.end() - 1)->const_flag = 1;
    (obj.end() - 1)->type = 1;
    return s_P1;
}

int P1a() {
    return s_P1;
}

int P2()
{
    return s_P2;
}

int Q1a()
{
    number_register = s.value;
    return s_Q1;
}

int Q1b()
{
    number_register = number_register * 10 + s.value;
    return s_Q1;
}

int Q2()
{
    (obj.end() - 1)->li.insert(number_register);
    return P2();
}

int Q2a() {
    return s_Q2;
}

int S1() {
    return s_S1;
}

int S1a()
{
    (obj.end() - 1)->li.insert(number_register);
    return s_S1;
}

int S1b()
{
    return s_S1;
}

int Error1()
{
    flag_Error = 1;
    cout << "���������� ������ � ������: " << str_register << endl;
    if ((!obj.empty()) && (str_register == (obj.end() - 1)->str))
        obj.pop_back();

    classToken k;
    k.class_register = ERROR;
    k.str = str_register;
    k.const_flag = 3;
    obj.push_back(k);

    return J1();
}

int Exit1()
{
    return s_Stop;
}

int Exit2()
{
    (obj.end() - 1)->value = number_register;
    return s_Stop;
}

int Exit3()
{
    (obj.end() - 1)->name = name;
    if (!name.empty())
        name.clear();
    return s_Stop;
}

int Exit4()
{
    if (relat_register == 1)
        (obj.end() - 1)->value = LESS;
    else if (relat_register == 2)
        (obj.end() - 1)->value = MORE;
    else if (relat_register == 3)
        (obj.end() - 1)->value = EQUAL;
    else
        return Error1();
    return s_Stop;
}


#endif