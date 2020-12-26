#include "usb_transactions.h"

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
    // printf ("%s\n", folder);

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
        // printf ("%s\n", tmp_dir);

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
            // handleError(status);
            if (status < 0) return generateError(status);
            strcpy(dirs_b[i][j], file);
            // printf ("%s\n", dirs_b[i][j]);
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

// Pour l'envoi
int transferer_noms (char ** liste, unsigned int n_transferts, GPContext * context, Camera * camera) {
    int i = 0;

    char * commande = (char*) calloc(250, sizeof(char));
    int file_transfered = 0;

    printf ("1\n");
    FILE * HISTORIQUE = fopen("/home/remote/camera_client/local/data/images/historique.txt", "a+");
    printf("2\n");

    char ** hist_lines = (char**) calloc(MAX_CAPTURES, sizeof(char*));

    int x;

    for (x = 0; x < MAX_CAPTURES; x++) {
        hist_lines[x] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    x = 0;

    while (fgets(hist_lines[x++], TAILLE_NOM, HISTORIQUE));

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
            sprintf(commande, "../data/images/gets/%s", filename);
            printf ("%s\n", commande);
            status = gp_camera_file_get(camera, dossier, filename, GP_FILE_TYPE_NORMAL, file, context);
            handleError(status);
            printf("%s\n", dossier);

            if (status < 0) return generateError(status);

            // TÉLÉCHARGEMENT DE LA PHOTO UNIQUEMENT
            status = gp_file_save(file, (const char*) commande);

            if (status < 0) return generateError(status);

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
    free(hist_lines);
    free(filename);
    free(commande);
    free(dossier);
    gp_file_free(file);

    return 0;
}

int compare_file_historique (char * file, char ** historique, int lines) {
    int i;
    unsigned int file_s = strlen(file)-1;

    for (i = 0; i < lines; i++) {
        if (strncmp(historique[i], file, file_s) == 0) {
           return 1;
        }
    }

    return 0;
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