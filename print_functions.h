#pragma once 
#ifndef PRINT_FUNCTIONS_H
#define PRINT_FUNCTIONS_H
#include <iostream>
#include <vector>
#include "functionSM.h"
using namespace std;


// распечатать массив лексем 
void PrintObj(const std::vector<classToken> & vec_token)
{
    for (vector<classToken>::const_iterator it = vec_token.cbegin(); it != vec_token.cend(); ++it)
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
        else if (it->class_register == POP) 
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



#endif