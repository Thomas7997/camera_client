/*

    source lib : https://elixir.bootlin.com/linux/latest/source/include/linux/usb.h

    usb_host_endoint : données nécessaires pour le régler un endpoint pour les requêtes.
    usb_host_interface : interface de description de l’usb avec des paramètres côté host.
    usb_interface_assoc_descriptor : associations d’interfaces.
    usb_interface_condition : etat de liaison.
    device : informations spécifiques pour l’interface de l’appareil.


    usb_interface : stoque les blocs de paramétrages d’interface host.
    usb_host_config : la configuration de l'usb host, elle peut gérer plusieurs connexions USB.
    usb_config_descriptor : description de la navigation.
    usb_interface_cache : configuration de l'interface qui est stoquée dans le cache quand elle n'est pas activée.


*/

#include <stdio.h>
#include <stdlib.h>
// Pour linux
#include <linux/usb.h>

void handleError (int status) {
    printf ("status : %d\n", status);
}

int getUsbDriver (struct usb_device * usb_dev, char * data) {
    int i = 0;

    while (*(usb_dev+i) != NULL) {
        printf ("path : %s\n\n\n", *(i+usb_dev)->devpath);
        i++;
    }

    printf ("%s\n", data);
    int status = 0;

    // Peut être la fonction principale du script
    // URB request

    // int urb_iso_packets = 0;
    // gfp_t urb_mem_flags;
    // struct urb * URB = usb_alloc_urb(urb_iso_packets, urb_mem_flags);
    // usb_init_urb(URB);

    // status = usb_submit_urb(URB, urb_mem_flags);
    // handleError(status);
    // status = usb_free_urb(URB);
    // handleError(status);
    // status = usb_reset_device(usb_dev);
    // handleError(status);

    return status;
}

int main (void) {
    // connexion USB
    int status = 0;
    char * data = calloc(10000, sizeof(char));
    status = usb_for_each_dev(data, getUsbDriver);
    handleError(status);
    free(data);

    return 0;
}

/*



*/