#include "manage_selected2.h"

void clearBufLast (char * buf, unsigned int len, unsigned int nb) {
    int i;

    for (i = len; i >= len - nb; i--)
        buf[i] = 0;
    return;
}

int getCameraModel (Camera * cam) {
    CameraAbilities cam_abilities;
    FILE * MODEL = fopen("data/tmp/model.txt", "w");
    char * camera_model = (char*) calloc(50, sizeof(char));
    int status = gp_camera_get_abilities(cam, &cam_abilities);

    if (status < 0) {
        free(camera_model);
        fclose(MODEL);
        return generateError(status);
    }

    strcpy(camera_model, cam_abilities.model);

    fprintf (MODEL, "%s", camera_model);
    printf ("modèle : %s\n", camera_model);

    free(camera_model);
    fclose(MODEL);
    return 0;
}

void send_status_request (int status) {
    CURL *curl;
    CURLcode res;

    char * request_string = (char*) calloc(1000, sizeof(char));
    sprintf(request_string, "status=%d", status);
    
    curl_global_init(CURL_GLOBAL_ALL);
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/transfert/status");
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

int generateError (int status) {
    // Notifier une erreur en fonction de ce qu'il s'agit.

    // J'utilise un système de fichiers et un second script de lecture de fichiers et de lancement de requêtes CURL
    // Écire le status dans un fichier
    FILE * ERR = fopen("data/tmp/errors.txt", "r");
    FILE * NOTIFICATION;
    int previousStatus = 0;

    NOTIFICATION = fopen("data/tmp/errors.txt", "w");
    
    // Envoyer l'ordre de notifier vers un autre script qui détecte le changement de status
    
    fprintf(NOTIFICATION, "%d\n", status);
    fclose(NOTIFICATION);

    printf("GENERATE ERROR %d\n", status);
    fclose(ERR);

    return status;
}

// Envoi de la photo
void send_request (char *name) {
    printf("Requêtes\n");
    
    CURL *curl;
    CURLcode res;

    char * request_string = (char*) calloc(1000, sizeof(char));
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

unsigned int dossiers_to_list (char *** dossiers, char ** list, char ** dirs, unsigned int nb_dossiers, unsigned int * nb_files) {
    unsigned int x = 0, item = 0;

    while (x < nb_dossiers) {
        for (unsigned int y = 0; y < nb_files[x]; y++) {
            sprintf(list[item++], "%s/%s", dirs[x], dossiers[x][y]);
        }
        
        x++;
    }

    return item;
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

int sauvegarder_noms (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera) {
    int i = 0, file_transfered = 0, x = 0, nb_historique;
    char * commande = (char*) calloc(300, sizeof(char));
    char ** hist_lines = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char * current_file = (char*) calloc(TAILLE_NOM, sizeof(char));
    char * dossier = (char*) calloc(TAILLE_NOM, sizeof(char));
    char * filename = (char*) calloc(100, sizeof(char));

    for (int u = 0; u < MAX_CAPTURES; u++) {
        hist_lines[u] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    FILE * HISTORIQUE = fopen("data/images/historique.txt", "a");
    FILE * HISTORIQUER = fopen("data/images/historique.txt", "r");

    while (fgets(hist_lines[x++], TAILLE_NOM, HISTORIQUER));

    nb_historique = x;
    x = 0;

    for (i = 0; i < nb_historique; i++) {
        strcpy(dossier, "");
        filename = getName(liste[i], dossier);

        // Suite ici
    }

    for (int u = 0; u < MAX_CAPTURES; u++) {
        free(hist_lines[u]);
    }

    fclose(HISTORIQUE);
    fclose(HISTORIQUER);
    free(hist_lines);
    free(commande);
}

// Pour l'envoi
int transferer_noms (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera, int online) {
    int i = 0;

    char * commande = (char*) calloc(250, sizeof(char));
    int file_transfered = 0;

    FILE * HISTORIQUE = fopen("data/images/historique.txt", "a");
    FILE * HISTORIQUER = fopen("data/images/historique.txt", "r");

    system("cd data/images/gets");

    char ** hist_lines = (char**) calloc(MAX_CAPTURES, sizeof(char*));

    int x;

    for (x = 0; x < MAX_CAPTURES; x++) {
        hist_lines[x] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    x = 0;

    while (fgets(hist_lines[x++], TAILLE_NOM, HISTORIQUER));

    char * current_file = (char*) calloc(TAILLE_NOM, sizeof(char));
    char * dossier = (char*) calloc(TAILLE_NOM, sizeof(char));
    int status = 0;
    CameraFile * file;
    gp_file_new(&file);
    char * filename = (char*) calloc(100, sizeof(char));

    for (i = 0; i < n_transferts; i++) {
        strcpy(dossier, "");
        filename = getName(liste[i], dossier);
        // Il y aura peut être besoin d'insérer les lignes précédentes dans cette boucle

        // A corriger
        file_transfered = compare_file_historique(filename, hist_lines, x);
        if (file_transfered == 0) {
            if (online) {
                sprintf(commande, "data/images/gets/%s", filename);
            }

            else {
                sprintf(commande, "data/images/cloud/%s", filename);
            }

            printf ("%s\n", commande);
            status = gp_camera_file_get(camera, dossier, filename, GP_FILE_TYPE_NORMAL, file, context);
            handleError(status);
            printf("%s\n", dossier);

            if (status < 0) return generateError(status);

            status = gp_file_save(file, (const char*) commande);

            if (status < 0) return generateError(status);

            if (online) {
                // sprintf(commande, "mv data/images/gets/%s /home/thomas/camera_server/public", filename);
                // system(commande);
                // send_request(filename);
            }

            else {
                // sprintf(commande, "mv data/images/gets/%s data/images/cloud", filename);
                // system(commande);

                // Indiquer que des fichiers ont été transférés hors ligne.

                // Ou détecter dans un autre script que des fichiers existent dans le mode hors ligne.
            }

            // Envoyer le nom du nouveau fichier transféré au socket

            // ÉCIRE DANS L'HISTORIQUE DES TRANSFERTS
            printf("%s\n", commande);
            printf ("Transfert !\n");
            fprintf(HISTORIQUE, "%s\n", filename);
        }
    }

    for (x = 0; x < MAX_CAPTURES; x++) {
        free(hist_lines[x]);
    }

    free(current_file);
    fclose(HISTORIQUE);
    fclose(HISTORIQUER);
    free(hist_lines);
    free(filename);
    free(commande);
    free(dossier);
    gp_file_free(file);

    return 0;
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

void parseRatings (int * ratings, char ** lines, unsigned int size) {
    // A coder
    // Lecture en imparité d'indexs

    int i = 0, x = 0;

    while (i < size) { // OU x < size
        ratings[x] = parseRating(lines[i]);        
        i++;
    }
}

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
    
    uint64_t size_l = 64000;
      status = gp_camera_file_read(camera,
        dir,
        file,
        GP_FILE_TYPE_NORMAL,
        0,
        data,
        &size_l,
        context
      );

      if (status < 0) return status;

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
        return -1; // Il faudrait trouver un code d'état spécial
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


    // std::cout << data;

    // const std::vector<uint8_t> bytes = std::vector<uint8_t> vec(str.begin(), str.end());

    // std::vector<uint8_t> bytes = static_assert(std::is_same<uint8_t, char>::value, "uint8_t is not unsigned char");
    

  // Exiv2::XmpParser::initialize();
  // ::atexit(Exiv2::XmpParser::terminate);

   return 0;


}

int get_files_and_dirs (char *** dirs_b, char ** dirs_n, unsigned int * nb, unsigned int * dir_sizes, Camera * camera, GPContext * context) {
    CameraList * folderList;
    char * folder = (char*) calloc(100, sizeof(char));
    gp_list_new(&folderList);
    const char * dir;
    int status = gp_camera_folder_list_folders(camera, "/", folderList, context);
    if (status < 0) return generateError(status);

    status = gp_list_get_name(folderList, 0, (const char**) &dir);
    if (status < 0) return generateError(status);

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
    if (status < 0) return generateError(status);

    int local_nb = gp_list_count(folderList);
    int nb_files = 0;

    if (local_nb < 0) return generateError(local_nb);
    *nb = local_nb;

    for (unsigned int i = 0; i < *nb; i++) {
        // status = gp_list_reset(fileList);

        const char * subdir;
        status = gp_list_get_name(folderList, i, (const char**) &subdir);

        sprintf (tmp_dir, "%s/%s", folder, subdir);
        printf ("%s\n", tmp_dir);

        strcpy(dirs_n[i], tmp_dir);

        status = gp_list_reset(fileList);
        if (status < 0) return generateError(status);

        status = gp_camera_folder_list_files(camera, tmp_dir, fileList, context);
        if (status < 0) return generateError(status);

        nb_files = gp_list_count(fileList);
        if (nb_files < 0) return generateError(nb_files);

        dir_sizes[i] = nb_files;

        for (unsigned int j = 0; j < nb_files; j++) {
            const char * file;
            status = gp_list_get_name(fileList, j, (const char**) &file);
            handleError(status);
            if (status < 0) return generateError(status);
            strcpy(dirs_b[i][j], file);
            printf ("%s\n", dirs_b[i][j]);
        }

        // sprintf(dirs_n[i], "/%s", dir);
    }

    free(tmp_dir);
    free(folder);
    free(tmp);
    gp_list_free(folderList);
    gp_list_free(fileList);
    return 0; // Pas d'erreur
}

void afficher_i (unsigned int * buffer, unsigned int n) {
    unsigned int i = 0;

    for (i = 0; i < n; i++) {
        printf ("%u\n", buffer[i]);
    }
}

int eachFileRating (char *** dossiers, char ** dirs, char ** transferts, unsigned int * dir_sizes, unsigned int nb_dirs, unsigned int * transferts_nb, Camera * camera, GPContext * context) {
    printf("For each rating\n");
    int y = 0;
    int rates = 0;

    char * commande = (char*) calloc(300, sizeof(char));
    char * nom = (char*) calloc(15, sizeof(char));
    char * files = (char*) calloc(10000, sizeof(char));
    char * data = (char*) calloc(150000, sizeof(char));

    printf ("Allocating size : %d\n", nb_dirs);
    FILE * RATING = fopen("data/images/rating.txt", "w");

    afficher_i(dir_sizes, nb_dirs);

    int i, x = 0;

    for (int y = nb_dirs-1; y >= 0; y--) {
        printf("dir_size : %u\n", dir_sizes[y]);
        // indice du dernier fichier de dossier
        if (dir_sizes[y] > 0) {
            i = dir_sizes[y] - 1;
        }

        else {
            i = -1;
        }

        while (i >= 0) {
            // Commande
            printf("%s\n", dossiers[y][i]);
            int status = getPlacements(&rates, dirs[y], dossiers[y][i], data, context, camera);

            if (status < 0) return generateError(status);

            if (rates == 5) {
                sprintf(transferts[x++], "%s/%s", dirs[y], dossiers[y][i]);
            }

            fprintf(RATING, "%s : %d\n", dossiers[y][i], rates);

            i--;
        }
    }

    printf ("Lancement de la commande.\n");
    printf ("Commande lancée.\n");

    fclose(RATING);
    free(commande);
    free(nom);
    free(files);
    free(data);

    *transferts_nb = x;

    return 0;
}

int eachFileRating_1 (char ** files, char ** transferts, unsigned int files_nb, unsigned int * transferts_nb, Camera * camera, GPContext * context) {
    printf("For each rating\n");
    int y = 0;
    int rates = 0;

    char * nom = (char*) calloc(100, sizeof(char));
    char * data = (char*) calloc(150000, sizeof(char));
    char * dirname = (char*) calloc(100, sizeof(char));

    printf ("Allocating size : %d\n", files_nb);
    FILE * RATING = fopen("data/images/rating.txt", "w");

    int i, x = 0;

    for (int y = 0; y < PART_NB; y++) {
        printf("%d\n", y);
        // Commande
        printf("%s\n", files[y]);
        nom = getName(files[y], dirname);
        int status = getPlacements(&rates, dirname, nom, data, context, camera);

        if (status < 0) return generateError(status);

        if (rates == 5) {
            sprintf(transferts[x++], "%s/%s", dirname, nom);
        }

        fprintf(RATING, "%s : %d\n", files[y], rates);
    }

    fclose(RATING);
    free(nom);
    free(data);
    free(dirname);

    *transferts_nb = x;

    return 0;
}

void cut_list(char ** files, unsigned int size, char ** newList) {
    unsigned int x = 0, y;
    
    if (size <= 50 && size > 0) {
        for (y = size-1; y >= 0; y--) {
            strcpy(newList[x++], files[y]);
        }
    }

    else {
        for (y = size-1; y >= size-PART_NB; y--) {
            strcpy(newList[x++], files[y]);
        }
    }
}
