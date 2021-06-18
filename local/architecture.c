#include <stdio.h>
#include <stdlib.h>

int main () {
	system("mkdir data data/stop data/images data/images/gets data/images/cloud data/images data/images/tmp data/images/downloads data/images/deletes data/tmp data/log /home/remote/camera_server/public /home/remote/camera_server/public/sd /home/remote/camera_server/public/gets");
	system("echo \"0\" > data/stop/selection.txt;echo \"0\" > data/stop/transferts.txt");
	system("echo \"0\" > data/tmp/camera_connected.txt;echo \"0\" > data/tmp/connected.txt;echo \"0\" > data/tmp/errors.txt;echo \"0\" > data/tmp/wifi_status.txt;echo \"1\" > data/tmp/transfert_choice.txt;echo \"1\" > data/tmp/normal_format.txt;echo \"0\" > data/tmp/raw_format.txt;echo \"1\" > data/tmp/first_use.txt;touch data/tmp/downloads.txt;touch data/tmp/deletes.txt");
	return 0;
}
