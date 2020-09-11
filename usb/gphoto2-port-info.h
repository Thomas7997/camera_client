 
   #ifndef GPHOTO_PORT_INFO_H
   #define GPHOTO_PORT_INFO_H
   
   struct _GPPortInfo {
           GPPortType type;        
           char *name;             
           char *path;             
           /* Private */
           char *library_filename; 
   };
   #endif