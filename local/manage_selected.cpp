#include "manage_selected2.h"

void clearBufLast (char * buf, unsigned int len, unsigned int nb) {
    int i;

    for (i = len; i >= len - nb; i--)
        buf[i] = 0;
    return;
}

// Envoi de la photo
void send_request (char *name) {
    printf("Requêtes\n");
    
    CURL *curl;
    CURLcode res;

    char * request_string = (char *) calloc(1000, sizeof(char));
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
    free(request_string);
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

// unsigned int load_dossiers (char ** dossiers, const char * path, unsigned int * refs) {
//     int k=0; //nb de fois ou on detecte les mots "le dossier"
//     int l=0;
//     int r=0;
//     char TAB1[TMaxL] = ""; // va contenir tt le file.txt
//     char tmp[100]={0};
//     int tab[TMax]={0}; //liée  a k contient les indice de ou les mots "le dossier" sont dans le TAB1
//     char code[TMax]=""; //contient les noms des dossiers separés par "+%+"
//     char code1[500]="";                    
//     char TAB2[]= "Le dossier";
//     char TAB3[]="aucun";
//     unsigned int * ks = (unsigned int*) calloc(10, sizeof(unsigned int));

//     FILE * File;
//     File = fopen(path, "r");
//     if (File == NULL) {
//         exit(0);
//     }
  
//     int rado = 0;
//     int longeur = 0;
//     // boucle for pour mettre dans TAB1 
//     for(int aze=0; aze<50000; aze++) {
//         fgets(tmp,2000,File);
//         while(tmp[rado] != 0) { 
//             TAB1[longeur] = tmp[rado];
//             longeur = longeur+1;
//             rado = rado + 1;
//         }

//         for(int zer=0; zer<100; zer++) {
//             tmp[zer] = 0;
//         }

//         rado=0;
//     }

//     // compare "le dossier" et TAB1[X] parcour tout TAB1

//     for(int c = 0;c < longeur; c++) {
//         if(TAB1[c]==TAB2[0]) {
//         if(TAB1[c+1]==TAB2[1]) {
//         if(TAB1[c+2]==TAB2[2]) {
//             if(TAB1[c+3]==TAB2[3])
//             {
//                 if(TAB1[c+4]==TAB2[4])
//                 {if(TAB1[c+5]==TAB2[5])
//                 {if(TAB1[c+6]==TAB2[6])
//                    {if(TAB1[c+7]==TAB2[7])

//                    {
//                     if(TAB1[c+8]==TAB2[8])
//                      {
//                         if(TAB1[c+9]==TAB2[9])    {  
                           
//                             tab[k]=c+3;                   
//                              k=k+1;
//     }}} }}}}}}}}

//     int g=0;
//     for(int rty=0;rty<k;rty++) {
//         g=tab[rty];
//         for (int h=0;h<60;h++) {
//             if(TAB1[g+h]==TAB3[0]) {
//                 if(TAB1[g+1+h]==TAB3[1]) {
//                     if(TAB1[g+h+2]==TAB3[2]) {
//                             if(TAB1[g+h+3]==TAB3[3]) {
//                                 tab[rty]=0; 
//                             }
//                         }
//                     }
//                 }

//             }
//         }
//     int q=0;
//     int t=0;
//     int qsd=0;
//     unsigned int x = 0;
//     unsigned int y = 0;
//     unsigned int xk = 0;

//     for(int s=0;s<k;s++) {
//         if(tab[s] != 0) {
//             ks[xk++] = s;
//             strcpy(code, "");
//             // printf("%d  ,,",tab[s]);
//             t = tab[s];
//             qsd=0;
//             for (int a=0;a<15;a++) {
//                 if(TAB1[t+a]=='/') {
//                     y = 0;
//                     while(TAB1[t+a+qsd+2]!=' ') { 
//                         code[q]=TAB1[t+a+qsd];
//                         printf ("%c", code[q]);
//                         dossiers[x][y++] = code[q];
//                         qsd++;  
//                         q=q+1;
//                     }
//                     printf ("1\n");
//                     x++;
//                 }
//             }

//             // code[q]='+';code[q+1]='%';code[q+2]='+';q=q+3;
//         }
//     }

//     t=0;
//     int klm=0;
//     int fdj=0;
//     for(int jkl=0;jkl<k;jkl++) {
//         if(tab[jkl]!=0) {
//             printf("%d\n",tab[jkl]);
//             klm=tab[jkl];
//             for (int hjk=0;hjk<100;hjk++) {	
//                 if(TAB1[klm+hjk]=='#') {
//                     while (TAB1[klm+hjk+fdj]!='I') {   
//                         code1[t]=TAB1[klm+hjk+fdj];
//                         fdj++;
//                         t++;
//                     }
//                 }
//                 fdj=0;
//             }
//         }
//     }

//     // for(int u=0;u<10000;u++) {
//     //     printf("%c",code1[u]);
//     // }


//     unsigned int u = 0;
//     char * number = (char*) calloc(5, sizeof(char));

//     x = 0;

//     while (code1[u] != 0) {
//         // printf ("%c", code1[u]);
//         strcpy(number, "");
//         if (code1[u-1] == '#') {
//             y = 0;
//             while (code1[u] != ' ') {
//                 number[y++] = code1[u];
//                 // printf ("%c", code1[u]);
//                 u++;
//             }

//             sscanf(number, "%d", &refs[x]);

//             // Calcul
//             refs[x] = refs[x] + ks[x];
//             x++;
//         }
        
//         u++;
//     }

//     free(number);
//     free(ks);
//     fclose(File);

//     return x;
// }

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
    unsigned int file_s = strlen(file)-1;

    for (i = 0; i < lines; i++) {
        printf("%s | %s\n", historique[i], file);
        if (strncmp(historique[i], file, file_s) == 0) {
           return 1;
        }
    }

    return 0;
}

void mirroir (char * buf, unsigned int n) {
    int i = 0;
    char car;

    for (i = 0; i < (n+1)/2; i++) {
        car = buf[n-i-1];
        buf[n-i-1] = buf[i];
        buf[i] = car;
    }
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

// Pour l'envoi
void transferer_noms (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera) {
    // int i = 0;

    // char * commande = (char*) calloc(250, sizeof(char));
    // int file_transfered = 0;

    // FILE * HISTORIQUE = fopen("data/images/historique.txt", "a");
    // FILE * HISTORIQUER = fopen("data/images/historique.txt", "r");

    // system("cd data/images/gets");

    // char ** hist_lines = (char **) calloc(MAX_CAPTURES, sizeof(char*));

    // int x;

    // for (x = 0; x < MAX_CAPTURES; x++) {
    //     hist_lines[x] = (char*) calloc(TAILLE_NOM, sizeof(char));
    // }

    // x = 0;

    // while (fgets(hist_lines[x++], TAILLE_NOM, HISTORIQUER));

    // char * current_file = (char*) calloc(TAILLE_NOM, sizeof(char));
    // char * dossier = (char*) calloc(TAILLE_NOM, sizeof(char));
    // int status = 0;
    // CameraFile * file;
    // gp_file_new(&file);

    // char * filename = (char*) calloc(100, sizeof(char));

    // for (i = 0; i < n_transferts; i++) {
    //     strcpy(dossier, "");
    //     filename = getName(liste[i], dossier);
    //     // Il y aura peut être besoin d'insérer les lignes précédentes dans cette boucle

    //     // A corriger
    //     file_transfered = compare_file_historique(filename, hist_lines, x);
    //     if (file_transfered == 0) {
    //         sprintf(commande, "data/images/gets/%s", filename);

    //         printf ("%s\n", commande);
    //         status = gp_camera_file_get(camera, dossier, filename, GP_FILE_TYPE_NORMAL, file, context);
    //         handleError(status);
    //         printf("%s\n", dossier);
    //         status = gp_file_save(file, (const char*) commande);
    //         sprintf(commande, "mv data/images/gets/%s /home/thomas/camera_server/public", filename);
    //         // Envoyer le nom du nouveau fichier transféré au socket

    //         // ÉCIRE DANS L'HISTORIQUE DES TRANSFERTS
    //         printf("%s\n", commande);
    //         printf ("Transfert !\n");
    //         // system(commande);
    //         fprintf(HISTORIQUE, "%s\n", filename);
    //         // send_request(filename);
    //     }
    // }

    // for (x = 0; x < MAX_CAPTURES; x++) {
    //     free(hist_lines[x]);
    // }

    // free(current_file);
    // fclose(HISTORIQUE);
    // fclose(HISTORIQUER);
    // free(hist_lines);
    // free(filename);
    // free(commande);
    // free(dossier);
    // gp_file_free(file);
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

void parseRatings (int * ratings, char ** lines, unsigned int size) {
    // A coder
    // Lecture en imparité d'indexs

    int i = 0, x = 0;

    while (i < size) { // OU x < size
        ratings[x] = parseRating(lines[i]);        
        i++;
    }
}

// unsigned int read_dir_list (char ** dossiers, unsigned int * refs) {
//     unsigned int dirs_nb = load_dossiers(dossiers, "data/images/gets.txt", refs);

//     for (unsigned int i = 0; i < dirs_nb; i++) {
//         clearBufLast(dossiers[i], strlen(dossiers[i])-1, 1);
//     }

//     return dirs_nb; // Nombre de dossiers contenant des fichiers
// }

// unsigned int read_file_list (char ** files, char ** lines, unsigned int nb, unsigned int starts) {
//     unsigned int line_size, x = 0, i, y;

//     // Parser la liste de fichiers
//     for (i = starts; i < nb; i++) {
//         if (lines[i][0] == '#') {
//             for (y = 7; y <= 18; y++) {
//                 files[x][y-7] = lines[i][y];
//             }
//             x++;
//         }

//         else if (x != 0) break;
//     }

//     return x;
// }

void handleError(int status) {
    printf ("%s\n", gp_result_as_string(status));
}

static void
ctx_error_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "\n*** Contexterror ***              \n%s\n",str);
        fflush   (stderr);
}

static void
ctx_status_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "%s\n", str);
        fflush   (stderr);
}

GPContext* sample_create_context() {
	GPContext *context;

	/* This is the mandatory part */
	context = gp_context_new();

	/* All the parts below are optional! */
        gp_context_set_error_func (context, ctx_error_func, NULL);
        gp_context_set_status_func (context, ctx_status_func, NULL);

	/* also:
	gp_context_set_cancel_func    (p->context, ctx_cancel_func,  p);
        gp_context_set_message_func   (p->context, ctx_message_func, p);
        if (isatty (STDOUT_FILENO))
                gp_context_set_progress_funcs (p->context,
                        ctx_progress_start_func, ctx_progress_update_func,
                        ctx_progress_stop_func, p);
	 */
	return context;
}

// Retourne le status, le reste, dans les pointeurs
int getPlacements(int * rating, char * dir, char * file, char * data, GPContext * context, Camera * camera)
{
    int status = 0;
    
    uint64_t size_l = 32000;
      status = gp_camera_file_read(camera,
        dir,
        file,
        GP_FILE_TYPE_NORMAL,
        0,
        data,
        &size_l,
        context
      );

      try {

    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open((const Exiv2::byte*) data, size_l);
    // std::cout << image;
    // assert (image.get() != 0);
    image->readMetadata();
 
    Exiv2::XmpData &xmpData = image->xmpData();
    if (xmpData.empty()) {
        std::string error(dir);
        error += ": No XMP data found in the file";
        throw Exiv2::Error(Exiv2::kerErrorMessage, error);
    }
    if (xmpData.empty()) 
      {
        return -1;
        // std::string error(argv[1]);
        // error += ": No XMP properties found in the XMP packet";
        // throw Exiv2::Error(Exiv2::kerErrorMessage, error);
      }
 
        std::string stars = "";
        Exiv2::XmpData::const_iterator md = xmpData.begin();
        stars = md->toString();
        *rating = std::stoi(stars);
    }
 
    
catch (Exiv2::AnyError& e) 
  {
    std::cout << "Caught Exiv2 exception '" << e << "'\n";
    return -1;
  }

    return 0;

    // std::cout << data;

    // const std::vector<uint8_t> bytes = std::vector<uint8_t> vec(str.begin(), str.end());

    // std::vector<uint8_t> bytes = static_assert(std::is_same<uint8_t, char>::value, "uint8_t is not unsigned char");
    

  // Exiv2::XmpParser::initialize();
  // ::atexit(Exiv2::XmpParser::terminate);

   *rating = 0;


}

// unsigned int get_files_and_dirs (char *** dirs_b, char ** dirs_n, char ** lines, unsigned int nb, unsigned int * sizes_list) {
//     // Executer read_dir_list, read_file_list puis blockerize ...
//     unsigned int * ref_lines = (unsigned int*) calloc(MIN_DIRS, sizeof(unsigned int));
//     char ** files = (char**) calloc(1000, sizeof(char*));
//     unsigned int x, y, z;

//     printf ("Parsing ...\n");

//     for (int i = 0; i < 1000; i++) {
//         files[i] = (char*) calloc(100, sizeof(char));
//     }

//     unsigned int lines_nb = read_dir_list(dirs_n, ref_lines);
//     unsigned int tmp_size, tmp_y = 0;

//     printf("%d", lines_nb);

//     for (x = 0; x < lines_nb; x++) {
//         for (int i = 0; i < 1000; i++) {
//             for (int j = 0; j < 100; j++) {
//                 files[i][j] = 0;
//             }
//         }

//         tmp_size = read_file_list(files, lines, nb, ref_lines[x]); // ref_line est la lignes où commencer

//         for (y = 0; y < tmp_size; y++) {
//             strcpy(dirs_b[x][y], files[y]);
//         }

//         // tmp_y += tmp_size;
//         // z += tmp_size-1;

//         x++;
//     }

//     for (int i = 0; i < 1000; i++) {
//         free(files[i]);
//     }

//     free(files);

//     printf ("1");

//     return 1;
// }

int get_files_and_dirs (char *** dirs_b, char ** dirs_n, Camera * camera, GPContext * context) {
    CameraList * folderList;
    char * dir;
    char * file;
    char * folder = (char*) calloc(100, sizeof(char));
    int status = gp_list_new(&folderList);
    status = gp_camera_folder_list_folders(camera,
		"/",
		folderList,
		context 
	);

    status = gp_list_get_name(folderList, 0, (const char**) &dir);

    // status = gp_list_reset(folderList);

    char * tmp = (char*) calloc(100, sizeof(char));
    char * tmp_dir = (char*) calloc(100, sizeof(char));

    CameraList * fileList;
    gp_list_new(&fileList);

    unsigned int n = strlen(dir)-1;
    strcpy(tmp_dir, "/store_");

    unsigned int x = 0, z = 0;

    while (dir[z++] != '_');
    while (dir[z] != 0) {
        tmp_dir[7+x++] = dir[z++];
    }

    sprintf(folder, "%s/DCIM", tmp_dir);
    printf ("%s\n", folder);

    status = gp_camera_folder_list_folders(camera,
		folder,
		folderList,
		context 
	);

    int nb = gp_list_count(folderList);
    int nb_files = 0;

    for (unsigned int i = 0; i < nb; i++) {
        // status = gp_list_reset(fileList);

        strcpy(dir, "");
        status = gp_list_get_name(folderList, i, (const char**) &dir);

        sprintf (tmp_dir, "%s/%s", folder, dir);
        printf ("%s\n", tmp_dir);

        strcpy(dirs_n[i], tmp_dir);

        status = gp_camera_folder_list_files(camera,
		    tmp_dir,
		    fileList,
		    context 
	    );

        nb_files = gp_list_count(fileList);

        if (nb_files != 0) {
            for (unsigned int j = 0; j < nb_files; j++) {
                strcpy(file, "");
                status = gp_list_get_name(fileList, j, (const char**) &file);
                handleError(status);
                strcpy(dirs_b[i][j], file);
                printf ("%s\n", dirs_b[i][j]);
            }
        }

        // sprintf(dirs_n[i], "/%s", dir);
    }

    free(tmp_dir);
    free(folder);
    free(tmp);
    gp_list_free(fileList);
    gp_list_free(folderList);

    return nb; // Nombre de dossiers
}

int eachFileRating (char *** dossiers, char ** dirs, char ** transferts, unsigned int * dir_sizes, unsigned int nb_dirs, Camera * camera, GPContext * context) {
    printf("For each rating\n");
    int y = 0;
    int rates = 0;

    char * commande = (char*) calloc(300, sizeof(char));
    char * nom = (char*) calloc(15, sizeof(char));
    char * files = (char*) calloc(10000, sizeof(char));
    char * data = (char*) calloc(150000, sizeof(char));

    printf ("Allocating size : %d\n", nb_dirs);
    FILE * RATING = fopen("data/images/rating.txt", "w");

    unsigned int i, x = 0;

    for (int y = 0; y < nb_dirs; y++) {
        i = 0;
        while (dossiers[y][i][0] != 0) {
            // Commande
            printf("%s\n", dossiers[y][i]);
            int timeout = 0;
            getPlacements(&rates, dirs[y], dossiers[y][i], data, context, camera);
            if (rates == 5) {
                sprintf(transferts[x++], "%s/%s", dirs[y], dossiers[y][i]);
            }

            // fprintf(RATING, "%s : %d\n", dossiers[y][i], rates);
            i++;
        }
    }

    printf ("Lancement de la commande.\n");
    printf ("Commande lancée.\n");

    // Traiter chaque image
    // i = 0;
    // unsigned int rating = 0;

    // for (y = 0; y < nb_dirs; y++) {
    //     for (i = 0; i < dir_sizes[y]; i++) {
    //         // Pour chaque rating reçu

    //         if (ratings[i+y] == 5) {
    //             strcpy(transferts[x++], dossiers[y][i]);
    //         }

    //         fprintf(RATING, "%s : %s\n", dirs[i], dossiers[y][i]);
    //     }
    // }

    fclose(RATING);
    free(commande);
    free(nom);
    free(files);
    free(data);

    return x;
}
