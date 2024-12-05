#include <stdio.h>
#include <stdlib.h>

int main() {
    // Wyświetlamy komunikat, że aktualizacja pakietów jest w toku
    printf("Rozpoczynam aktualizację pakietów systemowych...\n");

    // Uruchamiamy polecenie do aktualizacji pakietów systemowych
    // Używamy system() do wykonania polecenia w powłoce
    int result = system("sudo apt-get update && sudo apt-get upgrade -y");

    // Sprawdzamy, czy polecenie się powiodło
    if (result == 0) {
        printf("Pakiety zostały zaktualizowane pomyślnie.\n");
    } else {
        printf("Wystąpił błąd podczas aktualizacji pakietów.\n");
    }

    return 0;
}
