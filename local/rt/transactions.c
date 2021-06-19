#include "transactions.h"

unsigned int dossiers_to_list (char *** dossiers, char ** list, unsigned int nb_dossiers) {
    unsigned int x = 0, item = 0;

    while (x < nb_dossiers) {
        for (unsigned int y = 0; dossiers[x][y][0] != 0; y++) {
            sprintf(list[item++], "%s", dossiers[x][y]);
        }
        
        x++;
    }

    return item;
}

unsigned int listDir (char ** files, const char * path) {
  DIR *dir;
  struct dirent *entry;
  unsigned int x = 0;

  if ((dir = opendir(path)) == NULL)
    perror("opendir() error");
  else {
    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_name[0] != '.') strcpy(files[x++], entry->d_name);
    } // Store entry->name inside char * list here
    closedir(dir);
  }
  return x;
}

void removeFile (const char * path) {
  int del;
  
  do {
    del = remove(path);
    if (!del)
      printf("The file %s is Deleted successfully\n", path);
    else
      printf("The file %s is not Deleted\n", path);
  } while (!del);
}

unsigned int fill_downloaded_files (char ** files) {
  return listDir(files, "../data/images/downloads");
}

unsigned int fill_deleted_files (char ** files) {
  return listDir(files, "../data/images/deletes");
}

void send_download (char * file) {
  char * cmd = (char*) calloc(200, sizeof(char));

  sprintf(cmd, "../data/images/download/%s mv /home/remote/camera_server/public/download", file);
  system(cmd);

  free(cmd);
}

void send_delete (char * file) {
  char * cmd = (char*) calloc(200, sizeof(char));

  sprintf(cmd, "../data/images/deletes/%s mv /home/remote/camera_server/public/deletes", file);
  system(cmd);

  free(cmd);
}
