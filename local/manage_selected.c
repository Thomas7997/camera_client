#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <time.h>
#define MAX 80

#define MAX_CAPTURES 100000
#define TAILLE_NOM 100
#define TMaxL 1000
#define TMax 100
#define MAX_DIRS 20

// Envoi de la photo
void send_request (char *name) {
    printf("Requêtes\n");
    
    CURL *curl;
    CURLcode res;

    char * request_string = calloc(10000000, sizeof(char));
    sprintf(request_string, "name=%s", name);

    
    curl_global_init(CURL_GLOBAL_ALL);
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/transfert/photo");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_string);
    
        res = curl_easy_perform(curl); 
        if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));
    
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    printf ("\n");
}

void vider_buffer (char *buffer) {
    for (int i = 0; i < strlen(buffer); i++) {
        buffer[i] = 0;
    }
}

void afficher_tab2 (char ** tab) {
    for (int i = 0; i < MAX_CAPTURES; i++) {
        for (int j = 0; j < TAILLE_NOM; j++) {
            printf ("%c", tab[i][j]);
        }
        printf ("\n");
    }
}

// Il faut enlever l'extension
void transform_noms (char ** liste, char ** nouvelleListe, int size) {
    int i = 0, j;

    for (i = 0; i < size; i++) {
        j = 0;
        while (liste[i][j] != '.') {
            nouvelleListe[i][j] = liste[i][j];
            printf ("%c", nouvelleListe[i][j]);
            j++;
        }
        printf ("\n");
    }
}

void linearize (char *base, char **lines) {
    int i = 0, j = 0, x = 0;

    while (lines[i][0] != 0) {
        j = 0;
        while (lines[i][j] != 0 && lines[i][j+1] != '\n') {
            base[x++] = lines[i][j++];
        }
        i++;
    }
}

int compare_file_historique (char * file, char ** historique, int lines) {
    int i;

    for (i = 0; i < lines; i++) {
        if (strcmp(historique[i], file) == 0) {
           return 1; 
        }
    }

    return 0;
}

// Pour l'envoi
void transferer_noms (char ** liste) {
    printf ("Transferts\n\n\n");

    int i = 0;

    char commande[250] = "";
    int file_transfered = 0;

    FILE * HISTORIQUE = fopen("data/images/historique.txt", "a+");

    char ** hist_lines = calloc(MAX_CAPTURES, sizeof(char*));

    int x;

    for (x = 0; x < MAX_CAPTURES; x++) {
        hist_lines[x] = calloc(TAILLE_NOM, sizeof(char));
    }

    x = 0;

    while (fgets(hist_lines[x++], TAILLE_NOM, HISTORIQUE));

    char * current_file = calloc(TAILLE_NOM, sizeof(char));

    while (liste[i][0] != 0) {
        // Il y aura peut être besoin d'insérer les lignes précédentes dans cette boucle

        sprintf(current_file, "%s", liste[i]);

        file_transfered = compare_file_historique(current_file, hist_lines, x);

        if (file_transfered == 0) {
            sprintf(commande, "gphoto2 --get-file=\"%s\";mv ./data/images/gets/%s /home/thomas/camera_server/public", liste[i], liste[i]);

            printf ("%s\n", commande);

            // Envoyer le nom du nouveau fichier transféré au socket
            system(commande);

            // ÉCIRE DANS L'HISTORIQUE DES TRANSFERTS
            fprintf(HISTORIQUE, "%s\n", liste[i]);

            send_request(liste[i]);
            
            i++;
        }
    }

    for (x = 0; x < MAX_CAPTURES; x++) {
        free(hist_lines[x]);
    }

    free(current_file);
    fclose(HISTORIQUE);
    free(hist_lines);
}

void enlever_last_car(char *chaine) {
    chaine[strlen(chaine)-1] = 0;
}

void select_medias () {
    system("cd data/images/cloud;ls *.JPG > ../cloud.txt;cd ../../..");
    FILE * CLOUD = fopen("./data/images/cloud.txt", "r");

    fclose(CLOUD);
}

void getFiles (void) {
    // system("cd data/images/gets;rm -f *;gphoto2 --get-all-files;ls *.JPG > ../gets.txt;cd ../../..");
    system("./rates.sh");
}

unsigned int parseRating (char * line) {
    int i;

    return line[strlen(line)-2] - '0';
}

/*
unsigned int getRating (char * file) {
    char * commande = calloc(300, sizeof(char));
    printf ("Rating\n%s\nsize : %d\n", file, strlen(file));

    sprintf(commande, "cd data/images/gets;exiv2 %s -g Rating > ../tmp/exif.txt;cd ../../..", file);
    // sprintf(commande, "cd data/images/gets;echo $(exiftool -filename -imagesize -exif:fnumber -xmp:all %s) > ../tmp/exif.txt;cd ../../..", file);
    system(commande);
    // execl("/bin/bash", "bash", "-c", commande, (char *) NULL);
    printf ("%s\n", commande);

    // Lecture
    int x = 0;

    printf ("Getting lines\n");

    // Remplissage
    while (fgets(lignes[x], 999, RATING)) {
        printf ("%s\n", lignes[x++]);
    }

    unsigned int rating = parseRating(lignes);

    printf ("Finished\n");

    for (int i = 0; i < 10; i++) {
        free(lignes[i]);
    }

    fclose(RATING);
    free(lignes);
    free(commande);

    return rating;
}
*/

void parseRatings (unsigned int * ratings, char ** lines, unsigned int size) {
    // A coder
    // Lecture en imparité d'indexs

    int i = 1, x = 0;

    char * line = calloc(100, sizeof(char));

    while (i < 2*size+1) { // OU x < size
        strcpy(line, lines[i]);
        ratings[x] = parseRating(line);
        printf ("%d\n", x++);
        
        i += 2;
    }

    free(line);
}


// A finir car ca ne fonctionne que sur le premier élément (dossier)

int select_dir (unsigned char * dossiers, FILE * File) {
    int k=0;
    int l=0;
    int r=0;
    char TAB1[TMaxL] = "";
    char tmp[100]={0};
    int tab[TMax]={0};
    char code[TMax]="";
    char TAB2[10]="Le dossier";
    char TAB3[5]="aucun";
    File = fopen("files.txt", "r");
    if (File == NULL) {
        exit(0);
    }
   
    int rado=0;
    int longueur=0;
    for(int aze=0; aze<50000; aze++) {
        fgets(tmp,2000,File);
        while(tmp[rado]!=0) { 
            TAB1[longueur]=tmp[rado];
            longueur=longueur+1;
            rado=rado+1;
        }

        for(int zer=0; zer < 100; zer++) {
            tmp[zer]=0;
        }
        
        rado=0;
    }

    int x, c, same = 1;

    for (c = 0; c < longueur; c++) {
        same = 1;
        for (x = 0; x < 10; x++) {
            if (TAB1[c+x] != TAB2[x]) {
                same = 0;
                break;
            }
        }

        if (same == 1) {
            tab[k]=c+5;
            k=k+1;
        }
    }

    int g=0;
    for(int rty=0;rty<k;rty++) {
        g=tab[rty];
        for (int h=0; h<60; h++) {
            same = 1;
            for (int y = 0; y < 4; y++) {
                if(TAB1[g+h+y]!=TAB3[y]) {
                    same = 0;
                    break;
                }
            }

            if (same == 1) {
                tab[rty]=0;
            }
        }
    }
    int q=0;
    int t=0;

    char * result = calloc(100, sizeof(char));

    for(int s=0; s < k; s++) {
        if(tab[s]!=0) {
            // printf("%c",tab[s]);
            t=tab[s];
            for (int a=0; a<100; a++) {
                if (TAB1[t+a]=='/') {
                    while (TAB1[t+a+q]!='c') {
                        code[q]=TAB1[t+a+q];
                        q++;
                    }
                    goto label;
                }
            }
        }
    }

    label: {
        for (int i = 0; i < strlen(code)-3; i++) {
            result[i] = code[i];
        }
        
        printf("%s", result);
        printf("\n");
    }

    free(result);
        
    return 0;
}

void clearBufLast (char * buf, unsigned int len, unsigned int nb) {
    int i;

    for (i = len; i >= len - nb; i--)
        buf[i] = 0;
    return;
}

unsigned int read_dir_list (char ** dirs, char ** lines, unsigned int nb, unsigned int * ref) {
    int i, j, x = 0, o = 0, y;
    char * current_line = calloc(100, sizeof(char));
    char key = 0;

    for (i = 0; i < nb; i++) {
        if (lines[i][0] != '#') {
            printf ("%s\n", lines[i]);
            strcpy(current_line, "");
            y = 1;
            j = 0;
            current_line[0] = '/';
            while (lines[i][j++] != '/');

            while (lines[i][j] != 'n' && lines[i][j] != 'c') {
                current_line[y++] = lines[i][j++];
            }

            key = current_line[y-1];
            clearBufLast(current_line, y, 5);

            printf ("%s\n%d\n", current_line, y);

            if (key == 'c') {
                strcpy(dirs[x++], current_line);
            }

            else if (key != 'n') {
                printf ("Index error : %c\n", key);
            }

            else {
                printf ("Vide\n");
            }
        }
    }

    free(current_line);

    return x+1; // Nombre de dossiers contenant des fichiers
}

unsigned int read_file_list (char ** files, char ** lines, unsigned int nb, unsigned int starts, unsigned int start) {
    unsigned int line_size, x = 0, i, y;

    // Parser la liste de fichiers
    for (i = starts; i < nb; i++) {
        if (lines[i][0] == '#') {
            for (y = 7; y <= 12; y++) {
                files[x][y-7] = lines[i][y];
            }
            x++;
        }

        else if (x != 0) break;
    }

    return x+1;
}

unsigned int get_files_and_dirs (char *** dirs_b, char ** lines, unsigned int nb, unsigned int * sizes_list) {
    // Executer read_dir_list, read_file_list puis blockerize ...
    unsigned int ref_line;
    char ** files = calloc(10000, sizeof(char*));
    char ** dirs = calloc(10000, sizeof(char*));
    unsigned int x, y, z;

    printf ("Parsing ...\n");

    for (int i = 0; i < 10000; i++) {
        files[i] = calloc(100, sizeof(char));
        dirs[i] = calloc(100, sizeof(char));
    }

    unsigned int lines_nb = read_dir_list(dirs, lines, nb, &ref_line), tmp_size, tmp_y = 0;

    for (x = 0; x < lines_nb; x++) {
        tmp_size = read_file_list(files, lines, nb, ref_line, z); // ref_line est les lignes où commencer
        for (y = 0; y < tmp_size; y++) {
            strcpy(dirs_b[x][y], "");
            sprintf(dirs_b[x][y], "%s/%s", dirs[x], dirs[tmp_size+y]);
        }
        tmp_y += tmp_size;
        z += tmp_size-1;
    }

    for (int i = 0; i < 10000; i++) {
        free(files[i]);
        free(dirs[i]);
    }

    free(files);
    free(dirs);
    return lines_nb+1;
}

int eachFileRating (char *** dirs, char ** transferts, unsigned int * dir_sizes, unsigned int nb_dirs) {
    printf("For each rating\n");
    int y = 0;

    char * commande = calloc(10000, sizeof(char));
    char * nom = calloc(15, sizeof(char));
    char * files = calloc(10000, sizeof(char));

    printf ("Allocating size : %d\n", nb_dirs);

    int * ratings = calloc(nb_dirs, sizeof(int));

    // Vider le fichier
    system("rm -f ../tmp/exif.txt;touch ../tmp/exif.txt");

    for (int y = 0; y < nb_dirs; y++) {
        for (int i = 0; i < dir_sizes[y]; i++) {
            // Commande
            sprintf(commande, "./exiv_xmp %s >> ../tmp/exif.txt", dirs[y][i]);
            printf ("%s\n", commande);
            system(commande);
        }
    }

    printf ("Lancement de la commande.\n");
    system(commande);
    printf ("Commande lancée.\n");

    // Traiter chaque image
    int i = 0, rating = -1, x = 0;

    FILE * RATING = fopen("data/images/rating.txt", "w");
    FILE * RATINGS = fopen("data/images/tmp/exif.txt", "r");

    char ** lignes = calloc(10000, sizeof(char*));

    for (int i = 0; i < 10000; i++) {
        lignes[i] = calloc(100, sizeof(char));
    }

    while (fgets(lignes[i++], 99, RATINGS));

    parseRatings(ratings, lignes, nb_dirs);

    for (y = 0; y < nb_dirs; y++) {
        for (i = 0; i < dir_sizes[y]; i++) {
            // Pour chaque rating recus

            if (ratings[i+y] == 5) {
                strcpy(transferts[x++], dirs[y][i]);
            }

            fprintf(RATING, "%s : %d\n", dirs[y][i], ratings[i+y]);
        }
    }

    fclose(RATING);
    fclose(RATINGS);

    for (int i = 0; i < 10000; i++) {
        free(lignes[i]);
    }

    free(lignes);
    free(commande);
    free(nom);
    free(ratings);
    free(files);

    return x;
}

int main (void) {
    getFiles();
    FILE * GETS = fopen("./data/images/gets.txt", "r");

    char ** liste_captures = calloc(MAX_CAPTURES, sizeof(char*));
    char ** transferts = calloc(MAX_CAPTURES, sizeof(char*));
    char ** nouvelles_captures = calloc(MAX_CAPTURES, sizeof(char*));
    char *** dirs = calloc(MAX_DIRS, sizeof(char**));
    unsigned int * dir_sizes = calloc(1000, sizeof(unsigned int));

    int i, j, number = 0;

    for (i = 0; i < MAX_CAPTURES; i++) {
        liste_captures[i] = calloc(TAILLE_NOM, sizeof(char));
        transferts[i] = calloc(TAILLE_NOM, sizeof(char));
        nouvelles_captures[i] = calloc(TAILLE_NOM, sizeof(char));
    }

    for (int s = 0; s < MAX_DIRS; s++) {
        dirs[i] = calloc(MAX_CAPTURES, sizeof(char*));
        for (int z = 0; z < MAX_CAPTURES; z++) {
            dirs[i][z] = calloc(TAILLE_NOM, sizeof(char));
        }
    }
    

    i = 0;

    while (fgets(liste_captures[i], TAILLE_NOM, GETS) != NULL) {
        // Remplissage
        enlever_last_car(liste_captures[i++]);
        number++;
    }

    i = 0;

    printf ("1\n");

    unsigned int files_nb = get_files_and_dirs(dirs, liste_captures, number, dir_sizes);
    int transferts_nb = eachFileRating(dirs, transferts, dir_sizes, files_nb);

    // transform_noms(transferts, nouvelles_captures, transferts_nb);

    transferer_noms(transferts);

    fclose(GETS);

    for (i = 0; i < MAX_CAPTURES; i++) {
        free(liste_captures[i]);
        free(transferts[i]);
        free(nouvelles_captures[i]);
    }

    for (int s = 0; s < MAX_DIRS; s++) {
        for (int z = 0; z < MAX_CAPTURES; z++) {
            free(dirs[i][z]);
        }
        free(dirs[i]);
    }

    free(liste_captures);
    free(transferts);
    free(nouvelles_captures);
    free(dirs);
    free(dir_sizes);

    return 0;
}