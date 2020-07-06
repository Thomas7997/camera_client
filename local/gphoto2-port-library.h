#ifndef __GPHOTO2_PORT_LIBRARY_H__
    #define __GPHOTO2_PORT_LIBRARY_H__
    
    #include <gphoto2/gphoto2-port-info-list.h>
    #include <gphoto2/gphoto2-port.h>
    
    typedef struct _GPPortOperations {
            int (*init)     (GPPort *);
            int (*exit)     (GPPort *);
            int (*open)     (GPPort *);
            int (*close)    (GPPort *);
            int (*read)     (GPPort *,       char *, int);
           int (*check_int)(GPPort *,      char *, int, int);
            int (*write)    (GPPort *, const char *, int);
            int (*update)   (GPPort *);
    
            /* Pointers to devices. Please note these are stubbed so there is
             no need to #ifdef GP_PORT_* anymore. */
    
            /* for serial devices */
            int (*get_pin)   (GPPort *, GPPin, GPLevel*);
            int (*set_pin)   (GPPort *, GPPin, GPLevel);
            int (*send_break)(GPPort *, int);
            int (*flush)     (GPPort *, int);
    
            /* for USB devices */
            int (*find_device)(GPPort * dev, int idvendor, int idproduct);
            int (*find_device_by_class)(GPPort * dev, int class, int subclass, int protocol);
           int (*clear_halt) (GPPort * dev, int ep);
            int (*msg_write)  (GPPort * dev, int request, int value, int index,
                                    char *bytes, int size);
           int (*msg_read)   (GPPort * dev, int request, int value, int index,
                                    char *bytes, int size);
            int (*msg_interface_write)  (GPPort * dev, int request, 
                                    int value, int index, char *bytes, int size);
            int (*msg_interface_read)  (GPPort * dev, int request, 
                                    int value, int index, char *bytes, int size);
            int (*msg_class_write) (GPPort * dev, int request, 
                                   int value, int index, char *bytes, int size);
            int (*msg_class_read) (GPPort * dev, int request, 
                                    int value, int index, char *bytes, int size);
    
            /* For USB disk direct IO devices */
           int (*seek) (GPPort * dev, int offset, int whence);
   
            /* For USB Mass Storage raw SCSI ports */
            int (*send_scsi_cmd) (GPPort *port, int to_dev,
                                    char *cmd, int cmd_size,
                                    char *sense, int sense_size,
                                    char *data, int data_size);
    
            int (*reset)     (GPPort *);
    
    } GPPortOperations;
    
   typedef GPPortType (* GPPortLibraryType) (void);
    typedef int (* GPPortLibraryList)       (GPPortInfoList *list);
    
    typedef GPPortOperations *(* GPPortLibraryOperations) (void);
    
    /*
   94  * If you want to write an io library, you need to implement the following
   95  * functions. Everything else in your io library should be declared static.
     */
    
   GPPortType gp_port_library_type       (void);
  int gp_port_library_list       (GPPortInfoList *list);
   
  GPPortOperations *gp_port_library_operations (void);
   
  #endif /* __GPHOTO2_PORT_LIBRARY_H__ */
