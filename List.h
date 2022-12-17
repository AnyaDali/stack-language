#include <iostream>
#include <cstring>

using namespace std;

class List;

class Iterator;

template <typename _N>
struct Node;

template <typename _Ty1, typename _Ty2, typename _Num>
class IList
{
public:
    virtual int power() const = 0;
    virtual bool liner_finder(const _Num &number) const = 0;
    virtual void push_back(const _Num &p) = 0;
    virtual _Ty1 Union(const _Ty1 &obj) const = 0;
    virtual _Ty1 intersection(const _Ty1 &obj) const = 0;
    virtual void clear() = 0;
    virtual bool operator!() const = 0;
    virtual bool operator==(const _Ty1 &obj) const = 0;
    virtual bool operator!=(const _Ty1 &obj) const = 0;
    virtual _Ty1 difference(const _Ty1 &obj) const = 0;
    virtual _Ty1 symmetric_difference(const _Ty1 &obj) const = 0;
    virtual void adding_absent() = 0;
    virtual _Ty1 &operator=(const _Ty1 &obj) = 0;
    virtual int operator[](const int i) const = 0;
    virtual bool empty() const = 0;

    virtual ~IList() {}
};

template <typename _N>
struct Node
{
    _N x;
    Node *next;
};

class List : public IList<List, Node<int>, int>
{
protected:
    Node<int> *lst;

public:
    class Iterator
    {
    protected:
        Node<int> *it;

    public:
        friend class List;

        Iterator()
        {
            it = nullptr;
        }

        Iterator &operator=(const Iterator &obj)
        {
            if (this == &obj)
                return *this;
            it = obj.it;
            return *this;
        }

        Iterator &operator++()
        {
            if (it != nullptr)
                it = it->next;
            else
                throw "Выход за границы диапазаона";
            return *this;
        }

        Iterator operator+(const int p)
        {
            Iterator res = *this;
            for (int i = 0; i < p; ++i)
            {
                if (res.it != nullptr)
                    res.it = res.it->next;
                else
                    throw "Выход за границы диапазаона";
            }
            return res;
        }

        bool operator!=(const Iterator &obj) const
        {
            return (it != obj.it);
        }

        bool operator==(const Iterator &obj) const
        {
            return (it == obj.it);
        }

        int &operator*() const
        {
            return it->x;
        }
    };

    friend ostream &operator<<(ostream &os, const List &obj);

    friend istream &operator>>(istream &is, List &obj);

    List()
    {
        lst = nullptr;
    }

    List(const List &obj)
    {
        lst = nullptr;
        *this = obj;
    }

    ~List()
    {
        clear();
    }

public:
    int power() const override
    {
        Node<int> *tail = lst;
        int count = 0;
        while (tail)
        {
            tail = tail->next;
            count++;
        }
        return count;
    }

private:
    bool liner_finder(const int &number_register) const override
    {
        Node<int> *tail = lst;
        while (tail)
        {
            if (tail->x == number_register)
                return true;
            tail = tail->next;
        }
        return false;
    }

public:
    void push_back(const int &p) override
    {
        if (lst == nullptr) //если список пуст
        {
            Node<int> *elem = new Node<int>;
            elem->x = p;
            lst = elem;
            lst->next = nullptr;
            return;
        }
        Node<int> *prev = nullptr;
        Node<int> *tail = lst;
        while (tail)
        {
            prev = tail;
            tail = tail->next;
        }
        Node<int> *elem = new Node<int>;
        elem->x = p;
        prev->next = elem;
        prev = prev->next;
        prev->next = tail;
        return;
    }

public:
    List Union(const List &obj) const override
    {

        List res = *this;
        Node<int> *tail = obj.lst;
        while (tail)
        {
            if (!liner_finder(tail->x))
                res.push_back(tail->x);
            tail = tail->next;
        }
        return res;
    }

public:
    List intersection(const List &obj) const override
    {
        List res;
        Node<int> *tail = obj.lst;
        while (tail)
        {
            if (liner_finder(tail->x))
                res.push_back(tail->x);
            tail = tail->next;
        }
        return res;
    }

public: //исправлено
    void clear() override
    {
        Node<int> *tail = lst;
        if (tail == nullptr)
            return;
        do
        {
            Node<int> *prev = tail;
            tail = tail->next;
            delete prev;
        } while (tail != nullptr);
        lst = nullptr;
    }

public:
    bool operator!() const override
    {
        if (lst == nullptr)
            return true;
        return false;
    }

public:
    bool operator==(const List &obj) const override
    {
        if (power() != obj.power())
            return false;
        Node<int> *tail = obj.lst;
        while (tail)
        {
            if (!liner_finder(tail->x))
                return false;
            tail = tail->next;
        }
        return true;
    }

public:
    bool operator!=(const List &obj) const override
    {
        if (operator==(obj))
            return false;
        return true;
    }

public:
    List difference(const List &obj) const override
    {
        List res;
        Node<int> *tail = lst;
        while (tail)
        {
            if (!obj.liner_finder(tail->x))
                res.push_back(tail->x);
            tail = tail->next;
        }
        return res;
    }

public:
    List symmetric_difference(const List &obj) const override
    {
        List res1;
        Node<int> *tail = lst;
        while (tail)
        {
            if (!obj.liner_finder(tail->x))
                res1.push_back(tail->x);
            tail = tail->next;
        }
        List res2;
        tail = obj.lst;
        while (tail)
        {
            if (!liner_finder(tail->x))
                res2.push_back(tail->x);
            tail = tail->next;
        }
        return res1.Union(res2);
    }

public:
    void adding_absent() override
    {
        int c = 0;
        bool fl = true;
        while (fl)
        {
            if (!liner_finder(c))
            {
                push_back(c);
                fl = false;
            }
            ++c;
        }
    }

public:
    List &operator=(const List &obj) override
    {
        if (this == &obj)
            return *this;
        if (lst)
            this->clear();
        Node<int> *tail2 = obj.lst;
        if (tail2 != nullptr)
        {
            Node<int> *elem = new Node<int>;
            elem->x = tail2->x;
            lst = elem;
            lst->next = nullptr;
            tail2 = tail2->next;
        }
        Node<int> *tail1 = lst;
        while (tail2 != nullptr)
        {
            Node<int> *elem = new Node<int>;
            elem->x = tail2->x;
            elem->next = nullptr;
            tail1->next = elem;
            tail1 = tail1->next;
            tail2 = tail2->next;
        }
        return *this;
    }

public:
    Iterator begin() const
    {
        Iterator obj;
        obj.it = lst;
        return obj;
    }

public:
    bool empty() const override
    {
        if (lst == nullptr)
            return true;
        return false;
    }

public:
    int operator[](const int index) const override
    {
        if (index >= power())
            throw "выход за пределы диапазона";

        Iterator it = begin();
        for (int i = 0; i < index; i++)
            ++it;

        return *it;
    }

public:
    Iterator end() const
    {
        Iterator obj;
        obj.it = nullptr;
        return obj;
    }
};

ostream &operator<<(ostream &os, const List &obj)
{
    Node<int> *tail = obj.lst;

    os << '[';
    while (tail)
    {
        os << tail->x;
        if (tail->next)
            os << ',';
        tail = tail->next;
    }
    os << ']';
    return os;
}

istream &operator>>(istream &is, List &obj)
{
    //исправлено. теперь при вводе данных, если obj содержал эл-ты, они стрираются
    if (!obj.empty())
        obj.clear();
    char ch;
    while (is.peek() == ' ')
        ch = is.get();

    if (ch = is.get() != '[')
        throw "Синтаксическая ошибка: ожидался символ [";

    string str;
    while (is.peek() != ']')
    {
        while (is.peek() == ' ')
            ch = is.get();

        while (is.peek() >= '0' && is.peek() <= '9')
            str += is.get();

        int num = atoi(str.c_str());
        str.clear();

        obj.push_back(num);

        while (is.peek() == ' ')
            ch = is.get();

        if (is.peek() != ',' && is.peek() != ']')
            throw "Синтаксическая ошибка";

        if (is.peek() == ',')
            ch = is.get();

        while (is.peek() == ' ')
            ch = is.get();
    }
    ch = is.get();

    while (is.peek() == ' ')
        is.get(ch);

    if (is.peek() == '\n')
        is.ignore(1);
    return is;
}
