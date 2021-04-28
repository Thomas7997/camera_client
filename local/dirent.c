#define _POSIX_SOURCE
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple script that will replace the system() one.

void removeFile (const char * path) {
  int del;
  
  do {
    del = remove(path);
    if (!del)
      printf("The file %s is Deleted successfully\n", path);
    else
      printf("The file %s is not Deleted\n", path);
  } while (del);
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

void displayList (char ** list, unsigned int n) {
  for (unsigned int i = 0; i < n; i++) {
    printf("%s\n", list[i]);
  }
}

int main(int argc, char ** argv) {
  char ** files = calloc(100, sizeof(char*));

  for (unsigned int i = 0; i < 100; i++) {
    files[i] = calloc(100, sizeof(char*));
  }

  unsigned int n = listDir(files, argv[1]);
  displayList(files, n);
  removeFile("dirent");

  for (unsigned int i = 0; i < 100; i++) {
    free(files[i]);
  }

  free(files);
}