# TELECAM Core app

The main objectives of this part of the project was to manage the usb interface between the Raspberry PI card and any DSLR.  
This app has the ability to manage usb connections, detect the online/offline mode and transfer medias.  

During the offline mode, the USB transfers can keep be processed to save transfered medias to avoid data loss during the USB connection if there is no local connection interruption.  
Then, if some transfers has been made during the offline mode, the saved data can be emited to the connected desktop app over wifi when wireless connection turns on.  

### Global process of a WiFi connection management
![Wifi sync on Smartphone - global](https://github.com/Thomas7997/camera_client/assets/45339466/4aea8cc7-db51-444e-bb43-675f7c501f24)

### Offline mode
![Offline mode](https://github.com/Thomas7997/camera_client/assets/45339466/4baa740a-b139-461b-b044-ec887fd3546f)
