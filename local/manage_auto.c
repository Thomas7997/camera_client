#include "manage_auto.h"

// int generateError (int status) {
//     // Notifier une erreur en fonction de ce qu'il s'agit.

//     // Pour une optimisation de vitesse, il serait préférable d'utiliser un système de fichiers et un second script de lecture de fichiers et de lancement de requêtes CURL
//     // Écire le status dans un fichier
//     FILE * ERR = fopen("data/tmp/errors.txt", "r+");
//     int previousStatus = 0;

//     switch (status)
//     {
//     case -53:
//         // Rebrancher la télécommande
//         printf ("Il faut rebrancher la télécommande.\n");
//         break;
    
//     default:
//         break;
//     }

//     if (fscanf(ERR, "%d", &previousStatus) == 1 && previousStatus != status) {
//         send_status_request(status);
//         fprintf(ERR, "%d\n", status);
//     }

//     printf("GENERATE ERROR %d\n", status);
//     fclose(ERR);

//     return status;
// }

int getCameraModel (Camera * cam) {
    CameraAbilities cam_abilities;
    FILE * MODEL = fopen("data/tmp/model.txt", "w");
    char * camera_model = (char*) calloc(50, sizeof(char));
    int status = gp_camera_get_abilities(cam, &cam_abilities);

    if (status < 0) {
        free(camera_model);
        fclose(MODEL);
        // return generateError(status);
        handleError(status);
    }

    strcpy(camera_model, cam_abilities.model);

    fprintf (MODEL, "%s", camera_model);
    printf ("modèle : %s\n", camera_model);

    free(camera_model);
    fclose(MODEL);
    return 0;
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

void getCaptureEvent(Camera * camera, GPContext * context) {
    int status = 0;
    CameraEventType e_type;
    char * data;
    struct timeval	start, curtime;
    gettimeofday (&start, NULL);

    printf ("WAITING ...\n");

    while (1) {
        printf ("ANALYSE...\n");
        int timediff;
        gettimeofday (&curtime, NULL);
        timediff = ((curtime.tv_sec - start.tv_sec)*1000)+((curtime.tv_usec - start.tv_usec)/1000);

        status = gp_camera_wait_for_event(camera, 1000, &e_type, (void**) &data, context);
        if (e_type == GP_EVENT_FILE_CHANGED) {
            printf ("%s\n", data);
        }        
    }

    printf("FINISHED\n");
}