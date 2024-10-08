#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include "wyklad13b.h"
#include "string.h"
#include "iostream"
#include "ctime"
#include <fstream>
#include <iomanip>
#include <Windows.h>
using namespace std;

struct Dane {
  int id;
  char nazwa[10];
  double temp;
  double pojemn;

  Dane& operator +=(const Dane& tmp) {
    this->id = 0;
    strcpy(this->nazwa, "         ");
    this->temp += tmp.temp;
        this->pojemn += tmp.pojemn;
    return *this;
  }
};


ostream& operator << (ostream& stream, const struct Dane dane) {
  return   stream << '|' 
    << setfill('0') << setw(6) << dane.id
    << '|'
    << left << setfill(' ') << setw(10) << dane.nazwa
    << '|'
  << right << setw(7) << dane.temp    << '|'    << setw(6) << dane.pojemn
    << '|' << endl;
}

void wpisz_naglowek(ofstream& file) {
  time_t current_time = time(nullptr);
  tm* local_time = localtime(&current_time);
  char* time_str = asctime(local_time);
  string name = getenv("COMPUTERNAME");

  file << right << setw(34) << time_str;
  file << "\n\n\n" << name << '\n';
}

void wpisz_linie_tablicy(ofstream& file) {
  file << "+------+----------+-------+------+" << endl;
}

void wpisz_naglowek_tablicy(ofstream& file) {
  wpisz_linie_tablicy(file);
  file << '|';
  file << "  ID  ";
  file << '|';
  file << "   nazwa  ";
  file << '|';
  file << " temp. ";
  file << '|';
  file << std::setw(6) << "pojemn";
  file << '|';
  file << endl;
  wpisz_linie_tablicy(file);
}

template <typename El>
void generuj_raport(El &tab, const char* nazwa) {
  ofstream file;

  file.open(nazwa, ios::app);

  wpisz_naglowek(file);
  wpisz_naglowek_tablicy(file);
  Dane tmp;
  tmp.temp = 0;
  tmp.pojemn = 0;
  for (int i = 0; i < tab.getSize(); i++) {
    file << tab[i];
    tmp += tab[i];
  }
  wpisz_linie_tablicy(file);
  file << tmp;
  wpisz_linie_tablicy(file);

  file << "\n\n\n";
  file.close();
}

int main() {
  Tablica<Dane, 2> Tab;

  for (int i = 0; i < 2; i++) {
    Tab[i].id = i + 1;
    Tab[i].nazwa[0] = 'a' + i;  
    Tab[i].nazwa[1] = '\0';   
    Tab[i].temp = (i + 1) * 10;
    Tab[i].pojemn = (i + 1) * 2;
  }

  generuj_raport(Tab, "plik.txt");

  return 0;
}
