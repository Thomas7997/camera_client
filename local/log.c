#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main () {
    while (1) {
        FILE * LOG = fopen("data/tmp/capture.txt", "r");
        
        char * buf = calloc(1000, sizeof(char));

        while (fgets(buf, 999, LOG)) {
            printf ("%s\n", buf);
        }

        free(buf);
        buf = NULL;
        
        fclose(LOG);
        usleep(5000);
    }

    return 0;
}
