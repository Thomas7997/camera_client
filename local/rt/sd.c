#include "sd.h"

int get_files_and_dirs (Dossiers * dirs_b, Fichiers * dirs_n, Camera * camera, GPContext * context) {
    Dossiers * dossiers_fils = new_dossiers_list();
    Fichiers * fichiers_liste = new_fichiers_list();
    CameraList * folderList;
    char * folder = (char*) calloc(100, sizeof(char));
    gp_list_new(&folderList);
    const char * dir;
    int status = gp_camera_folder_list_folders(camera, "/", folderList, context);
    if (status < 0) return status;

    status = gp_list_get_name(folderList, 0, (const char**) &dir);
    if (status < 0) return status;

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

    // Insérer
    dirs_b = insertenqueue_Dossiers(dossiers_fils, tmp_dir, NULL, NULL);

    sprintf(folder, "%s/DCIM", tmp_dir);
    // printf ("%s\n", folder);

    // Insérer
    dossiers_fils->fils = insertenqueue_Dossiers(dossiers_fils->fils, "DCIM", NULL, NULL);

    status = gp_camera_folder_list_folders(camera,
		folder,
		folderList,
		context
	);
    if (status < 0) return status;

    int local_nb = gp_list_count(folderList);
    int nb_files = 0;

    if (local_nb < 0) return local_nb;

    for (unsigned int i = 0; i < local_nb; i++) {
        // status = gp_list_reset(fileList);

        const char * subdir;
        status = gp_list_get_name(folderList, i, (const char**) &subdir);
        dossiers_fils->fils->fils = insertenqueue_Dossiers(dossiers_fils->fils->fils, subdir, NULL, NULL);

        if (status < 0) return status;

        sprintf (tmp_dir, "%s/%s", folder, subdir);
        // printf ("%s\n", tmp_dir);

        // strcpy(dirs_n[i], tmp_dir);

        status = gp_list_reset(fileList);
        if (status < 0) return status;

        status = gp_camera_folder_list_files(camera, tmp_dir, fileList, context);
        if (status < 0) return status;

        nb_files = gp_list_count(fileList);
        if (nb_files < 0) return status;

        for (unsigned int j = 0; j < nb_files; j++) {
            const char * file;
            status = gp_list_get_name(fileList, j, (const char**) &file);
            // handleError(status);
            if (status < 0) return status;
            // strcpy(dirs_b[i][j], file);
            fichiers_liste = insertenqueue_Fichiers(fichiers_liste, file, "jpg");
            // printf ("%s\n", dirs_b[i][j]);
        }

        // INSÉRER
        dossiers_fils->fils->fils = insertenqueue_Dossiers(dossiers_fils->fils->fils, subdir, fichiers_liste, NULL);

        // sprintf(dirs_n[i], "/%s", dir);
    }

    dirs_b = insertenqueue_Dossiers(dirs_b, "", NULL, dossiers_fils);

    free(tmp_dir);
    free(folder);
    free(tmp);
    gp_list_free(folderList);
    gp_list_free(fileList);
    return 0; // Pas d'erreur
}

void afficher_fichiers_liste (Fichiers * fichiers) {
    if (fichiers != NULL) {
        printf("%s\n", fichiers->nom);
        afficher_fichiers_liste(fichiers->suiv);
    }
}

void afficher_carte_sd_liste (Dossiers * dossiers) {
    if (dossiers != NULL) {
        printf("%s\n", dossiers->nom);
        afficher_fichiers_liste(dossiers->fichiers);
        afficher_carte_sd_liste(dossiers->suiv);
    }
}

Fichiers* insertenqueue_Fichiers (Fichiers * liste, const char * valeur, const char * type) {
    Fichiers * P;
    Fichiers * Q;
    Fichiers * tete;

    P = (Fichiers*) malloc(sizeof(Fichiers));

    strcpy(liste->nom, valeur);
    strcpy(liste->type, type);

    if (liste == NULL) {
        return P;
    }

    else {
        Q = liste;
        tete = liste;
        while (Q->suiv != NULL) {
            Q = Q->suiv;
        }
        Q->suiv = P;
        return tete;
    }
}

Dossiers* insertenqueue_Dossiers (Dossiers * liste, const char * valeur, Fichiers * fichiers, Dossiers * fils) {
    Dossiers * P;
    Dossiers * Q;
    Dossiers * tete;

    P = (Dossiers*) malloc(sizeof(Dossiers));

    strcpy(P->nom, valeur);
    P->fichiers = fichiers;
    strcpy(P->fils->nom, fils->nom);
    P->fils->fichiers = fils->fichiers;

    if (liste == NULL) {
        return P;
    }

    else {
        Q = liste;
        tete = liste;
        while (Q->suiv != NULL) {
            Q = Q->suiv;
        }
        Q->suiv = P;
        return tete;
    }
}

Fichiers * new_fichiers_list (void) {
    Fichiers * fichiers = (Fichiers*) malloc(sizeof(Fichiers));
    fichiers->nom = (char*) malloc(sizeof(char));
    fichiers->type = (char*) malloc(sizeof(char));

    return fichiers;
}

Dossiers * new_dossiers_list (void) {
    Dossiers * dossiers = (Dossiers*) malloc(sizeof(Dossiers));
    dossiers->fichiers = (Fichiers*) new_fichiers_list();
    dossiers->nom = (char*) malloc(sizeof(char));
    dossiers->fils = (Dossiers*) malloc(sizeof(Dossiers));
    dossiers->fils->nom = (char*) malloc(sizeof(char));
    dossiers->fils->fichiers = (Fichiers*) new_fichiers_list();

    return dossiers;
}

void free_fichiers (Fichiers * fichiers) {
    free(fichiers->nom);
    free(fichiers->type);
    free(fichiers);
}

void free_dossiers (Dossiers * dossiers) {
    free(dossiers->nom);
    free_fichiers(dossiers->fichiers);
    free(dossiers->fils->nom);
    free_fichiers(dossiers->fils->fichiers);
    free(dossiers);
}

int get_sd_card_previews (char ** files, unsigned int nb, Camera * camera, GPContext * context) {
    int i, j, status;
    CameraFile * file;
    status = gp_file_new(&file);

    char * dir = (char*) calloc(TAILLE_NOM, sizeof(char));
    char * targetPath = (char*) calloc(100, sizeof(char));

    for (i = 0; i < nb; i++) {
        char * filename = (char*) getName(files[i], dir);
        printf("%s\n%s\n", dir, filename);
        status = gp_camera_file_get(camera, dir, filename, GP_FILE_TYPE_PREVIEW, file, context);
        printf("RECEPTION...\n");

        if (status < 0) return status;

        sprintf(targetPath, "/home/remote/camera_server/public/sd/%s", filename);

        status = gp_file_save(file, (const char*) targetPath);

        printf("SAUVEGARDE ...\n");

        if (status < 0) return status;
    }

    free(dir);
    free(targetPath);
    gp_file_free(file);
}

int sd_card_lecture_mode (char *** dossiers, char ** dirs_n, char ** files, Camera * camera, GPContext * context) {
    int status = 0;

    int i, j, number = 0;
    unsigned int * dir_sizes = (unsigned int*) calloc(MIN_DIRS, sizeof(int));
    unsigned int files_nb;

    for (unsigned int e = 0; e < MIN_DIRS; e++) {
        for (unsigned int j = 0; j < MAX_CAPTURES; j++) {
            strcpy(dossiers[e][j], "");
        }
        strcpy(dirs_n[e], "");          
    }

    for (unsigned int e = 0; e < MAX_CAPTURES*MIN_DIRS; e++) {
        strcpy(files[e], "");
    }

    status = get_files_and_dirs(dossiers, dirs_n, &files_nb, dir_sizes, camera, context);
    handleError(status);

    if (status < 0) return status;

    unsigned int nb_files = dossiers_to_list(dossiers, files, dirs_n, files_nb, dir_sizes);
    printf("%u\n", nb_files);

    status = get_sd_card_previews (files, nb_files, camera, context);
    handleError(status);

    if (status < 0) return status;

    free(dir_sizes);

    return 1;
}
