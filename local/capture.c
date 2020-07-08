#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main (void) {
	while (1) {
		system("cd ./data/capture/cloud;gphoto2 --wait-event-and-download=1000s");
		sleep(1000);	
	}
	return 0;
}
