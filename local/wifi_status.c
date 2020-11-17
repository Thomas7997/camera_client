#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mirroir (char * buf, unsigned int n) {
    int i = 0;
    char car;

    for (i = 0; i < (n+1)/2; i++) {
        car = buf[n-i-1];
        buf[n-i-1] = buf[i];
        buf[i] = car;
    }
}

int main (void) {
    system("nmcli device wifi > data/tmp/wifi.txt");
    FILE * WIFI = fopen("data/tmp/wifi.txt", "r");
    char ** txt = (char**) calloc(100, sizeof(char*));
    char * signal_str = (char*) calloc(4, sizeof(char));

    for (int i = 0; i < 100; i++) {
        txt[i] = (char*) calloc(300, sizeof(char));
    }

    unsigned int x = 0, size, connected = 0, signal = 0, y, u;

    while (fgets(txt[x++], 299, WIFI));
    size = x;

    for (x = 1; x < size; x++) {
        if (txt[x][0] == '*') {
            connected = 1;
            u = 0;
            y = strlen(txt[x]) - 1;
            while (txt[x][y--] == ' ');
            while (txt[x][y--] != ' ');
            y -= 2;
            while (txt[x][y--] != ' ');
            while (txt[x][y--] == ' ');
            while (txt[x][y] != ' ') {
                signal_str[u++] = txt[x][y--];
            }

            printf ("%s\n", signal_str);

            mirroir(signal_str, u);
            sscanf(signal_str, "%d", &signal);
            signal = signal / 4;

            break;
        }
    }

    // On pourrait aussi faire d'autres opérations comme la puissance du signal du wifi

    FILE * STATUS = fopen("data/tmp/wifi_status.txt", "w");
    FILE * SIGNAL = fopen("data/tmp/wifi_signal.txt", "w");
    fprintf(STATUS, "%d", connected);
    fclose(STATUS);

    fprintf(SIGNAL, "%d", signal);

    for (int i = 0; i < 100; i++) {
        free(txt[i]);
    }

    free(txt);
    fclose(WIFI);
    fclose(SIGNAL);
    free(signal_str);

    return 0;
}