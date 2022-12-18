#pragma once
#ifndef HASH_MSET_H
#define HASH_MSET_H
#include <iostream>

class hash_set {
protected:
    struct node { // двусвязный список
        unsigned __val;
        node* __next;
        node* __prev;
    };

    struct __pair_node {            // элемент массива хеш-таблицы 
        node* __node_ptr;           // указатель на список 
        size_t __size_curr_bucket;  // размер текущего бакета 
    };

    size_t size_of_table; // размерность массива хеш таблицы
    __pair_node* arr;  // хеш таблица 
    size_t count_of_element; // кол-во элементов в множестве
    node* __head; // указатель на начало списка 
    node* __end; // указатель на конец списка 

    // хэш-функция 
    size_t hash_function(unsigned x) const {
        return x % size_of_table; 
    }
    

    // выделение памяти под массив 
    __pair_node* __mem_alloc(size_t sz) {
        return new __pair_node[sz];
    }


    //инициализация массива, изначально все элементы массива ссылаются на nullptr 
    void initialize_table(__pair_node* __first, __pair_node* __last) {
        for(;__first != __last; ++__first) {
            __first->__node_ptr = nullptr;
            __first->__size_curr_bucket = 0;
        }
    }

    // создание ноды 
    node* create_new_node(unsigned x, node * next = nullptr, node* prev = nullptr) {
        return new node{x, next, prev};
    }


    class iterator {
    protected:
        
        node* ptr;
        
        //конструктор от указателя для begin и end
        iterator(node* ptr1) : ptr(ptr1) {}

    public:
        iterator() : ptr(nullptr) {} 

        iterator(const iterator& it) : ptr(it.ptr) {}

        iterator& operator=(const iterator& it) {
            ptr = it.ptr;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ptr = ptr->__next;
            return tmp;
        }

        iterator& operator++() {
            ptr = ptr->__next;
            return *this;
        }

        iterator& operator--() {
            ptr = ptr->__prev;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            ptr = ptr->__prev;
            return tmp;
        }

        const node* base() const { return ptr; } 

        const unsigned& operator*() const { return ptr->__val; }

        bool operator==(const iterator &iter) const { return ptr == iter.ptr; }

        bool operator!=(const iterator &iter) const { return ptr != iter.ptr; }  

        friend class hash_set; //для begin и end при обращении к приватному конструктору
    };

    // удаление элемента списка 
    node* delete_el(node* curr) {
        if (curr == __head) {
            __head = curr->__next;
            __head->__prev = nullptr;
            delete(curr);
            return __head;
        } else {
            node* nxt = curr->__next;
            node* prv = curr->__prev;
            nxt->__prev = prv;
            prv->__next = nxt;
            delete(curr);
            return nxt;
        }
    }

    // очистка списка 
    void __clear_list() {
        if (empty()) {
            return;
        }
        node * prev = __head;
        node * curr = __head->__next;
        for(; curr != __end; curr = curr->__next) {
            delete(prev);
            prev = curr;
        }
        delete (prev);
        __head = __end;
        __head->__next = __head->__prev = __end;
        __end->__next = __end->__prev = __end;
    } 

    void clear_table(__pair_node * first, __pair_node* last) {
        for(; first != last; ++first) {
            first->__node_ptr = nullptr;
            first->__size_curr_bucket = 0;
        }
    }   

    // удалить первый встретившийся x из таблицы 
    void erase_first_el(unsigned x) {
        // [голова бакета, размер бакета]
        auto& [head_bucket, sz_bucket] = arr[hash_function(x)];
        if (head_bucket == nullptr) { // ничего не делаем 
            return;
        }
        node* curr = head_bucket;
        size_t c_erase = 0; // кол-во удалений 
    
        for(size_t i = 0; i < sz_bucket; ++i) {
            if (curr->__val == x) {
                ++c_erase;
                if (head_bucket == curr) {
                    head_bucket = head_bucket->__next;
                }
                delete_el(curr);
                break;
            } else {
                curr = curr->__next;
            }
        }

        sz_bucket -= c_erase; // 1 или 0
        count_of_element -= c_erase; // 1 или 0
        if(sz_bucket == 0) {
            head_bucket = nullptr;
        }
    }

public:


    hash_set() : size_of_table(29), arr(__mem_alloc(size_of_table)),  count_of_element(0), 
                __head(new node()),
                __end(__head) {
        __head->__next = __head->__prev = __end;
        __end->__next = __end->__prev = __end;
        initialize_table(arr, arr + size_of_table);
    }   

    hash_set(const hash_set & hs) : size_of_table(hs.size_of_table), arr(__mem_alloc(size_of_table)),
                                    count_of_element(0), __head(new node()), 
                                    __end(__head) {
        __head->__next = __head->__prev = __end;
        __end->__next = __end->__prev = __end;
        initialize_table(arr, arr + size_of_table);
        for(auto c_iter = hs.begin(); c_iter != hs.end(); ++c_iter) {
            insert(*c_iter);
        }   
    }

    hash_set& operator=(const hash_set& hs) {
        if (this == &hs) {
            return *this;
        }
        if(!empty()) {
            clear();
        } 
        size_of_table = hs.size_of_table; 
        for(auto c_iter = hs.begin(); c_iter != hs.end(); ++c_iter) {
            insert(*c_iter);
        }  

        return *this;


    }

    void erase(unsigned x) {
        auto &[head_bucket, sz_bucket] = arr[hash_function(x)];
        if (head_bucket == nullptr) {
            return;
        }
        node* curr = head_bucket;
        size_t c_erase = 0; // кол-во удалений 
    
        for(size_t i = 0; i < sz_bucket; ++i) {
            if (curr->__val == x) {
                ++c_erase;
                if (head_bucket == curr) {
                    head_bucket = head_bucket->__next;
                }
                curr = delete_el(curr);//удаляем curr и возвращааем указатель на prev
            } else {
                curr = curr->__next;
            }
        }

        sz_bucket -= c_erase;
        count_of_element -= c_erase;
        if(sz_bucket == 0) {
            head_bucket = nullptr;
        }
    }

    void insert(unsigned x) {
        auto & [head_bucket, sz_bucket] = arr[hash_function(x)];
        if (head_bucket == nullptr) { // добавление элемента, если бакет был пуст
            node * nxt = __head; 
            __head = create_new_node(x, nxt, nullptr);
            nxt->__prev = __head;
            head_bucket = __head;
        } else {
            node * nxt = head_bucket->__next;
            node * prv = head_bucket;
            node* new_node = create_new_node(x, nxt, prv);
            nxt->__prev = new_node;
            prv->__next = new_node;
        }
        ++sz_bucket;
        ++count_of_element;
    }

    size_t count(unsigned x) const {
        size_t __count = 0;
        auto [curr,  sz_bucket] = arr[hash_function(x)];
        for(size_t i = 0; i < sz_bucket; ++i) {
            if (curr->__val == x) {
                ++__count;
            }
            curr = curr->__next;
        }   
        return __count;
    } 

    // доступ к элементу 
    iterator find(unsigned x) const {
        auto [curr,  sz_bucket] = arr[hash_function(x)];
        for(size_t i = 0; i < sz_bucket; ++i, curr = curr->__next) {
            if (curr->__val == x) {
                return iterator(curr);
            }
        }   
        return end();
    }
    
    bool contain(unsigned x) const {
        return find(x) != end();
    }

    size_t count_bucket() const {
        return size_of_table;
    }
    // мощность множества 
    size_t power() const {
        return count_of_element;
    }

    bool empty() const {
        return __head == __end;
    }

    size_t bucket_size(size_t n) const {
        return arr[n % size_of_table].__size_curr_bucket;
    }
    
    void add_the_first_missing() {
        for(unsigned i = 0; true; ++i) {
            if (!contain(i)) {
                insert(i);
                break;
            }
        }
    }

    void clear() {
        // очистить список 
        __clear_list();
        // отавить таблицу в начальном состоянии 
        clear_table(arr, arr + size_of_table);
        count_of_element = 0;
    }

    ~hash_set() {
        clear();
        delete[]arr;
        delete(__end);  
    }
  
    iterator begin() const { return iterator(__head); } 
    iterator end() const { return iterator(__end); }
    

    // разность 
    void diff(const hash_set & hs) {
        for(auto iter = hs.begin(); iter!= hs.end(); ++iter) {
            erase_first_el(*iter);
        }
    }

    // объединение  
    void merge(const hash_set &hs) {
        for(auto iter = hs.begin(); iter!= hs.end(); ++iter) {
            if (count(*iter) < hs.count(*iter)) {
                insert(*iter);
            }
        }
    }
    

    // пересечение 
    void intersection(const hash_set& hs) {
        for(auto iter = begin(); iter != end(); ++iter) {
            if (count(*iter) > hs.count(*iter)) {
                erase_first_el(*iter);
            }
        }
    } 
};

std::ostream& operator<<(std::ostream& os, const hash_set & obj) {
    os << "[";
    auto curr = obj.begin();
    if (!obj.empty()) {
        auto next = ++obj.begin();
        for(; next != obj.end(); ++curr, ++next) {
            os << *curr << " ";
        }
        os << *curr;
    }
    os << "]";
    return os;
}

void ignore_spaces(std::istream& is) {
        while(is.peek() == ' ') {
            is.get();
        }
}

std::istream& operator>>(std::istream& is, hash_set& obj) {   
    obj.clear();
    

    ignore_spaces(is);

    if (is.peek() != '[') {
        throw "wrong read";
    }
    is.get(); 

    ignore_spaces(is);
    if (is.peek() == ']') {
            is.get();      
    } else  {
        while(is.peek() != ']') {
            ignore_spaces(is);
            
            int tmp;
            is >> tmp;
            obj.insert(tmp);
            ignore_spaces(is);
        }
        is.get();
    }

    ignore_spaces(is);
    if (is.peek() == '\n') {
        is.ignore();
    }
    return is;
}

unsigned max_v(unsigned x, unsigned y) {
    return x > y ? x : y; 
}

unsigned min_v(unsigned x, unsigned y) {
    return x < y ? x : y; 
}

int abs_v(int exp) {
    return exp > 0 ? exp : -exp;
} 

hash_set set_union(const hash_set & hs1, const hash_set & hs2) {
    hash_set x;
    for(auto c_iter = hs1.begin(); c_iter != hs1.end(); ++c_iter) {
        if (x.count(*c_iter) < max_v(hs1.count(*c_iter), hs2.count(*c_iter))) {
            x.insert(*c_iter);
        }
    }
    for(auto c_iter = hs2.begin(); c_iter != hs2.end(); ++c_iter) {
        if (x.count(*c_iter) < max_v(hs1.count(*c_iter), hs2.count(*c_iter))) {
            x.insert(*c_iter);
        }
    }

    return x;
}

hash_set set_intersec(const hash_set & hs1, const hash_set & hs2) {
    hash_set x;
    for(auto c_iter = hs1.begin(); c_iter != hs1.end(); ++c_iter) {
        if (x.count(*c_iter) < min_v(hs1.count(*c_iter), hs2.count(*c_iter))) {
            x.insert(*c_iter);
        }
    }
    for(auto c_iter = hs2.begin(); c_iter != hs2.end(); ++c_iter) {
        if (x.count(*c_iter) < min_v(hs1.count(*c_iter), hs2.count(*c_iter))) {
            x.insert(*c_iter);
        }
    }  
    
    return x;
} 

hash_set set_diff(const hash_set & hs1, const hash_set & hs2) {
    hash_set x;
    for(auto c_iter = hs1.begin(); c_iter != hs1.end(); ++c_iter) {
        if (static_cast<int>(x.count(*c_iter)) < 
                static_cast<int>(hs1.count(*c_iter)) - static_cast<int>(hs2.count(*c_iter))) {
            x.insert(*c_iter);
        }
    }
    return x;
}

hash_set set_symm_diff(const hash_set &hs1, const hash_set & hs2) {
    hash_set x;
    for(auto c_iter = hs1.begin(); c_iter != hs1.end(); ++c_iter) {
        if (static_cast<int>(x.count(*c_iter)) < 
                abs_v(static_cast<int>(hs1.count(*c_iter)) - static_cast<int>(hs2.count(*c_iter)))) {
            x.insert(*c_iter);
        }
    } 
    for(auto c_iter = hs2.begin(); c_iter != hs2.end(); ++c_iter) {
        if (static_cast<int>(x.count(*c_iter)) < 
                abs_v(static_cast<int>(hs1.count(*c_iter)) - static_cast<int>(hs2.count(*c_iter)))) {
            x.insert(*c_iter);
        }
    }
    return x;
}

bool operator==(const hash_set &hs1, const hash_set &hs2) {
    for(auto c_iter = hs1.begin(); c_iter != hs1.end(); ++c_iter) {
        if (hs1.count(*c_iter) != hs2.count(*c_iter)) {
            return false;
        }
    }

    for(auto c_iter = hs2.begin(); c_iter != hs2.end(); ++c_iter) {
        if (hs1.count(*c_iter) != hs2.count(*c_iter)) {
            return false;
        }
    }

    return true;
}

bool operator!=(const hash_set &hs1, const hash_set& hs2) {
    return !operator==(hs1, hs2);
}

#endif