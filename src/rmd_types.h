/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#ifndef RMDTYPES_H
#define RMDTYPES_H 1

#include "config.h"

#include <stdio.h>
#include <pthread.h>
#include <zlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

//this type exists only
//for comparing the planes at caching.
//u_int64_t mught not be available everywhere.
//The performance gain comes from casting the unsigned char
//buffers to this type before comparing the two blocks.
//This is made possible by the fact that blocks
//for the Y plane are 16 bytes in width and blocks
//for the U,V planes are 8 bytes in width
#ifdef HAVE_U_INT64_T
typedef u_int64_t cmp_int_t;
#else
typedef u_int32_t cmp_int_t;
#endif

// Boolean type
typedef int boolean;
#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE  (!FALSE)
#endif

// Forward declarations 
typedef struct _ProgData ProgData;

typedef struct _DisplaySpecs{   //this struct holds some basic information
    int screen;                 //about the display,needed mostly for
    unsigned int width, height; //validity checks at startup
    Window root;
    Visual *visual;
    GC gc;
    int depth;
}DisplaySpecs;

typedef struct _BRWindow{   //'basic recorded window' specs
    XRectangle rect;         //window attributes
    XRectangle rrect;        //part of window that is recorded
    int nbytes;             //size of zpixmap when screenshoting
    Window windowid;        //id
}BRWindow;

//defaults in the following comment lines may be out of sync with reality
//check the SetupDefaultArgs() function further bellow
typedef struct _ProgArgs{
    Window windowid;    //window to record(default root)
    char *display;      //display to connect(default :0)
    int x,y;            //x,y offset(default 0,0)
    unsigned int width,height;   //defaults to window width and height
    float fps;              //desired framerate(default 15)
    int full_shots;     //do not poll damage, take full screenshots
}ProgArgs;

typedef struct _HotKey{     //Hold info about the shortcuts
    int modnum;             //modnum is the number of modifier masks
    unsigned int mask[4];  //that should be checked (the initial
    int key;                //user requested modifier plus it's
}HotKey;                    //combinations with LockMask and NumLockMask).

//this structure holds any data related to the program
//It's usage is mostly to be given as an argument to the
//threads,so they will have access to the program data, avoiding
//at the same time usage of any globals.
struct _ProgData {
/**remoteDesktop specific structs*/
    ProgArgs args;          //the program arguments
    XImage *image;
    XShmSegmentInfo shminfo;
    DisplaySpecs specs;     //Display specific information
    DisplaySpecs specs_target;     //Display specific information
    BRWindow brwin;         //recording window
    HotKey      pause_key,  //Shortcuts
                stop_key;
/**X related info*/
    Display *dpy;           //curtrent display
/**Condition Variables*/
    pthread_cond_t  time_cond;  //this gets a broadcast by the handler
/**Buffers,Flags and other vars*/
    int damage_event,       //damage event base code
        damage_error,       //damage error base code
        timer_alive;        //determines loop of timer thread

    /** Progam state vars */
    boolean running;             //1 while the program is capturing/paused/encoding

    boolean capture_busy;

    pthread_mutex_t time_mutex;

};

#endif

