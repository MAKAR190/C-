#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>

using namespace std;

template <typename T>
class Kontener {
    T* data;
    int capacity;
    int top;
public:
    Kontener(int max_size) : capacity(max_size), top(0) {
        data = new T[capacity];
    }

    ~Kontener() {
        delete[] data;
    }

    void push(T value) {
        assert(top < capacity);
        data[top++] = value;
    }

    T pop() {
        assert(top > 0); 
        return data[--top];
    }

    int rozmiar() const { return top; }

    class iterator {
        Kontener& kontener;
        int index;
    public:
        iterator(Kontener& k, int start_index = 0) : kontener(k), index(start_index) {}

        iterator& operator++() {
            ++index;
            return *this;
        }

        T& operator*() const {
            return kontener.data[index];
        }

        bool operator!=(const iterator& other) const {
            return index != other.index;
        }
    };

    iterator begin() { return iterator(*this, 0); }
    iterator end() { return iterator(*this, top); }
};

double randdouble() {
    return rand() / (double(RAND_MAX) + 1);
}

template <typename Iter1, typename Iter2>
void Roznicuj(Iter1 i1, Iter1 i2, Iter2 i3) {
    while (i1 != i2) {
        *i1 = *i1 - *i3;
        ++i1;
        ++i3;
    }
}

template <typename Iter>
void DoKwadratu(Iter i1, Iter i2) {
    while (i1 != i2) {
        *i1 = (*i1) * (*i1);
        ++i1;
    }
}

template <typename Iter, typename T>
void Sumuj(Iter i1, Iter i2, T& a) {
    while (i1 != i2) {
        a += *i1;
        ++i1;
    }
}

int main() {
    srand(static_cast<unsigned>(time(0))); 

    const int n = 100;  

    Kontener<double> p(n);
    Kontener<double> q(n);

    for (int i = 0; i < n; ++i) {
        p.push(randdouble() * 10);  
        q.push(randdouble() * 10);  
    }

    Roznicuj(p.begin(), p.end(), q.begin());

    DoKwadratu(p.begin(), p.end());

    double suma = 0;
    Sumuj(p.begin(), p.end(), suma);

    double dlugosc_wektora = sqrt(suma);

    cout << "Dlugosc wektora laczacego punkty p i q: " << dlugosc_wektora << endl;

    return 0;
}
