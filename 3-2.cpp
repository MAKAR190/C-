#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <limits>

using namespace std;

// Szablon klasy Konten
template <typename T>
class Konten {
    T* dane;
    int pojemnosc;
    int szczyt;

public:
    // Konstruktor domyślny
    Konten() : dane(nullptr), pojemnosc(0), szczyt(0) {}

    // Konstruktor z parametrem maksymalnego rozmiaru
    Konten(int maks_rozmiar) : pojemnosc(maks_rozmiar), szczyt(0) {
        dane = new T[pojemnosc];
    }

    // Konstruktor kopiujący
    Konten(const Konten& inny) : pojemnosc(inny.pojemnosc), szczyt(inny.szczyt) {
        dane = new T[pojemnosc];
        for (int i = 0; i < szczyt; ++i) {
            dane[i] = inny.dane[i];
        }
    }

    // Konstruktor przenoszący
    Konten(Konten&& inny) noexcept : dane(inny.dane), pojemnosc(inny.pojemnosc), szczyt(inny.szczyt) {
        inny.dane = nullptr;
        inny.pojemnosc = 0;
        inny.szczyt = 0;
    }

    // Operator przypisania kopiującego
    Konten& operator=(const Konten& inny) {
        if (this == &inny) return *this;

        delete[] dane;
        pojemnosc = inny.pojemnosc;
        szczyt = inny.szczyt;
        dane = new T[pojemnosc];
        for (int i = 0; i < szczyt; ++i) {
            dane[i] = inny.dane[i];
        }
        return *this;
    }

    // Operator przypisania przenoszącego
    Konten& operator=(Konten&& inny) noexcept {
        if (this == &inny) return *this;

        delete[] dane;
        dane = inny.dane;
        pojemnosc = inny.pojemnosc;
        szczyt = inny.szczyt;

        inny.dane = nullptr;
        inny.pojemnosc = 0;
        inny.szczyt = 0;
        return *this;
    }

    ~Konten() {
        delete[] dane;
    }

    void dodaj(T wartosc) {
        assert(szczyt < pojemnosc);
        dane[szczyt++] = wartosc;
    }

    T usun() {
        assert(szczyt > 0);
        return dane[--szczyt];
    }

    int rozmiar() const { return szczyt; }

    T& operator[](int indeks) {
        assert(indeks < szczyt);
        return dane[indeks];
    }

    class iterator {
        Konten& kontener;
        int indeks;
    public:
        iterator(Konten& k, int start_indeks = 0) : kontener(k), indeks(start_indeks) {}

        iterator& operator++() {
            ++indeks;
            return *this;
        }

        T& operator*() const {
            return kontener.dane[indeks];
        }

        bool operator!=(const iterator& inny) const {
            return indeks != inny.indeks;
        }
    };

    iterator poczatek() { return iterator(*this, 0); }
    iterator koniec() { return iterator(*this, szczyt); }
};

// Szablon klasy pomiary
template <typename T>
class pomiary {
public:
    Konten<T> przed;
    Konten<T> po;

    // Konstruktor domyślny z inicjalizacją kontenerów `przed` i `po` o domyślnym rozmiarze
    pomiary() : przed(10), po(10) {}

    // Konstruktor z określeniem rozmiaru
    pomiary(int rozmiar) : przed(rozmiar), po(rozmiar) {}
};

// Szablon klasy Kalkulator
template <typename T>
class Kalkulator {
public:
    static void testtStudenta(
        typename Konten<pomiary<T>>::iterator& pierwszy,
        typename Konten<pomiary<T>>::iterator& drugi) {
        
        double min_t_wartosc = std::numeric_limits<double>::max();
        typename Konten<pomiary<T>>::iterator najlepszy_przed, najlepszy_po;

        for (typename Konten<pomiary<T>>::iterator it = pierwszy; it != drugi; ++it) {
            pomiary<T>& para_pomiarow = *it;

            double suma_przed = 0.0, suma_po = 0.0;
            int n_przed = para_pomiarow.przed.rozmiar();
            int n_po = para_pomiarow.po.rozmiar();

            for (int i = 0; i < n_przed; ++i) suma_przed += para_pomiarow.przed[i];
            for (int i = 0; i < n_po; ++i) suma_po += para_pomiarow.po[i];

            double srednia_przed = suma_przed / n_przed;
            double srednia_po = suma_po / n_po;

            double wariancja_przed = 0.0, wariancja_po = 0.0;

            for (int i = 0; i < n_przed; ++i) wariancja_przed += pow(para_pomiarow.przed[i] - srednia_przed, 2);
            for (int i = 0; i < n_po; ++i) wariancja_po += pow(para_pomiarow.po[i] - srednia_po, 2);

            wariancja_przed /= n_przed;
            wariancja_po /= n_po;

            double t_wartosc = abs(srednia_przed - srednia_po) /
                               sqrt((wariancja_przed / n_przed) + (wariancja_po / n_po));

            if (t_wartosc < min_t_wartosc) {
                min_t_wartosc = t_wartosc;
                najlepszy_przed = it;
                najlepszy_po = it;
            }
        }

        pierwszy = najlepszy_przed;
        drugi = najlepszy_po;
    }
};

// Funkcja do generowania losowych wartości
double generuj_losowa_liczbe(double min, double max) {
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    int m = 6 + rand() % 5;  // 5 < m < 10
    Konten<pomiary<double>> kontener_pomiarow(m);

    for (int i = 0; i < m; ++i) {
        int n = 6 + rand() % 5;  // 5 < n < 10
        pomiary<double> para_pomiarow(n);

        for (int j = 0; j < n; ++j) para_pomiarow.przed.dodaj(generuj_losowa_liczbe(0, 100));
        for (int j = 0; j < n; ++j) para_pomiarow.po.dodaj(generuj_losowa_liczbe(0, 100));

        kontener_pomiarow.dodaj(para_pomiarow);
    }

    typename Konten<pomiary<double>>::iterator pierwszy = kontener_pomiarow.poczatek();
    typename Konten<pomiary<double>>::iterator drugi = kontener_pomiarow.koniec();

    Kalkulator<double>::testtStudenta(pierwszy, drugi);

    cout << "Najlepsza para pomiarow z minimalna wartoscia T znaleziona." << endl;
    
    return 0;
}
