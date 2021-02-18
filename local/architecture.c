#include <stdio.h>
#include <stdlib.h>

int main () {
	system("mkdir data data/stop data/image data/image/cloud data/images data/images/gets data/images/cloud data/image/capture data/images data/images/tmp data/tmp data/log /home/remote/camera_server/public /home/remote/camera_server/public/sd");
	system("echo \"0\" > data/stop/selection.txt;echo \"0\" > data/stop/transferts.txt");
	system("echo \"0\" > data/tmp/camera_connected.txt;echo \"0\" > data/tmp/connected.txt;echo \"0\" > data/tmp/errors.txt;echo \"0\" > data/tmp/wifi_status.txt;echo \"1\" > data/tmp/transfert_choice.txt");
	return 0;
}
