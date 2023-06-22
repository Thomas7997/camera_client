# TELECAM Core app

The main objectives of this part of the project was to manage the usb interface between the Raspberry PI card and any DSLR.  
This app has the ability to manage usb connections, detect the online/offline mode and transfer medias.  

During the offline mode, the USB transfers can keep be processed to save transfered medias to avoid data loss during the USB connection if there is no local connection interruption.  
Then, if some transfers has been made during the offline mode, the saved data can be emited to the connected desktop app over wifi when wireless connection turns on.  

### Global process of a WiFi connection management
![Wifi sync on Smartphone - global](https://github.com/Thomas7997/camera_client/assets/45339466/e56e3584-de6f-4566-8f7b-a3c999c029cc)

### Offline mode
![Offline mode](https://github.com/Thomas7997/camera_client/assets/45339466/ca1cce10-06af-4a65-8c35-b7fd92e6ba42)

