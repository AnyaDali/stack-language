#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <stack>
#include "List.h"
#include "translit.h"
using namespace std;


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


SymbolicToken s;        // ���������� �������.
int class_register;     //������� ������
int number_register;    // ������� �����.
int relat_register;     //������� ���������
int str_register = 1;   //������ ����� ������� ������ � ���������
int discovery_register; //������� �����������
bool flag_Error = 0;    //���� ������ 0 - �� ������� ������, 1 - �����
string name;

//��������� ��� ������ �������
struct classToken
{
    int class_register; // ������� ������
    int type;           //0 - �����, 1 - ������
    List li;
    int const_flag; // ���� ���������: 1 - ���������, 0 - ���������� 3 - ���������� ��������� � ���������
    string name;    // ��� ����������
    int value;      // �������� ���������
    int str;        // ������ ����� ������� ������ � ���������
};

vector<classToken> obj;


int Error1();

// ��������� �������� (�� A1 �� Exit1).
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
    (obj.end() - 1)->class_register = OPERATION; //����� ������� - ��������
    (obj.end() - 1)->value = s.value;            // �������� �������
    (obj.end() - 1)->str = str_register;         //����� ������
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

int P1()
{
    if ((obj.end() - 1)->class_register != PUSH)
        return Error1();
    (obj.end() - 1)->const_flag = 1;
    (obj.end() - 1)->type = 1;
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
    (obj.end() - 1)->li.push_back(number_register);
    return s_Q2;
}

int S1a()
{
    (obj.end() - 1)->li.push_back(number_register);
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

// размерность таблицы 
const int state_number = 24;  
const int class_number = 11; 

typedef int (*function_pointer)();
function_pointer table[state_number][class_number];

void initialize_table()
{
    // заполняем таблицу функцией ошибок 
    for (int i = 0; i < state_number; i++)
        for (int j = 0; j < class_number; j++)
            table[i][j] = Error1;
    
    table[s_A1][Character] = B1a;
    table[s_A1][Sign] = C1a;
    table[s_A1][Attitude] = D1a;
    table[s_A1][Space] = A1;
    table[s_A1][LF] = A1b;
    table[s_A1][Comment] = I1a;

    table[s_A2][Character] = B1a;
    table[s_A2][Sign] = C1a;
    table[s_A2][Attitude] = D1a;
    table[s_A2][Space] = A2;
    table[s_A2][LF] = A2a;
    table[s_A2][Comment] = I2a;
    table[s_A2][EndOfFile] = Exit1;

    table[s_B1][Character] = M1;

    table[s_C1][Space] = C1;
    table[s_C1][LF] = A2a;
    table[s_C1][Comment] = I2a;
    table[s_C1][EndOfFile] = Exit1;

    table[s_D1][Attitude] = C1h;
    table[s_D1][Space] = C1g;
    table[s_D1][LF] = A2e;
    table[s_D1][Comment] = I2d;
    table[s_D1][Character] = Error1;
    table[s_D1][EndOfFile] = Exit1;

    table[s_E1][Character] = Error1;
    table[s_E1][Space] = F1;

    table[s_E2][Space] = F2;

    table[s_E3][Space] = F3;

    table[s_F1][Character] = H1a;
    table[s_F1][Digit] = G1a;
    table[s_F1][Space] = F1;
    table[s_F1][LBracket] = P1;

    table[s_F2][Digit] = G1a;
    table[s_F2][Space] = F2;

    table[s_F3][Character] = H1a;
    table[s_F3][Space] = F3;

    table[s_G1][Digit] = G1b;
    table[s_G1][Space] = C1e;
    table[s_G1][LF] = A2c;
    table[s_G1][Comment] = I2b;
    table[s_G1][EndOfFile] = Exit2;

    table[s_H1][Character] = H1b;
    table[s_H1][Digit] = H1b;
    table[s_H1][Space] = C1f;
    table[s_H1][LF] = A2d;
    table[s_H1][Comment] = I2c;
    table[s_H1][EndOfFile] = Exit3;

    table[s_I1][Character] = I1;
    table[s_I1][Digit] = I1;
    table[s_I1][Sign] = I1;
    table[s_I1][Attitude] = I1;
    table[s_I1][Space] = I1;
    table[s_I1][LF] = A1a;
    table[s_I1][Comment] = I1;
    table[s_I1][Comma] = I1;
    table[s_I1][RBracket] = I1;
    table[s_I1][LBracket] = I1;
    table[s_I1][Comment] = I1;
    table[s_I1][Error] = I1;

    table[s_I2][Character] = I2;
    table[s_I2][Digit] = I2;
    table[s_I2][Sign] = I2;
    table[s_I2][Attitude] = I2;
    table[s_I2][Space] = I2;
    table[s_I2][LF] = A2a;
    table[s_I2][Comma] = I2;
    table[s_I2][RBracket] = I2;
    table[s_I2][LBracket] = I2;
    table[s_I2][Comment] = I2;
    table[s_I2][Error] = I2;
    table[s_I2][EndOfFile] = Exit1;

    table[s_J1][Character] = J1;
    table[s_J1][Digit] = J1;
    table[s_J1][Sign] = J1;
    table[s_J1][Attitude] = J1;
    table[s_J1][Space] = J1;
    table[s_J1][LF] = A2a;
    table[s_J1][Comment] = J1;
    table[s_J1][Comma] = J1;
    table[s_J1][RBracket] = J1;
    table[s_J1][LBracket] = J1;
    table[s_J1][Error] = J1;
    table[s_J1][EndOfFile] = Exit1;

    table[s_P1][Digit] = Q1a;

    table[s_Q1][RBracket] = S1a;
    table[s_Q1][Digit] = Q1b;
    table[s_Q1][Comma] = Q2;

    table[s_Q2][Digit] = Q1a;

    table[s_S1][Space] = S1b;
    table[s_S1][LF] = A2a;
}

void parse(const char *filename)
{
    // открытие файла 
    ifstream buff(filename);
    // проверить, открыт ли файл 
    if (!buff.is_open())
    {
        cout << "could not open the file " << filename << endl;
        return;
    }
    
    char ch;         
    int state = s_A1;
    while (state != s_Stop && !flag_end)
    {
        ch = buff.get();
        s = transliterator(ch);
        if (flag_end == 1)
            break;
        state = (*table[state][s.token_class])();
    }
    int c = (obj.end() - 1)->class_register;
    if ((c == PUSH && (obj.end() - 1)->const_flag == 1) || c == JMP || c == JI || (c == ITEM && (obj.end() - 1)->const_flag == 1))
        Exit2();
    else if ((c == PUSH && (obj.end() - 1)->const_flag == 0) || c == POP || (c == ITEM && (obj.end() - 1)->const_flag == 0))
        Exit3();
    else if (c == ATTITUDE && ((obj.end() - 1)->value == LESS || (obj.end() - 1)->value == MORE || (obj.end() - 1)->value == EQUAL))
        Exit4();
    else
        Exit1();
    cout << "\n";

    buff.close();
}

struct Var
{
    bool type;
    int num;
    List li;
};

//��������� ��� �������� ����������
struct Variable
{
    string name;
    bool type; //0 - ����������, 1 - ������
    int num;
    List li;
};

stack<Var> x;       //���� �� ����� � �������
vector<Variable> t; //������ ����������

vector<Variable>::iterator FindElem(string name)
{
    vector<Variable>::iterator it;
    for (it = t.begin(); it != t.end(); ++it)
        if (it->name == name)
            return it;
    return it;
}

void PrintObj()
{
    for (vector<classToken>::iterator it = obj.begin(); it != obj.end(); ++it)
    {
        cout << it->str << ". ";
        if (it->class_register == OPERATION)
            cout << "OPERATION ";
        else if (it->class_register == ATTITUDE)
            cout << "ATTITUDE ";
        else if (it->class_register == READ)
            cout << "READ ";
        else if (it->class_register == WRITE)
            cout << "WRITE ";
        else if (it->class_register == POP) //��������� � ���������� ��������
            cout << "POP " << it->name;
        else if (it->class_register == PUSH)
        {
            if (it->const_flag == 0)
            {
                cout << "PUSH " << it->name;
            }
            if (it->const_flag == 1)
            {
                if (it->type == 0)
                    cout << "PUSH " << it->value;
                if (it->type == 1)
                    cout << "PUSH " << it->li;
            }
        }
        else if (it->class_register == JI)
            cout << "JI " << it->value;
        else if (it->class_register == JMP)
            cout << "JMP " << it->value;
        else if (it->class_register == END)
            cout << "END ";
        else if (it->class_register == LIREAD)
            cout << "LIREAD ";
        else if (it->class_register == ADD)
            cout << "ADD ";
        else if (it->class_register == DIFF)
            cout << "DIFF ";
        else if (it->class_register == SYMM)
            cout << "SYMM ";
        else if (it->class_register == UNION)
            cout << "UNION ";
        else if (it->class_register == INTERSEC)
            cout << "INTERSEC ";
        else if (it->class_register == ITEM)
        {
            if (it->const_flag == 0)
                cout << "ITEM " << it->name;
            if (it->const_flag == 1)
                cout << "ITEM " << it->value;
        }
        else if (it->class_register == POWER)
            cout << "POWER ";
        else if (it->class_register == COMMENT)
            cout << "COMMENT ";
        else if (it->class_register == ERROR)
            cout << "ERROR ";
        cout << "\n";
    }
}

bool CheckEl(const vector<string> &v, const string &name)
{
    for (vector<string>::const_iterator it = v.cbegin(); it != v.cend(); ++it)
        if (*it == name)
            return true;
    return false;
}

void PrintVariables()
{
    vector<string> v;
    for (vector<classToken>::iterator it = obj.begin(); it != obj.end(); ++it)
        if (it->const_flag == 0)
            if (!CheckEl(v, it->name))
                v.push_back(it->name);
    cout << "< ";
    for (vector<string>::iterator it = v.begin(); it != v.end(); ++it)
        cout << *it << " ";
    cout << ">" << endl;
}

void PrintConst()
{
    vector<int> v;
    for (vector<classToken>::iterator it = obj.begin(); it != obj.end(); ++it)
        if (it->const_flag == 1)
        {
            if (it->type == 0)
                cout << it->value << endl;
            else if (it->type == 1)
                cout << it->li << endl;
        }
}

void DataProcessing()
{
    if (flag_Error == 1)
        return;
    //������ �� ������ ������
    for (vector<classToken>::iterator it = obj.begin(); it != obj.end(); ++it)
    {
        if (it->class_register == OPERATION)
        {
            if (x.size() < 2)
            {
                cout << "������ ���������� ��������" << endl;
                return;
            }
            Var v1, v2;
            Var res;
            res.type = 0;
            v1 = x.top();
            x.pop();
            v2 = x.top();
            x.pop();
            if (v1.type != 0 || v2.type != 0)
            {
                cout << "������. ��� ������ �� ������������ �����" << endl;
                return;
            }
            if (it->value == '+')
                res.num = v2.num + v1.num;
            else if (it->value == '-')
                res.num = v2.num - v1.num;
            else if (it->value == '*')
                res.num = v2.num * v1.num;
            else if (it->value == '/')
                res.num = v2.num / v1.num;
            else if (it->value == '%')
                res.num = v2.num % v1.num;
            x.push(res);
        }
        else if (it->class_register == ATTITUDE)
        {
            if (x.size() < 2)
            {
                cout << "������ ���������� ���������" << endl;
                return;
            }

            if (x.top().type == 0)
            {
                Var res;
                res.type = 0;
                Var v1 = x.top();
                x.pop();
                Var v2 = x.top();
                x.pop();
                if (v1.type != 0 || v2.type != 0)
                {
                    cout << "������. ��� ������ �� ������������ �����" << endl;
                    return;
                }
                if (it->value == LESS)
                    res.num = v2.num < v1.num;
                else if (it->value == MORE)
                    res.num = v2.num > v1.num;
                else if (it->value == LESS_EQUAL)
                    res.num = v2.num <= v1.num;
                else if (it->value == MORE_EQUAL)
                    res.num = v2.num >= v1.num;
                else if (it->value == EQUAL)
                    res.num = v2.num == v1.num;
                else if (it->value == INEQUAL)
                    res.num = v2.num != v1.num;

                x.push(res);
            }
            else if (x.top().type == 1)
            {
                Var res;
                res.type = 0;
                Var v1 = x.top();
                x.pop();
                Var v2 = x.top();
                x.pop();
                if (v1.type != 1 || v2.type != 1)
                {
                    cout << "������. ��� ������ �� ������������ ������" << endl;
                    return;
                }
                if (it->value == EQUAL)
                    res.num = (v2.li == v1.li);
                else if (it->value == INEQUAL)
                    res.num = (v2.li != v1.li);

                x.push(res);
            }
        }
        else if (it->class_register == READ)
        {
            Var k;
            k.type = 0;
            cout << "������� �����: ";
            cin >> k.num;
            x.push(k);
        }
        else if (it->class_register == WRITE)
        {
            if (x.size() < 1)
            {
                cout << "������. ���������� �������� WRITE � ������� �����" << endl;
                return;
            }
            if (x.top().type == 0)
                cout << x.top().num << endl;
            else if (x.top().type == 1)
                cout << x.top().li << endl;

            x.pop();
        }
        else if (it->class_register == POP) //��������� � ���������� ��������
        {
            if (x.size() < 1)
            {
                cout << "������. ���������� �������� POP � ������� �����" << endl;
                return;
            }
            if (x.top().type == 0)
            {
                auto iter = FindElem(it->name); //�������� ������� ����������
                if (iter == t.end())
                {
                    Variable k;
                    k.type = 0;
                    k.name = it->name;
                    k.num = x.top().num;
                    x.pop();
                    t.push_back(k);
                }
                else
                {
                    iter->num = x.top().num;
                    iter->type = 0;
                    x.pop();
                }
            }
            else if (x.top().type == 1)
            {
                auto iter = FindElem(it->name);
                if (iter == t.end())
                {
                    Variable k;
                    k.type = 1;
                    k.name = it->name;
                    k.li = x.top().li;
                    x.pop();
                    t.push_back(k);
                }
                else
                {
                    iter->li = x.top().li;
                    iter->type = 1;
                    x.pop();
                }
            }
        }
        else if (it->class_register == PUSH)
        {

            if (it->type == 1)
            {
                if (it->const_flag == 1)
                {
                    Var k;
                    k.type = 1;
                    k.li = it->li;
                    x.push(k);
                }
            }
            else if (it->type == 0)
            {
                if (it->const_flag == 1)
                {
                    Var k;
                    k.type = 0;
                    k.num = it->value;
                    x.push(k);
                }
            }
            if (it->const_flag == 0)
            {
                /*
                ���� ��������� - ����������.
                ������ � �������� �� ������� ����������.
                */
                auto iter = FindElem(it->name);
                if (iter == t.end())
                {
                    cout << "������. ������� ��������� � ���� ���������� ��� ��������, �������: " << it->str << endl;
                    return;
                }
                Var k;
                k.type = iter->type;
                if (k.type == 0)
                    k.num = iter->num;
                else if (k.type == 1)
                    k.li = iter->li;
                x.push(k);
            }
        }
        else if (it->class_register == JI)
        {
            if (x.top().num != 0)
            {
                x.pop();
                int val = it->value;
                it = obj.begin();
                while (it->str != val)
                    ++it;
                --it;
            }
        }
        else if (it->class_register == JMP)
        {
            int val = it->value;
            it = obj.begin();
            while (it->str != val)
                ++it;
            --it;
        }
        else if (it->class_register == END)
            break;
        else if (it->class_register == LIREAD)
        {
            Var k;
            List li1;
            cout << "������� ������: ";
            cin >> li1;
            k.type = 1;
            k.li = li1;
            x.push(k);
        }
        else if (it->class_register == ADD)
        {
            if (x.size() < 1)
            {
                cout << "������. ���������� �������� ADD � ������� �����" << endl;
                return;
            }
            if (x.top().type != 1)
            {
                cout << "������. ��� ������ �� ������������ ������" << endl;
                return;
            }
            Var k;
            k.type = 1;
            List obj = x.top().li;
            obj.adding_absent();
            k.li = obj;
            x.pop();
            x.push(k);
        }
        else if (it->class_register == DIFF)
        {
            if (x.size() < 2)
            {
                cout << "������. ���������� �������� DIFF" << endl;
                return;
            }
            Var res;
            List li1, li2;
            if (x.top().type != 1)
            {
                cout << "������. ��� ������ �� ������������ ������" << endl;
                return;
            }
            li1 = x.top().li;
            x.pop();
            if (x.top().type != 1)
            {
                cout << "������. ��� ������ �� ������������ ������" << endl;
                return;
            }
            li2 = x.top().li;
            x.pop();
            res.type = 1;
            res.li = li2.difference(li1);
            x.push(res);
        }
        else if (it->class_register == SYMM)
        {
            if (x.size() < 2)
            {
                cout << "������. ���������� �������� SYMM" << endl;
                return;
            }
            Var res;
            List li1, li2;
            if (x.top().type != 1)
            {
                cout << "������. ��� ������ �� ������������ ������" << endl;
                return;
            }
            li1 = x.top().li;
            x.pop();
            if (x.top().type != 1)
            {
                cout << "������. ��� ������ �� ������������ ������" << endl;
                return;
            }
            li2 = x.top().li;
            x.pop();
            res.type = 1;
            res.li = li2.symmetric_difference(li1);
            x.push(res);
        }
        else if (it->class_register == UNION)
        {
            if (x.size() < 2)
            {
                cout << "������. ���������� �������� UNION" << endl;
                return;
            }
            Var res;
            List li1, li2;
            if (x.top().type != 1)
            {
                cout << "������. ��� ������ �� ������������ ������" << endl;
                return;
            }
            li1 = x.top().li;
            x.pop();
            if (x.top().type != 1)
            {
                cout << "������. ��� ������ �� ������������ ������" << endl;
                return;
            }
            li2 = x.top().li;
            x.pop();
            res.type = 1;
            res.li = li2.Union(li1);
            x.push(res);
        }
        else if (it->class_register == INTERSEC)
        {
            if (x.size() < 2)
            {
                cout << "������. ���������� �������� INTERSEC" << endl;
                return;
            }
            Var res;
            List li1, li2;
            if (x.top().type != 1)
            {
                cout << "������. ��� ������ �� ������������ ������" << endl;
                return;
            }
            li1 = x.top().li;
            x.pop();
            if (x.top().type != 1)
            {
                cout << "������. ��� ������ �� ������������ ������" << endl;
                return;
            }
            li2 = x.top().li;
            x.pop();
            res.type = 1;
            res.li = li2.intersection(li1);
            x.push(res);
        }
        else if (it->class_register == ITEM)
        {
            if (x.size() < 1)
            {
                cout << "������. ���������� �������� ITEM" << endl;
                return;
            }
            if (x.top().type != 1)
            {
                cout << "������. ��� ������ �� ������������ ������" << endl;
                return;
            }
            if (it->const_flag == 0)
            {
                Var k;
                k.type = 0;
                auto iter = FindElem(it->name);
                if (iter == t.end())
                {
                    cout << "������. ������� ��������� � ���� ���������� ��� ��������" << endl;
                    return;
                }
                if (iter->type != 0)
                {
                    cout << "������. �������� �������� item �� �������� ����������� ������" << endl;
                    return;
                }
                k.num = x.top().li[iter->num];
                x.pop();
                x.push(k);
            }
            else if (it->const_flag == 1)
            {
                Var k;
                k.type = 0;
                k.num = x.top().li[it->value];
                x.pop();
                x.push(k);
            }
        }
        else if (it->class_register == POWER)
        {
            if (x.size() < 1)
            {
                cout << "������. ���������� �������� POWER" << endl;
                return;
            }
            if (x.top().type != 1)
            {
                cout << "������. ��� ������ �� ������������ ������" << endl;
                return;
            }
            int res = x.top().li.power();
            x.pop();
            Var k;
            k.type = 0;
            k.num = res;
            x.push(k);
        }
    }
}

// ����� ��������� ����� ��� ./���-������������-����� ���-��������-�����.
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Invalid input format.\nInput format: stack_lang.exe -file_input.txt" << endl;
        return 1;
    }

    // инициализация состояний конечного преобразователя 
    initialize_table();
    parse(argv[1]);

    cout << "������ ������:" << endl;
    PrintObj();
    cout << "-----------------------------------" << endl;
    cout << "������ ��������: " << endl;
    PrintConst();
    cout << "������ ����������:" << endl;
    PrintVariables();
    cout << "�������������:" << endl;
    DataProcessing();
    return 0;
}
/*
��������� �����

��� ����������� �����: �� �����, ��� � � �������� ����� �� ������� �� ����������� ��������� ������.
���� ������� ���� �� ���� ������, �� ������������� �� ����� ������������ ������
������ ��������� �� ������ � ����� ������ � �������.

��� ������� �������� �������:
1) union
2) intersec
3) diff
4) symm
5) add
6) item n
7) =
8) !=
9) pop x 
10) power
��� ��� �������� ����������� � ������� ����� � ��������� ������������ �� ������� �����.
��������� ��� �������� push ��� ���������
push [p1,p2,...p] - ����� ������������ ����� �������, ��� ��������
...
������� add ��������� � ������ ������ ������������� ��-�
������ ���������: 
push [0,2,5,1]
add
write
��������� ������ �� ������:
[0,2,5,1,3]
... 
����������� ������ ����� ������� ����� ����� ������� � �������� (�����).
...
������ ������ � ������ pushh 2 �������������� � ����� ������ ������� (������� ������� �����������).
...
����� ��������, � ������� ��������� ���������� � ��������� �����, ����� ������, 
���� ���� ���� ��� �� ������� ����� �� �������������� �������� ��� ������,
� ���������������, � �� �������� ��� ������.
...
��� ������ ������, ������� ����������, ������� �������� ������� ���������� ����� ������, 
� ����� �������, ��� ����������, ��� ��������� �������������.
08.01.21
1) ������� �������� item ������ ����� ��������� ����������
2) ���������� ������ �������� ���������� � ��������
3) ��������� 
push 1
pop x
push x
write
push x
push 1
+
pop x
jmp 3
 �������� ���������
 ...
 09.01
1) ��� ������ ����������� � ����� ������������ (� � ������ �������, ������� ������� �����������).
2) ������ �������� push x ����� �������� ������, ���� ���������� x �� ������ ��������. 
3) ��� �������� item x ���� ��������, ����� ���������� x ����������� �������� ����������� ������.
4) �������� liread ��������� ��������� �� ������� � �������� ��� � ����
5) ������ ��� ������ item ���� �������� �� ����� �� ������� ���������
*/