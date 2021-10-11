#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-port-log.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>
#include <gphoto2/gphoto2-list.h>
#include <gphoto2/gphoto2-abilities-list.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/usbdevice_fs.h>

#define MAX 80
#define MAX_CAPTURES 10000
#define TAILLE_NOM 100
#define MIN_DIRS 10
#define NB_PARTS 2
#define PART_NB 50
// #define TRUE 1 == 1
// #define FALSE 1 == 0
#define UNKNOWN_MIME_TYPE 0
#define MOV_MIME_TYPE 1
#define MP4_MIME_TYPE 2
#define JPG_MIME_TYPE 3
#define CR2_MIME_TYPE 4

int x_sd = -1;

int reset_usb_dev (const char * filename) {
    int fd, rc;
    fd = open(filename, O_WRONLY);
    if (fd < 0) {
        perror("Error opening output file");
        return 1;
    }
    printf("Resetting USB device %s\n", filename);
    rc = ioctl(fd, USBDEVFS_RESET, 0);
    if (rc < 0) {
        perror("Error in ioctl");
        return 1;
    }
    close(fd);
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

// Bonne version
int
recursive_directory(char ** files, Camera *camera, const char *folder, GPContext *context, unsigned int * x) {
	int		i, ret;
	CameraList	*list;
	const char	*newfile;
	CameraFileInfo	fileinfo;
	CameraFile	*file;

	ret = gp_list_new (&list);
	if (ret < GP_OK) {
		printf ("Could not allocate list.\n");
		return ret;
	}

	ret = gp_camera_folder_list_folders (camera, folder, list, context);
	if (ret < GP_OK) {
		printf ("Could not list folders.\n");
		
        gp_list_free (list);
		return ret;
	}
	gp_list_sort (list);

	for (i = 0; i < gp_list_count (list); i++) {
		const char *newfolder;
		char *buf;
		int havefile = 0;

		gp_list_get_name (list, i, &newfolder);

		if (!strlen(newfolder)) continue;

		buf = (char*) malloc (strlen(folder) + 1 + strlen(newfolder) + 1);
		strcpy(buf, folder);
		if (strcmp(folder,"/"))		/* avoid double / */
			strcat(buf, "/");
		strcat(buf, newfolder);

		fprintf(stderr,"newfolder=%s\n", newfolder);

		ret = recursive_directory (files, camera, buf, context, x);
		free (buf);
		if (ret < GP_OK) {
			gp_list_free (list);
			printf ("Failed to recursively list folders.\n");
			return ret;
		}
		if (havefile) /* only look for the first directory with a file */
			break;
	}
	gp_list_reset (list);

	ret = gp_camera_folder_list_files (camera, folder, list, context);
	if (ret < GP_OK) {
		gp_list_free (list);
		printf ("Could not list files.\n");
		return ret;
	}
	gp_list_sort (list);
	if (gp_list_count (list) <= 0) {
		gp_list_free (list);
		return GP_OK;
	}

    int countFiles = gp_list_count(list);
    if (countFiles < 0) return countFiles;

    for (int z = 0; z < countFiles; z++) {
        gp_list_get_name (list, z, &newfile);
        sprintf(files[++x_sd], "%s/%s", folder, newfile);
    }

    // Reset here
    *x = x_sd + 1;

	gp_list_free (list);
	return GP_OK;
}

// Fonction principale pour la lecture de fichiers
int get_files (char ** files, Camera * camera, GPContext * context, unsigned int * x) {
    x_sd = -1;
    int status = recursive_directory(files, camera, "/", context, x);
    handleError(status);

    if (status < GP_OK) return status;

    return GP_OK;
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

void mirroir (char * buf, unsigned int n) {
    int i = 0;
    char car;

    for (i = 0; i < (n+1)/2; i++) {
        car = buf[n-i-1];
        buf[n-i-1] = buf[i];
        buf[i] = car;
    }
}

int generateError (int status) {
    // Notifier une erreur en fonction de ce qu'il s'agit.

    // J'utilise un système de fichiers et un second script de lecture de fichiers et de lancement de requêtes CURL
    // Écire le status dans un fichier
    // FILE * ERR = fopen("data/tmp/errors.txt", "r");
    FILE * NOTIFICATION;
    int previousStatus = 0;

    // NOTIFICATION = fopen("data/tmp/errors.txt", "w");
    
    // Envoyer l'ordre de notifier vers un autre script qui détecte le changement de status
    
    //fprintf(NOTIFICATION, "%d\n", status);
    //fclose(NOTIFICATION);

    printf("GENERATE ERROR %d\n", status);
    // fclose(ERR);

    return status;
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
                sprintf(commande, "mv data/images/gets/%s /home/thomas/camera_server/public", filename);
                // system(commande);
                // send_request(filename);
            }

            else {
                sprintf(commande, "mv data/images/gets/%s data/images/cloud", filename);
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


int getDay (void) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mday;
}

void RemplirLignes (char ** lns1, char ** lns2) {
    unsigned int i = 0;

    while (lns2[i][0] != 0) lns1[i] = lns2[i];
    return;
}

static int
selection_wait_event (Camera ** camera, GPContext *context, unsigned int * nroftransferts, char ** files, const char * filename) {
    int waittime = 100;
    static int nrofqueue=0;
    static int nrdownloads=0;

    static const char *bufferx;
    static const char *buffer;
	CameraEventType	evtype;
	CameraFilePath	*path;
	void		*data;
	int		retval;
    struct timeval	start, curtime;
    unsigned int x = 0;
    int selectionInterrupted = 0;

    int status = 0;
    int fd, rc;

    gettimeofday (&start, NULL);
	data = NULL;
	if (nrofqueue) waittime = 10; /* just drain the event queue */

	while (!selectionInterrupted) {
		int timediff;

		retval = gp_camera_wait_for_event(*camera, 10, &evtype, &data, context);
		if (retval != GP_OK) {
			fprintf (stderr, "return from waitevent in trigger sample with %d\n", retval);
			return retval;
		}
		path = data;

        selectionInterrupted = evtype == GP_EVENT_CAPTURE_COMPLETE;

        if (evtype == GP_EVENT_CAPTURE_COMPLETE) {
            printf("Capture complete !\n");
            status = gp_camera_exit(*camera, context);
            handleError(status);
            if (status < 0) return status;
            printf ("1\n");

            reset_usb_dev(filename);

            status = gp_camera_init(*camera, context);
            if (status < 0) return status;
            printf ("4\n");
        }

        if (evtype == GP_EVENT_FILE_CHANGED && path) {
            printf("%s/%s changed !\n", path->folder, path->name);
            sprintf(files[x++], "%s/%s", path->folder, path->name);
        }
    }

    *nroftransferts = x;
	return GP_OK+x;
}

int control_selection (Camera * camera, GPContext *context, unsigned int * nroftransferts, char ** files, const char * filename) {
    // Will be the new main process of a task
    FILE * STATE;
    int status = 0;
    int st = 0;
    int x = 0;

    while (1) {
        STATE = fopen("./data/tmp/selection.txt", "r");
        if (fscanf(STATE, "%d", &st) != 1) continue;

        if (st) {
            printf ("Working ...\n");

            status = selection_wait_event(&camera, context, nroftransferts, files, filename);
            fclose(STATE);
            if (status < 0) {
                // Go to usb connection
                // Send alerts

                handleError(status);
            }

            STATE = fopen("./data/tmp/selection.txt", "w");
            fprintf(STATE, "0");
            fclose (STATE);
        }

        else {
		    printf("Interrupted\n");
		    fclose(STATE);
		    usleep(500000);
        }
	}

    *nroftransferts = x;
	return GP_OK;
}

/*
int eachFileRating_2 (char ** files, char ** transferts, unsigned int files_nb, unsigned int * transferts_nb, Camera * camera, GPContext * context) {
    printf("For each rating\n");
    int y = 0;
    int rates = 0;

    char * nom = (char*) calloc(100, sizeof(char));
    char * data = (char*) calloc(150000, sizeof(char));
    char * dirname = (char*) calloc(100, sizeof(char));

    // FILE * RATING = fopen("data/images/rating.txt", "w");

    int i, x = 0;

    for (int y = 0; y < PART_NB; y++) {
        // Commande
        nom = getName(files[y], dirname);
        printf("%s\n%s\n", nom, dirname);
        int status = getPlacements(&rates, dirname, nom, data, context, camera);

        if (status < 0) return generateError(status);

        if (rates == 5) {
            sprintf(transferts[x++], "%s/%s", dirname, nom);
            printf("%s rated 5 *\n", transferts[x-1]);
        }

        // fprintf(RATING, "%s : %d\n", files[y], rates);
    }

    // fclose(RATING);
    free(nom);
    free(data);
    free(dirname);

    *transferts_nb = x;

    return 0;
}
*/

int main (int argc, char ** argv) {
    int status = 0;
    char *** dossiers = (char***) calloc(MIN_DIRS, sizeof(char**));

    reset_usb_dev(argv[1]);

    Camera * camera;
    GPContext *context = sample_create_context();

    char ** liste_captures = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char ** transferts = (char**) calloc(MAX_CAPTURES, sizeof(char*));
    char ** dirs_n = (char**) calloc(MIN_DIRS, sizeof(char*));
    char ** files = (char**) calloc(MIN_DIRS*MAX_CAPTURES, sizeof(char*));
    unsigned int * dir_sizes = (unsigned int*) calloc(1000, sizeof(unsigned int));
    char ** images_list = (char**) calloc(PART_NB, sizeof(char*));

    for (unsigned int d = 0; d < MIN_DIRS; d++) {
        dossiers[d] = (char**) calloc(MAX_CAPTURES, sizeof(char*));
        for (int dy = 0; dy < MAX_CAPTURES; dy++) {
            dossiers[d][dy] = (char*) calloc(TAILLE_NOM, sizeof(char));
        }
        dirs_n[d] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < PART_NB; i++) {
        images_list[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < MAX_CAPTURES; i++) {
        liste_captures[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
        transferts[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    for (unsigned int i = 0; i < MIN_DIRS*MAX_CAPTURES; i++) {
        files[i] = (char*) calloc(TAILLE_NOM, sizeof(char));
    }

    // DÉBUT RÉPÉTITIONS

    int stop = -1;
    status = 1;

    while (status != 0) {
        gp_camera_new (&camera);
        status = gp_camera_init(camera, context);
        handleError(status);

        if (status < 0) {
            generateError(status);
            gp_camera_exit(camera, context);
            gp_camera_free(camera);
        }

        usleep(5000);
    }

    int i, j, number = 0;

    i = 0;

    unsigned int files_nb = 0, transferts_nb = 0;

    for (unsigned int e = 0; e < MIN_DIRS; e++) {
        for (unsigned int j = 0; j < MAX_CAPTURES; j++) {
            strcpy(dossiers[e][j], "");
        }            
    }

    for (unsigned int e = 0; e < MAX_CAPTURES; e++) {
        strcpy(transferts[e], "");
    }

    for (unsigned int e = 0; e < MAX_CAPTURES*MIN_DIRS; e++) {
        strcpy(files[e], "");
    }

    for (unsigned int e = 0; e < PART_NB; e++) {
        strcpy(images_list[e], "");
    }
    
    status = get_files (files, camera, context, &files_nb);

    status = control_selection (camera, context, &transferts_nb, files, argv[1]);

    if (status < 0) return status;

    int online = 0;

    status = transferer_noms(transferts, transferts_nb, context, camera, online);

    if (status < 0) return status;

    gp_camera_exit(camera, context);
    gp_camera_free(camera);

    // FIN RÉPÉTITIONS

    for (int i = 0; i < MAX_CAPTURES; i++) {
        free(liste_captures[i]);
        free(transferts[i]);
    }

    for (int d = 0; d < MIN_DIRS; d++) {
        for (int dy = 0; dy < MAX_CAPTURES; dy++) {
            free(dossiers[d][dy]);
        }
        free(dossiers[d]);
        free(dirs_n[d]);
    }

    for (int i = 0; i < MIN_DIRS*MAX_CAPTURES; i++) {
        free(files[i]);
    }

    for (unsigned int i = 0; i < PART_NB; i++) {
        free(images_list[i]);
    }

    free(dirs_n);
    free(liste_captures);
    free(transferts);
    free(dir_sizes);
    free(dossiers);
    free(files);
    free(images_list);

    return 0;
}
