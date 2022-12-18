#include <iostream>
#include <fstream>
#include <unordered_set>
#include <stack>
#include "List.h"
#include "initialize_table.h"

using namespace std;

void parse_line(std::string && line, int & state) {
    for(auto cur_ch : line) {
        s = transliterator(cur_ch); // определяем символьную лексему 
        state = (*table_funtion[state][s.token_class])();
    }
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
    std::string curr_line;
    while (!buff.eof())
    {
        std::getline(buff, curr_line);
        curr_line += "\n";
        parse_line(std::move(curr_line), state);
    }
    std::cout <<"name = "<< obj.back().name << std::endl;
    /*int c = (obj.end() - 1)->class_register;
    if ((c == PUSH && (obj.end() - 1)->const_flag == 1) || c == JMP || c == JI || (c == ITEM && (obj.end() - 1)->const_flag == 1))
        Exit2();
    else if ((c == PUSH && (obj.end() - 1)->const_flag == 0) || c == POP || (c == ITEM && (obj.end() - 1)->const_flag == 0))
        Exit3();
    else if (c == ATTITUDE && ((obj.end() - 1)->value == LESS || (obj.end() - 1)->value == MORE || (obj.end() - 1)->value == EQUAL))
        Exit4();
    else
        Exit1();*/
    cout << "\n";

    buff.close();
}

struct Var
{
    bool type;
    // TODO: std::any 
    int num;
    List li; // сделать hash 
};

struct Variable 
{ 
    string name; // название переменной  
    bool type; //
    int num;
    List li;
};

stack<Var> x;       // главный стек интерпретатора
vector<Variable> t; // массив переменных 

vector<Variable>::iterator FindElem(string name)
{
    vector<Variable>::iterator it;
    for (it = t.begin(); it != t.end(); ++it)
        if (it->name == name)
            return it;
    return it;
}
// распечатать массив лексем 
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

bool CheckEl(const vector<string> &v, const string &name)
{
    for (vector<string>::const_iterator it = v.cbegin(); it != v.cend(); ++it)
        if (*it == name)
            return true;
    return false;
}

// распечатать массив переменных 
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

// распечатать массив констант 
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

// функция выполнения кода стекового языка  
void DataProcessing()
{
    if (flag_Error == 1)
        return;

    for (vector<classToken>::iterator it = obj.begin(); it != obj.end(); ++it)
    {
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
                cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the DIFF" << endl;
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
                cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the SYMM" << endl;
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
                cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the UNION" << endl;
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
                 
                cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the INTERSEC" << endl;
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


int main(int argc, char *argv[])
{
    //проверка на аргументы (входной файл)
    if (argc != 2)
    {
        cout << "Invalid input format.\nInput format: stack_lang.exe -file_input.txt" << endl;
        return 1;
    }

    // инициализация состояний конечного преобразователя 
    initialize_table();

    parse(argv[1]);

    cout << "Array of Tokens:" << endl;
    PrintObj();
    cout << "-----------------------------------" << endl;
    cout << "Array of Constants: " << endl;
    PrintConst();
    cout << "Array of Variables:" << endl;
    PrintVariables();
    cout << "Execution:" << endl;
    DataProcessing();
    return 0;
}