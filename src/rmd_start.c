/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#include "config.h"

#include "rmd_types.h"
#include "rmd_get_frame.h"
#include "rmd_timer.h"
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

static void rmdThreads(ProgData *pdata){
    pthread_t   image_capture_t,
                timer_t;

    pthread_create(&image_capture_t,
                   NULL,
                   (void *)GetFrame,
                   (void *)pdata);

    pdata->timer_alive=1;
    pthread_create(&timer_t,
                   NULL,
                   (void *)rmdTimer,
                   (void *)pdata);

    pthread_join(image_capture_t,NULL);
    syslog(LOG_ERR, "Shutting down.");

    pdata->timer_alive=0;
    pthread_join(timer_t,NULL);
}

static void SetupDefaultArgs(ProgArgs *args) {
    args->windowid             = 0;
    args->x                    = 0;
    args->y                    = 0;
    args->width                = 0;
    args->height               = 0;
    args->fps                  = 20;

    if (getenv("DISPLAY") != NULL) {
        args->display = (char *) malloc(strlen(getenv("DISPLAY")) + 1);
        strcpy(args->display, getenv("DISPLAY"));
    }
    else {
        args->display = ":0";
    }
}

static int InitializeData(ProgData *pdata){
    syslog(LOG_INFO, "Initializing...\n");
    pthread_mutex_init(&pdata->time_mutex, NULL);
    pthread_cond_init(&pdata->time_cond,NULL);
    pdata->image               = NULL;
    pdata->running             = TRUE;
    pdata->capture_busy        = FALSE;
    pdata->specs.screen        = DefaultScreen(pdata->dpy);
    pdata->specs_target.screen = 1;
    return 0;
}

static int rmdErrorHandler( Display *dpy, XErrorEvent *e )
{
    char error_desc[1024];
    XGetErrorText(dpy,e->error_code,error_desc,sizeof(error_desc));
    fprintf(stderr,"X Error: %s\n",error_desc);
    fflush(stderr);
    if((e->error_code==BadWindow)&&(e->request_code==X_GetWindowAttributes)){
        fprintf(stderr,"BadWindow on XGetWindowAttributes.\nIgnoring...\n");
        fflush(stderr);
        return 0;
    }
    else if((e->error_code==BadAccess)&&(e->request_code==X_GrabKey)){
        fprintf(stderr,"Bad Access on XGrabKey.\n"
                        "Shortcut already assigned.\n");
        fflush(stderr);
        return 0;
    }
    else
        exit(1);
}

int rmd_start(){
    ProgData pdata;
    
    SetupDefaultArgs(&pdata.args);

    if(XInitThreads ()==0){
        syslog(LOG_ERR, "Couldn't initialize thread support!\n");
        exit(7);
    }
    pdata.dpy = XOpenDisplay(pdata.args.display);
    XSetErrorHandler(rmdErrorHandler);
    if (pdata.dpy == NULL) {
        syslog(LOG_ERR, "Cannot connect to X server %s\n",pdata.args.display);
        exit(9);
    }
    else{
        if(InitializeData(&pdata)==0){
            rmdThreads(&pdata);
            XCloseDisplay(pdata.dpy);
            syslog(LOG_ERR, "Goodbye!\n");
        }
    }

    return 0;
}
