/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#include "rmd_types.h"
#include "rmd_queryextensions.h"
#include "rmd_setbrwindow.h"

#include <stdlib.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/XShm.h>

#include <sys/shm.h>
#include <errno.h>


int InitializeDisplay(ProgData *pdata){
    pdata->specs.screen = DefaultScreen(pdata->dpy);
    pdata->specs.width  = DisplayWidth(pdata->dpy, pdata->specs.screen);
    pdata->specs.height = DisplayHeight(pdata->dpy, pdata->specs.screen);
    pdata->specs.root   = RootWindow(pdata->dpy, pdata->specs.screen);
    pdata->specs.visual = DefaultVisual(pdata->dpy, pdata->specs.screen);
    pdata->specs.gc     = DefaultGC(pdata->dpy, pdata->specs.screen);
    pdata->specs.depth  = DefaultDepth(pdata->dpy, pdata->specs.screen);

    if((pdata->specs.depth!=32)&&
       (pdata->specs.depth!=24)&&
       (pdata->specs.depth!=16)){
        fprintf(stderr,"Only 32bpp,24bpp and 16bpp"
                       " color depth modes are currently supported.\n");
        return -1;
    }
    if (!SetBRWindow(pdata->dpy, &pdata->brwin, &pdata->specs, &pdata->args))
        return -1;
    QueryExtensions(pdata->dpy,
                    &pdata->args,
                    &pdata->damage_event,
                    &pdata->damage_error);

    pdata->specs_target.screen = 1;
    pdata->specs_target.width  = DisplayWidth(pdata->dpy, pdata->specs_target.screen);
    pdata->specs_target.height = DisplayHeight(pdata->dpy, pdata->specs_target.screen);
    pdata->specs_target.root   = RootWindow(pdata->dpy, pdata->specs_target.screen);
    pdata->specs_target.visual = DefaultVisual(pdata->dpy, pdata->specs_target.screen);
    pdata->specs_target.gc     = DefaultGC(pdata->dpy, pdata->specs_target.screen);
    pdata->specs_target.depth  = DefaultDepth(pdata->dpy, pdata->specs_target.screen);

    return 0;
}
int FirstFrame(ProgData *pdata,
                      XImage **image) {

    char *pxl_data=NULL;
    (*image)=XShmCreateImage(pdata->dpy,
                                 pdata->specs.visual,
                                 pdata->specs.depth,
                                 ZPixmap,
                                 pxl_data,
                                 &(pdata->shminfo),
                                 pdata->brwin.rrect.width,
                                 pdata->brwin.rrect.height);
    (pdata->shminfo).shmid=shmget(IPC_PRIVATE,
                                (*image)->bytes_per_line*
                                (*image)->height,
                                IPC_CREAT|0777);
    if((pdata->shminfo).shmid==-1){
        fprintf(stderr,"Failed to obtain Shared Memory segment!\n");
        return 12;
    }
    (pdata->shminfo).shmaddr=(*image)->data=shmat((pdata->shminfo).shmid,
                                                    NULL,0);
    (pdata->shminfo).readOnly = False;
    if(!XShmAttach(pdata->dpy,&(pdata->shminfo))){
        fprintf(stderr,"Failed to attach shared memory to proccess.\n");
        return 12;
    }
    XShmGetImage(pdata->dpy,
                 pdata->specs.root,
                 (*image),
                 pdata->brwin.rrect.x,
                 pdata->brwin.rrect.y,
                 AllPlanes);

    return 0;
}


void *GetFrame(ProgData *pdata){
    int init=0;
    if((init=InitializeDisplay(pdata))!=0){
        exit(init);
    }

    if((init=FirstFrame(pdata,&(pdata->image))!=0)){
        exit(init);
    }
    
    fprintf(stderr,"start get image\n");
    while(pdata->running){
        pthread_mutex_lock(&pdata->time_mutex);
        pthread_cond_wait(&pdata->time_cond, &pdata->time_mutex);
        pthread_mutex_unlock(&pdata->time_mutex);

        pdata->capture_busy = TRUE;

        XShmGetImage(pdata->dpy,pdata->specs.root,
                    (pdata->image),
                    (pdata->brwin.rrect.x),
                    (pdata->brwin.rrect.y),AllPlanes);
        XShmPutImage(pdata->dpy,pdata->specs_target.root,pdata->specs_target.gc,pdata->image,0,0,0,0,pdata->brwin.rrect.width,pdata->brwin.rrect.height,1);

        pdata->capture_busy = FALSE;
    }

    pthread_exit(&errno);
}

