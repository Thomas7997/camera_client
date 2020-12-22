#include "auto.h"

unsigned int filterVideos (char ** videos_l, char ** files_l, unsigned int files_n) {
    unsigned int x = 0, i, j, y;
    char ext[5] = "";

    for (i = 0; i < files_n; i++) {
        strcpy(ext, "");
        j = strlen(files_l[i]) - 1;
        y = 0;
        while (files_l[i][j] != '.') {
            ext[y++] = files_l[i][j--];
        }

        if (strcmp(ext, "VOM") == 0 || strcmp(ext, "4PM") == 0) {
            // Il s'agit donc d'une vidéo
            strcpy(videos_l[x++], files_l[i]);
        }
    }

    return x;
}

unsigned int filterPhotos (char ** photos_l, char ** files_l, unsigned int files_n) {
    unsigned int x = 0, i, j, y;
    char ext[5] = "";

    for (i = 0; i < files_n; i++) {
        strcpy(ext, "");
        j = strlen(files_l[i]) - 1;
        y = 0;
        while (files_l[i][j] != '.') {
            ext[y++] = files_l[i][j--];
        }

        if (strcmp(ext, "GPJ") == 0) {
            // Il s'agit donc d'une vidéo
            strcpy(photos_l[x++], files_l[i]);
        }
    }

    return x;
}

unsigned int getImageNumber (char * buffer) {
    unsigned int x = 0, y = strlen(buffer), nb;
    char * nb_str = (char*) calloc(100, sizeof(char));

    while (buffer[y--] != '.');
    while (buffer[y] != '_') {
        nb_str[x++] = buffer[y--];
    }

    mirroir(nb_str, x);
    sscanf(nb_str, "%u", &nb);
    free(nb_str);

    return nb;
}

int compareFilesLists(char ** transferts, char ** files, char ** liste_captures, int files_nb, int nb_list, unsigned int * indexs) {
    unsigned int i = 0, y = 0, newP, x = 0, a = 0;

    for (i = 0; i < files_nb; i++) {
        newP = 0;
        for (y = 0; y < nb_list; y++) {
            if (strcmp(files[i], liste_captures[y]) == 0) {
                newP = 1;
                break;
            }
        }

        if (newP == 0) {
            printf("\n\n\n\n\n\n\n\n\nT : %s\n%d\n", files[i], i);
            sprintf(transferts[a], "%s", files[i]);
            printf("%s\n", transferts[a++]);
        }
    }

    return a;
}

unsigned int save_clist_slist(char ** liste_captures, char ** files, unsigned int files_nb, unsigned int nb_list) {
    unsigned int i = 0;
    
    for (i = 0; i < files_nb; i++) {
        strcpy(liste_captures[i], files[i]);
    }

    return files_nb;
}

void afficher_liste(char ** transferts, unsigned int ret_comp) {
    unsigned int x = 0;

    while (x < ret_comp) {
        printf ("%s\n", transferts[x]);
        x++;
    }
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

unsigned int getVideoDatas (char *** dossiers, char ** dirs_n, char ** photos, char ** files, char ** liste_captures, unsigned int * files_nb, Camera* camera, GPContext * context, unsigned int * dir_sizes) {
    int status = get_files_and_dirs(dossiers, dirs_n, files_nb, dir_sizes, camera, context);
    // generateError(status);
    unsigned int nb_files = 0;

    if (status < 0)  generateError(status);

    nb_files = dossiers_to_list(dossiers, files, dirs_n, *files_nb, dir_sizes);

    // afficher_liste(liste_captures, nb_list);

    nb_files = dossiers_to_list(dossiers, files, dirs_n, *files_nb, dir_sizes);

    // La matière sortante est la liste de vidéos totale
    return filterVideos(photos, files, nb_files);
}

unsigned int getPhotoDatas (char *** dossiers, char ** dirs_n, char ** photos, char ** files, char ** liste_captures, unsigned int * files_nb, Camera* camera, GPContext * context, unsigned int * dir_sizes) {
    int status = get_files_and_dirs(dossiers, dirs_n, files_nb, dir_sizes, camera, context);
    // generateError(status);
    unsigned int nb_files = 0;

    if (status < 0)  generateError(status);

    nb_files = dossiers_to_list(dossiers, files, dirs_n, *files_nb, dir_sizes);

    // afficher_liste(liste_captures, nb_list);

    nb_files = dossiers_to_list(dossiers, files, dirs_n, *files_nb, dir_sizes);

    // La matière sortante est la liste de vidéos totale
    return filterPhotos(photos, files, nb_files);
}