#include <iostream>
#include <fstream>
#include <unordered_set>
#include <stack>
#include "hash_mset.h"
#include "initialize_table.h"
#include "print_functions.h"
#include "data_proc.h"
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
    PrintObj(obj);
    cout << "-----------------------------------" << endl;
    cout << "Array of Constants: " << endl;
    PrintConst();
    cout << "Array of Variables:" << endl;
    PrintVariables();
    cout << "Execution:" << endl;
    processing proc(obj);
    return 0;
}