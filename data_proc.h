#pragma once 
#ifndef  DATA_PROC_H
#define DATA_PROC_H

#include <stack>
#include <iostream>
#include <vector>
#include "hash_mset.h"
#include "functionSM.h"
using namespace std;

class processing {
protected:
    struct Var
    {
        bool type;
        // TODO: std::any 
        int num;
        hash_set li; // сделать hash 
    };

    
    struct Variable 
    { 
        string name; // название переменной  
        bool type; //
        int num;
        hash_set li;
    };

        
    vector<Variable>::iterator FindElem(std::vector<Variable>& t, string name)
    {
        vector<Variable>::iterator it;
        for (it = t.begin(); it != t.end(); ++it)
            if (it->name == name)
                return it;
        return it;
    }

    bool CheckEl(const vector<string> &v, const string &name)
    {
        for (vector<string>::const_iterator it = v.cbegin(); it != v.cend(); ++it)
            if (*it == name)
                return true;
        return false;
    }


    stack<Var> x;
public:
    processing(vector<classToken> & vec_lex) {
        vector<Variable> t;
        if (flag_Error == 1) {
            return;
        }

        for (vector<classToken>::iterator it = obj.begin(); it != obj.end(); ++it) {
            if (it->class_register == OPERATION)
            {
                if (x.size() < 2)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the OPERATOR " << (char)it->value <<  endl;
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
                    cout << "" << endl;
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
                        cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the RATIO " << endl;
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
                cout << "enter a number:";
                cin >> k.num;
                x.push(k);
            }
            else if (it->class_register == WRITE)
            {
                if (x.size() < 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the WRITE" << endl;
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
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the POP" << endl;
                    return;
                }
                if (x.top().type == 0)
                {
                    auto iter = FindElem(t, it->name); //�������� ������� ����������
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
                    auto iter = FindElem(t, it->name);
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
                    auto iter = FindElem(t, it->name);
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
                hash_set li1;
                cout << "enter a multiset:";
                cin >> li1;
                k.type = 1;
                k.li = li1;
                x.push(k);
            }
            else if (it->class_register == ADD)
            {
                if (x.size() < 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the ADD" << endl;
                    return;
                }
                if (x.top().type != 1)
                {
                    cout << "������. ��� ������ �� ������������ ������" << endl;
                    return;
                }
                Var k;
                k.type = 1;
                hash_set obj = x.top().li;
                obj.add_the_first_missing();
                k.li = obj;
                x.pop();
                x.push(k);
            }
            else if (it->class_register == DIFF)
            {
                if (x.size() < 2)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the DIFF" << endl;
                    return;
                }
                Var res;
                hash_set li1, li2;
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
                res.li = set_diff(li2, li1);
                x.push(res);
            }
            else if (it->class_register == SYMM)
            {
                if (x.size() < 2)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the SYMM" << endl;
                    return;
                }
                Var res;
                hash_set li1, li2;
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
                res.li = set_symm_diff(li2, li1); 
                x.push(res);
            }
            else if (it->class_register == UNION)
            {
                if (x.size() < 2)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the UNION" << endl;
                    return;
                }
                Var res;
                hash_set li1, li2;
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
                res.li = set_union(li2, li1);
                x.push(res);
            }
            else if (it->class_register == INTERSEC)
            {
                if (x.size() < 2)
                {
                    
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the INTERSEC" << endl;
                    return;
                }
                Var res;
                hash_set li1, li2;
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
                res.li = set_intersec(li2, li1);
                x.push(res);
            }
            else if (it->class_register == ITEM)
            {
                if (x.size() < 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the ITEM" << endl;
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
                    auto iter = FindElem(t, it->name);
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
                    k.num = x.top().li.contain(iter->num);
                    x.pop();
                    x.push(k);
                }
                else if (it->const_flag == 1)
                {
                    Var k;
                    k.type = 0;
                    k.num = x.top().li.contain(it->value);
                    x.pop();
                    x.push(k);
                }
            }
            else if (it->class_register == POWER)
            {
                if (x.size() < 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the POWER" << endl;
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
    ~processing() {}

    
};

#endif