#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Sprawdzamy, czy program otrzymał dokładnie 3 argumenty
    if (argc != 4) {
        printf("Błąd! Program wymaga trzech argumentów: nazwa_uzytkownika, haslo, grupa.\n");
        return 1;
    }

    // Przypisanie argumentów do zmiennych
    char *nazwa_uzytkownika = argv[1];
    char *haslo = argv[2];
    char *grupa = argv[3];

    // Tworzymy polecenie do dodania użytkownika
    char command[256];
    snprintf(command, sizeof(command), "sudo useradd -m -G %s %s", grupa, nazwa_uzytkownika);

    // Wywołanie polecenia useradd
    int result = system(command);
    if (result != 0) {
        printf("Błąd podczas dodawania użytkownika %s do grupy %s.\n", nazwa_uzytkownika, grupa);
        return 1;
    }

    // Tworzymy polecenie do ustawienia hasła użytkownika
    snprintf(command, sizeof(command), "echo '%s:%s' | sudo chpasswd", nazwa_uzytkownika, haslo);

    // Wywołanie polecenia chpasswd
    result = system(command);
    if (result != 0) {
        printf("Błąd podczas ustawiania hasła dla użytkownika %s.\n", nazwa_uzytkownika);
        return 1;
    }

    // Informujemy użytkownika o sukcesie
    printf("Użytkownik %s został pomyślnie dodany do systemu oraz przypisany do grupy %s.\n", nazwa_uzytkownika, grupa);
    return 0;
}
