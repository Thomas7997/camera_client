#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <time.h>
#define MAX 80

#define MAX_CAPTURES 100000
#define TAILLE_NOM 100
#define TMaxL 1500000
#define TMax 100
#define MIN_DIRS 10

struct _Dossier {
    char * title;
    char ** images;
    unsigned int nb_images;
    unsigned int ref_line;
};

typedef struct _Dossier Dossier;

void clearBufLast (char * buf, unsigned int len, unsigned int nb) {
    int i;

    for (i = len; i >= len - nb; i--)
        buf[i] = 0;
    return;
}

void add_dossier (Dossier * dossiers, char ** images, unsigned int index) {
    unsigned int x = 0;

    dossiers[index].images = images;
}

// Envoi de la photo
void send_request (char *name) {
    printf("Requêtes\n");
    
    CURL *curl;
    CURLcode res;

    char * request_string = calloc(1000, sizeof(char));
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


unsigned int load_dossiers (char ** dossiers, const char * path, unsigned int * refs) {
    int k=0; //nb de fois ou on detecte les mots "le dossier"
    int l=0;
    int r=0;
    char TAB1[TMaxL] = ""; // va contenir tt le file.txt
    char tmp[100]={0};
    int tab[TMax]={0}; //liée  a k contient les indice de ou les mots "le dossier" sont dans le TAB1
    char code[TMax]=""; //contient les noms des dossiers separés par "+%+"
    char code1[500]="";                    
    char TAB2[10]= "Le dossier";
    char TAB3[5]="aucun";
    unsigned int * ks = calloc(10, sizeof(unsigned int));

    FILE * File;
    File = fopen(path, "r");
    if (File == NULL) {
        exit(0);
    }
  
    int rado = 0;
    int longeur = 0;
    // boucle for pour mettre dans TAB1 
    for(int aze=0; aze<50000; aze++) {
        fgets(tmp,2000,File);
        while(tmp[rado] != 0) { 
            TAB1[longeur] = tmp[rado];
            longeur = longeur+1;
            rado = rado + 1;
        }

        for(int zer=0; zer<100; zer++) {
            tmp[zer] = 0;
        }

        rado=0;
    }

    // compare "le dossier" et TAB1[X] parcour tout TAB1

    for(int c = 0;c < longeur; c++) {
        if(TAB1[c]==TAB2[0]) {
        if(TAB1[c+1]==TAB2[1]) {
        if(TAB1[c+2]==TAB2[2]) {
            if(TAB1[c+3]==TAB2[3])
            {
                if(TAB1[c+4]==TAB2[4])
                {if(TAB1[c+5]==TAB2[5])
                {if(TAB1[c+6]==TAB2[6])
                   {if(TAB1[c+7]==TAB2[7])

                   {
                    if(TAB1[c+8]==TAB2[8])
                     {
                        if(TAB1[c+9]==TAB2[9])    {  
                           
                            tab[k]=c+3;                   
                             k=k+1;
    }}} }}}}}}}}

    int g=0;
    for(int rty=0;rty<k;rty++) {
        g=tab[rty];
        for (int h=0;h<60;h++) {
            if(TAB1[g+h]==TAB3[0]) {
                if(TAB1[g+1+h]==TAB3[1]) {
                    if(TAB1[g+h+2]==TAB3[2]) {
                            if(TAB1[g+h+3]==TAB3[3]) {
                                tab[rty]=0; 
                            }
                        }
                    }
                }

            }
        }
    int q=0;
    int t=0;
    int qsd=0;
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int xk = 0;

    for(int s=0;s<k;s++) {
        if(tab[s] != 0) {
            ks[xk++] = s;
            strcpy(code, "");
            // printf("%d  ,,",tab[s]);
            t = tab[s];
            qsd=0;
            for (int a=0;a<15;a++) {
                if(TAB1[t+a]=='/') {
                    y = 0;
                    while(TAB1[t+a+qsd+2]!=' ') { 
                        code[q]=TAB1[t+a+qsd];
                        printf ("%c", code[q]);
                        dossiers[x][y++] = code[q];
                        qsd++;  
                        q=q+1;
                    }
                    printf ("1\n");
                    x++;
                }
            }

            // code[q]='+';code[q+1]='%';code[q+2]='+';q=q+3;
        }
    }

    t=0;
    int klm=0;
    int fdj=0;
    for(int jkl=0;jkl<k;jkl++) {
        if(tab[jkl]!=0) {
            printf("%d\n",tab[jkl]);
            klm=tab[jkl];
            for (int hjk=0;hjk<100;hjk++) {	
                if(TAB1[klm+hjk]=='#') {
                    while (TAB1[klm+hjk+fdj]!='I') {   
                        code1[t]=TAB1[klm+hjk+fdj];
                        fdj++;
                        t++;
                    }
                }
                fdj=0;
            }
        }
    }

    // for(int u=0;u<10000;u++) {
    //     printf("%c",code1[u]);
    // }


    unsigned int u = 0;
    char * number = calloc(5, sizeof(char));

    x = 0;

    printf ("1\n");

    while (code1[u] != 0) {
        // printf ("%c", code1[u]);
        strcpy(number, "");
        if (code1[u-1] == '#') {
            y = 0;
            while (code1[u] != ' ') {
                number[y++] = code1[u];
                // printf ("%c", code1[u]);
                u++;
            }

            sscanf(number, "%d", &refs[x]);

            // Calcul
            refs[x] = refs[x] + ks[x];
            x++;
        }
        
        u++;
    }

    printf("1\n");

    free(number);
    free(ks);
    fclose(File);

    printf("%d\n", x);

    return x;
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

    return line[0] - '0';
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

    int i = 0, x = 0;

    while (i < size) { // OU x < size
        ratings[x] = parseRating(lines[i]);        
        i++;
    }
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

unsigned int read_dir_list (Dossier * dossiers) {
    char ** dirs = calloc(MIN_DIRS, sizeof(char*));
    unsigned int * ref_lines = calloc(MIN_DIRS, sizeof(unsigned int));

    for (int i = 0; i < MIN_DIRS; i++) {
        dirs[i] = calloc(100, sizeof(char));
    }

    unsigned int dirs_nb = load_dossiers(dirs, "files.txt", ref_lines);
    
    for (int i = 0; i < dirs_nb; i++) {
        dossiers[i].title = dirs[i];
        dossiers[i].ref_line = ref_lines[i];
        printf("%s, %d\n", dossiers[i].title, dossiers[i].ref_line);
    }

    for (int i = 0; i < MIN_DIRS; i++) {
        free(dirs[i]);
    }

    free(dirs);
    free(ref_lines);

    return dirs_nb; // Nombre de dossiers contenant des fichiers
}

unsigned int read_file_list (char ** files, char ** lines, unsigned int nb, unsigned int starts) {
    unsigned int line_size, x = 0, i, y;

    // Parser la liste de fichiers
    for (i = starts; i < nb; i++) {
        if (lines[i][0] == '#') {
            for (y = 7; y <= 18; y++) {
                files[x][y-7] = lines[i][y];
            }
            x++;
        }

        else if (x != 0) break;
    }

    return x;
}

unsigned int get_files_and_dirs (Dossier * dirs_b, char ** lines, unsigned int nb, unsigned int * sizes_list) {
    // Executer read_dir_list, read_file_list puis blockerize ...
    unsigned int ref_line;
    char ** files = calloc(1000, sizeof(char*));
    unsigned int x, y, z;

    printf ("Parsing ...\n");

    for (int i = 0; i < 1000; i++) {
        files[i] = calloc(100, sizeof(char));
    }

    unsigned int lines_nb = read_dir_list(dirs_b);
    unsigned int tmp_size, tmp_y = 0;

    printf("%d", lines_nb);

    for (x = 0; x < lines_nb; x++) {
        for (int i = 0; i < 1000; i++) {
            for (int j = 0; j < 100; j++) {
                files[i][j] = 0;
            }
        }


        dirs_b[x].nb_images = read_file_list(files, lines, nb, dirs_b[x].ref_line); // ref_line est la lignes où commencer

        add_dossier(dirs_b, files, x);

        for (int z = 0; z < dirs_b[x].nb_images; z++) {
            printf ("%s\n", dirs_b[x].images[z]);
        }
        // tmp_y += tmp_size;
        // z += tmp_size-1;

        x++;
    }

    for (int i = 0; i < 1000; i++) {
        free(files[i]);
    }

    free(files);

    return lines_nb;
}

int eachFileRating (Dossier * dossiers, char ** transferts, unsigned int * dir_sizes, unsigned int nb_dirs) {
    printf("For each rating\n");
    int y = 0;

    char * commande = calloc(300, sizeof(char));
    char * nom = calloc(15, sizeof(char));
    char * files = calloc(10000, sizeof(char));

    printf ("Allocating size : %d\n", nb_dirs);

    int * ratings = calloc(nb_dirs, sizeof(int));

    // Vider le fichier
    system("echo "" > ./data/tmp/exif.txt");

    unsigned int i = 0;

    for (int y = 0; y < nb_dirs; y++) {
        for (int i = 0; i < dossiers[y].nb_images; i++) {
            // Commande
            printf("%s %s\n", dossiers[y].title, dossiers[y].images[i]);
            sprintf(commande, "./exiv_xmp %s %s >> ./data/tmp/exif.txt", dossiers[y].title, dossiers[y].images[i]);
            printf ("%s\n", commande);
            system(commande);
        }
    }

    printf ("Lancement de la commande.\n");
    system(commande);
    printf ("Commande lancée.\n");

    // Traiter chaque image
    i = 0;
    unsigned int rating = -1, x = 0;

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
                strcpy(transferts[x++], dossiers[y].images[i]);
            }

            fprintf(RATING, "%s : %d\n", dossiers[y].images[i], ratings[i+y]);
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

void init_dossiers (Dossier * dirs) {
    dirs = calloc(MIN_DIRS, sizeof(Dossier));

    dirs->title = calloc(100, sizeof(char));
    dirs->images = calloc(1000, sizeof(char*));
    for (int i = 0; i < 1000; i++) {
        dirs->images[i] = calloc(100, sizeof(char));
    }
}

int main (void) {
    getFiles();
    FILE * GETS = fopen("data/images/gets.txt", "r");
    Dossier * dossiers;
    init_dossiers(dossiers);

    char ** liste_captures = calloc(MAX_CAPTURES, sizeof(char*));
    char ** transferts = calloc(MAX_CAPTURES, sizeof(char*));
    unsigned int * dir_sizes = calloc(1000, sizeof(unsigned int));

    int i, j, number = 0;

    for (i = 0; i < MAX_CAPTURES; i++) {
        liste_captures[i] = calloc(TAILLE_NOM, sizeof(char));
        transferts[i] = calloc(TAILLE_NOM, sizeof(char));
    }

    i = 0;

    while (fgets(liste_captures[i], TAILLE_NOM, GETS) != NULL) {
        // Remplissage
        enlever_last_car(liste_captures[i++]);
        number++;
    }

    i = 0;

    unsigned int files_nb = get_files_and_dirs(dossiers, liste_captures, number, dir_sizes);
    int transferts_nb = eachFileRating(dossiers, transferts, dir_sizes, files_nb);

    transferer_noms(transferts);

    fclose(GETS);

    for (i = 0; i < MAX_CAPTURES; i++) {
        free(liste_captures[i]);
        free(transferts[i]);
    }

    free(dossiers);
    free(liste_captures);
    free(transferts);
    free(dir_sizes);
    free(dossiers);

    return 0;
}