# TELECAM Core app

## The TELECAM project
During the Covid-19, me and my team had the idea to work on a project to make the media transfers possible between a digital camera and a computer over WiFi.  
We made the choice of using the Raspberry Pi computer as the main hardware component to use for the project.  
My role, as a developer, was to write both of the internal programs that run inside the Raspberry Pi and the desktop app to visualize the transfers.  
To realise this project, I have spliten the problem into different components wich are hosted on Github repositories.  
Here is the list of the components :

> * camera_client : Usb / WiFi interfacing (transfers from the camera over USB)
> * camera_server : WiFi transfers from the Raspberry Pi to a computer
> * camera_interface : Desktop app to view the transfers and set some transfer options

## What is TELECAM Core ?
The TELECAM Core's responsibility is to focus on the data transition between the USB connector and the WiFi transmiter which is an essential part of the media transfer.  
This app has the ability to manage usb connections, detect the online/offline mode and transfer medias.  

During the offline mode, the USB transfers can keep be processed to save transfered medias to avoid data loss during the USB connection if there is no local connection interruption.  
Then, if some transfers has been made during the offline mode, the saved data can be emited to the connected desktop app over wifi when wireless connection turns on.  

### Global process of a WiFi connection management
![Wifi sync on Smartphone - global](https://github.com/Thomas7997/camera_client/assets/45339466/e56e3584-de6f-4566-8f7b-a3c999c029cc)

### Offline mode
![Offline mode](https://github.com/Thomas7997/camera_client/assets/45339466/ca1cce10-06af-4a65-8c35-b7fd92e6ba42)

