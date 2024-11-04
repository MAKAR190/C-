#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>

template <typename T>
class Konten {
public:
    T* data;            
    int size;        

    Konten(int n) : size(n) {
        data = new T[size];
    }

    ~Konten() {
        delete[] data;
    }

    T& operator[](int index) {
        return data[index];
    }
};

template <typename T>
class Kalkulator {
public:
    static void NajmOdleglosc(
        Konten<T>*& pierwszy,
        Konten<T>*& drugi,
        Konten<Konten<T>*>& punkty,
        int liczbaPunktow
    ) {
        double min_odleglosc = std::numeric_limits<double>::max();

        for (int i = 0; i < liczbaPunktow; ++i) {
            for (int j = i + 1; j < liczbaPunktow; ++j) {
                double odleglosc = obliczOdleglosc(*punkty[i], *punkty[j]);
                if (odleglosc < min_odleglosc) {
                    min_odleglosc = odleglosc;
                    pierwszy = punkty[i];
                    drugi = punkty[j];
                }
            }
        }
    }

private:
    static double obliczOdleglosc(const Konten<T>& p1, const Konten<T>& p2) {
        double suma = 0;
        for (int i = 0; i < p1.size; ++i) {
            suma += std::pow(p1.data[i] - p2.data[i], 2);
        }
        return std::sqrt(suma);
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    const int m = 7;  
    const int n = 6;  

    Konten<Konten<double>*> punkty(m);

    for (int i = 0; i < m; ++i) {
        punkty[i] = new Konten<double>(n);
        for (int j = 0; j < n; ++j) {
            punkty[i]->data[j] = static_cast<double>(rand()) / RAND_MAX * 10.0;
        }
    }

    Konten<double>* pierwszy = nullptr;
    Konten<double>* drugi = nullptr;
    Kalkulator<double>::NajmOdleglosc(pierwszy, drugi, punkty, m);

    std::cout << "Najmniejsza odleglosc jest miedzy punktami:\n";
    std::cout << "Punkt 1: ";
    for (int i = 0; i < n; ++i) {
        std::cout << pierwszy->data[i] << " ";
    }
    std::cout << "\nPunkt 2: ";
    for (int i = 0; i < n; ++i) {
        std::cout << drugi->data[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < m; ++i) {
        delete punkty[i];
    }

    return 0;
}
