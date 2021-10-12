#include "scripts.h"

void mirroir (char * buf, unsigned int n) {
    int i = 0;
    char car;

    for (i = 0; i < (n+1)/2; i++) {
        car = buf[n-i-1];
        buf[n-i-1] = buf[i];
        buf[i] = car;
    }
}

void enlever_last_car(char *chaine) {
    chaine[strlen(chaine)-1] = 0;
}

void clearList (char ** list) {
    unsigned int x = 0;

    for (char * str = *list; str != 0; str = *(list+x), x++) {
        strcpy(str, "");
    }
}

void clearStr (char * str) {
    for (unsigned int i = 0; str[i]; i++) {
        str[i] = 0;
    }
}

void dislpayList(char ** list) {
    printf("Displaying list ...\n");

    for (unsigned int x = 0; *list[x]; x++) {
        printf("%s\n", list[x]);
    }

    printf("List displayed !\n");
}

char * getName (char * buf, char * dossier) {
    unsigned int n = strlen(buf), x = 0, y = 0;
    char * buffer = (char*) calloc(100, sizeof(char));

    unsigned int i = n-1;

    while (buf[i] != '/') {
        buffer[x++] = buf[i--];
    }

    while (i > 0) {
        dossier[y++] = buf[i-1];
        i--;
    }

    mirroir(buffer, x);
    mirroir(dossier, y);

    return buffer;
}

int find_dir_filename (char ** files, const char * name, char * dir) {
    char * tmpDir = (char*) calloc(50, sizeof(char));
    
    for (int f = 0; *files[f]; f++) {
        char * filename = getName(files[f], tmpDir);
        printf("%s\n", tmpDir);
        if (!strcmp(filename, name)) {
            strcpy(dir, tmpDir);
            // free(tmpDir);
            return f; // index
        }

        strcpy(tmpDir, "");
        free(filename);
    }

    free(tmpDir);
    
    return -1; // Not found
}

void operation_finished (const char * path, const char * name) {
    FILE * R = fopen(path, "r");

    char ** names = (char**) calloc(MAX_DOWNLOADS, sizeof(char*));

    for (int i = 0; i < MAX_DOWNLOADS; i++) {
        names[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    unsigned int x = 0, index, n;
    while (fgets(names[x], 99, R)) {
        enlever_last_car(names[x]);
        printf("%s\n%s\n%d\n", names[x], name, x);
        if (!strncmp(names[x], name, strlen(name))) index = x;
        x++;
    }

    n = x;
    x = 0;

    fclose(R);
    FILE * W = fopen(path, "w");

    for (x = 0; x < index; x++) {
        fprintf(W, "%s\n", names[x]);
    }

    for (x = index+1; x < n; x++) {
        fprintf(W, "%s\n", names[x]);
    }

    for (int i = 0; i < MAX_DOWNLOADS; i++) {
        free(names[i]);
        names[i] = NULL;
    }

    free(names);
    names = NULL;
    fclose(W);
}