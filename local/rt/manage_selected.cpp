#include "manage_selected.h"

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
        error += ": No XMP data found in the file\n";
        std::cout << error;
        //throw Exiv2::Error(Exiv2::kerErrorMessage, error);
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

    int i, x = 0;

    for (int y = 0; y < PART_NB; y++) {
        // Commande
        nom = getName(files[y], dirname);
        printf ("%s\n", nom);
        int status = getPlacements(&rates, dirname, nom, data, context, camera);

        if (status < 0) return generateError(status);

        if (rates == 5) {
            sprintf(transferts[x], "%s/%s", dirname, nom);
            printf("%s\n", transferts[x++]);
        } 

        // printf ("ÉCRITURE DU PLACEMENT.\n");

        // fprintf(RATING, "%s : %d\n", files[y], rates);

        // printf ("Placement écrit.\n");
    }

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

void findPathIndex(char ** files, char * name, char * path) {
    unsigned int x = 0;
    char * dir = (char*) calloc(200, sizeof(char));
    char * cur_name = (char*) calloc(100, sizeof(char));

    cur_name = getName(files[0], dir);

    while (strcmp(cur_name, name) != 0) {
        strcpy(dir, "");
        cur_name = getName(files[x++], dir);
        printf("%s\n", cur_name);
    }

    strcpy(path, dir);
    strcat(path, "/");
    strcat(path, name);

    free(dir);
    free(cur_name);
}
