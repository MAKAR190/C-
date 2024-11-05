#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <limits>

using namespace std;

// Kontener klasy Konten
template <typename T>
class Konten {
    T* dane;
    int pojemnosc;
    int szczyt;
public:
    Konten(int maks_rozmiar) : pojemnosc(maks_rozmiar), szczyt(0) {
        dane = new T[pojemnosc];
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

// Klasa pomiary, przechowująca przed i po pomiary
template <typename T>
class pomiary {
public:
    Konten<T> przed;
    Konten<T> po;

    pomiary(int rozmiar) : przed(rozmiar), po(rozmiar) {}
};

// Funkcja do generowania losowych wartości double
double losowaWartosc() {
    return rand() / (double(RAND_MAX) + 1) * 100;
}

// Klasa Kalkulator
template <typename T>
class Kalkulator {
public:
    static void testtStudenta(
        typename Konten<pomiary<T>>::iterator &min_para,
        typename Konten<pomiary<T>>::iterator poczatek,
        typename Konten<pomiary<T>>::iterator koniec
    ) {
        double min_T = std::numeric_limits<double>::max();

        for (typename Konten<pomiary<T>>::iterator it = poczatek; it != koniec; ++it) {
            pomiary<T>& pomiar = *it;
            int n = pomiar.przed.rozmiar();
            if (n <= 1) continue; // Unikamy dzielenia przez zero

            double srednia_D = 0;
            for (int j = 0; j < n; ++j) {
                srednia_D += (pomiar.przed[j] - pomiar.po[j]);
            }
            srednia_D /= n;

            double S_kwadrat = 0;
            for (int j = 0; j < n; ++j) {
                double roznica = (pomiar.przed[j] - pomiar.po[j]) - srednia_D;
                S_kwadrat += roznica * roznica;
            }
            double S = std::sqrt(S_kwadrat / (n - 1));
            double T = srednia_D / (S / std::sqrt(n));

            if (std::abs(T) < min_T) {
                min_T = std::abs(T);
                min_para = it;
            }
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Tworzymy kontener typu Konten przechowujący pomiary<double>
    Konten<pomiary<double>> pomiaryKontener(10);

    // Generowanie par serii wartości losowych i dodawanie ich do kontenera
    for (int i = 0; i < 7; ++i) {
        int dlugosc_serii = 5 + rand() % 5;
        pomiary<double> seria(dlugosc_serii);

        for (int j = 0; j < dlugosc_serii; ++j) {
            seria.przed.dodaj(losowaWartosc());
            seria.po.dodaj(losowaWartosc());
        }

        pomiaryKontener.dodaj(seria);
    }

    // Tworzenie obiektu Kalkulator i wywołanie metody testtStudenta
    auto poczatek = pomiaryKontener.poczatek();
    auto koniec = pomiaryKontener.koniec();
    auto min_para = poczatek;

    Kalkulator<double>::testtStudenta(min_para, poczatek, koniec);

    // Wyświetlanie wyników
    cout << "Para serii o najniższej wartości T: " << endl;
    pomiary<double>& wynik = *min_para;
    cout << "Przed: ";
    for (int i = 0; i < wynik.przed.rozmiar(); ++i) {
        cout << wynik.przed[i] << " ";
    }
    cout << "\nPo: ";
    for (int i = 0; i < wynik.po.rozmiar(); ++i) {
        cout << wynik.po[i] << " ";
    }
    cout << endl;

    return 0;
}
