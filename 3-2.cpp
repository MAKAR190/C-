#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <limits>

using namespace std;

// Kontener klasy Konten
template <typename T>
class Konten {
    T* data;
    int capacity;
    int top;
public:
    Konten(int max_size) : capacity(max_size), top(0) {
        data = new T[capacity];
    }

    ~Konten() {
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

    int size() const { return top; }

    T& operator[](int index) {
        assert(index < top);
        return data[index];
    }

    class iterator {
        Konten& kontener;
        int index;
    public:
        iterator(Konten& k, int start_index = 0) : kontener(k), index(start_index) {}

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

// Klasa pomiary, przechowująca przed i po pomiary
template <typename T>
class pomiary {
public:
    Konten<T> przed;
    Konten<T> po;

    pomiary(int size) : przed(size), po(size) {}
};

// Funkcja do generowania losowych wartości double
double randdouble() {
    return rand() / (double(RAND_MAX) + 1) * 100;
}

// Klasa Kalkulator
template <typename T>
class Kalkulator {
public:
    static void testtStudenta(
        typename Konten<pomiary<T>>::iterator &start,
        typename Konten<pomiary<T>>::iterator &end
    ) {
        typename Konten<pomiary<T>>::iterator min_pair = start;
        double min_T = std::numeric_limits<double>::max();

        for (int i = 0; start != end; ++start, ++i) {
            pomiary<T>& pom = *start;
            int n = pom.przed.size();
            
            double D_mean = 0;
            for (int j = 0; j < n; ++j) {
                D_mean += (pom.przed[j] - pom.po[j]);
            }
            D_mean /= n;

            double S_squared = 0;
            for (int j = 0; j < n; ++j) {
                double diff = (pom.przed[j] - pom.po[j]) - D_mean;
                S_squared += diff * diff;
            }
            double S = std::sqrt(S_squared / (n - 1));
            double T = D_mean / (S / std::sqrt(n));

            if (std::abs(T) < min_T) {
                min_T = std::abs(T);
                min_pair = start;
            }
        }

        start = min_pair;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Tworzymy kontener typu Konten przechowujący pomiary<double>
    Konten<pomiary<double>> measurements(10);

    // Generowanie par serii wartości losowych i dodawanie ich do kontenera
    for (int i = 0; i < 7; ++i) {
        int series_length = 5 + rand() % 5;
        pomiary<double> series(series_length);

        for (int j = 0; j < series_length; ++j) {
            series.przed.push(randdouble());
            series.po.push(randdouble());
        }

        measurements.push(series);
    }

    // Tworzenie obiektu Kalkulator i wywołanie metody testtStudenta
    auto start = measurements.begin();
    auto end = measurements.end();

    Kalkulator<double>::testtStudenta(start, end);

    // Wyświetlanie wyników
    cout << "Para serii o najniższej wartości T: " << endl;
    pomiary<double>& result = *start;
    cout << "Przed: ";
    for (int i = 0; i < result.przed.size(); ++i) {
        cout << result.przed[i] << " ";
    }
    cout << "\nPo: ";
    for (int i = 0; i < result.po.size(); ++i) {
        cout << result.po[i] << " ";
    }
    cout << endl;

    return 0;
}
