#include <stdio.h>
#include <stdlib.h>

void kill_process (void) {
    system("systemctl stop capture");
}

void start_process (void) {
    system("systemctl start capture");
}

int main (void) {
    while (1) {
        FILE * SCRIPT = fopen("commands/capture.txt", "r");

        int command = -1, last = 0;

        fscanf(SCRIPT, "%d", &command);

        if (command == 0) {
            last = command;
            kill_process();
        }

        else if (command == 1) {
            if (last == 0) {
                last = command;
                start_process();
            }
        }

        fclose(SCRIPT);
    }

    return 0;   
}