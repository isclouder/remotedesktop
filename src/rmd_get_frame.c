/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#include "rmd_types.h"
#include "rmd_setbrwindow.h"

#include <stdlib.h>
#include <syslog.h>
#include <stdint.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/XShm.h>

#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/extensions/shmstr.h>

#include <sys/shm.h>
#include <errno.h>


int InitializeDisplay(ProgData *pdata){
    pdata->specs.width  = DisplayWidth(pdata->dpy, pdata->specs.screen);
    pdata->specs.height = DisplayHeight(pdata->dpy, pdata->specs.screen);
    pdata->specs.root   = RootWindow(pdata->dpy, pdata->specs.screen);
    pdata->specs.visual = DefaultVisual(pdata->dpy, pdata->specs.screen);
    pdata->specs.gc     = DefaultGC(pdata->dpy, pdata->specs.screen);
    pdata->specs.depth  = DefaultDepth(pdata->dpy, pdata->specs.screen);

    if((pdata->specs.depth!=32)&&
       (pdata->specs.depth!=24)&&
       (pdata->specs.depth!=16)){
        syslog(LOG_ERR, "Only 32bpp,24bpp and 16bpp"
                       " color depth modes are currently supported.\n");
        return -1;
    }
    if (!SetBRWindow(pdata->dpy, &pdata->brwin, &pdata->specs, &pdata->args)){
        syslog(LOG_ERR, "SetBRWindow err.\n");
        return -1;
    }

    pdata->specs_target.width  = DisplayWidth(pdata->dpy, pdata->specs_target.screen);
    pdata->specs_target.height = DisplayHeight(pdata->dpy, pdata->specs_target.screen);
    pdata->specs_target.root   = RootWindow(pdata->dpy, pdata->specs_target.screen);
    pdata->specs_target.visual = DefaultVisual(pdata->dpy, pdata->specs_target.screen);
    pdata->specs_target.gc     = DefaultGC(pdata->dpy, pdata->specs_target.screen);
    pdata->specs_target.depth  = DefaultDepth(pdata->dpy, pdata->specs_target.screen);

    return 0;
}
int QueryExtensions(Display *dpy,
                     int *shm_opcode){
    int xf_event_basep,
        xf_error_basep,
        shm_event_base,
        shm_error_base;

    if(!XQueryExtension(dpy,
                        "MIT-SHM",
                        shm_opcode,
                        &shm_event_base,
                        &shm_error_base)){
        syslog(LOG_ERR, "Shared Memory extension not present!\n");
        return -1;
    }
    if(XFixesQueryExtension(dpy,&xf_event_basep,&xf_error_basep)==False){
        syslog(LOG_ERR, "Xfixes extension not present!\n");
        return -1;
    }
    return 0;
}

int GetZPixmap(Display *dpy,
               Window root,
               char *data,
               int x,
               int y,
               int width,
               int height){
    xGetImageReply reply;
    xGetImageReq *request;
    long nbytes;

    LockDisplay(dpy);
    GetReq(GetImage,request);
    request->drawable=root;
    request->x=x;
    request->y=y;
    request->width=width;
    request->height=height;
    request->planeMask=AllPlanes;
    request->format=ZPixmap;
    if((!_XReply(dpy,(xReply *)&reply,0,xFalse))||(!reply.length)){
        UnlockDisplay(dpy);
        SyncHandle();
        return 1;
    }
    nbytes=(long)reply.length<<2;
    _XReadPad(dpy,data,nbytes);
    UnlockDisplay(dpy);
    SyncHandle();
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
        syslog(LOG_ERR, "Failed to obtain Shared Memory segment!\n");
        return 12;
    }
    (pdata->shminfo).shmaddr=(*image)->data=shmat((pdata->shminfo).shmid,
                                                    NULL,0);
    (pdata->shminfo).readOnly = False;
    if(!XShmAttach(pdata->dpy,&(pdata->shminfo))){
        syslog(LOG_ERR, "Failed to attach shared memory to proccess.\n");
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

void paint_cursor(ProgData *pdata)
{
    int x_off = 0;
    int y_off = 0;
    XFixesCursorImage *xcim;
    int width = pdata->image->width;
    int height = pdata->image->height;
    int x, y;
    int line, column;
    int to_line, to_column;
    int pixstride = pdata->image->bits_per_pixel >> 3;
    int8_t *pix = (int8_t *)(pdata->image->data);

    if (pdata->image->bits_per_pixel != 24 && pdata->image->bits_per_pixel != 32)
            return;
    xcim = XFixesGetCursorImage(pdata->dpy);
    x = xcim->x - xcim->xhot;
    y = xcim->y - xcim->yhot;
    to_line = FFMIN((y + xcim->height), (height + y_off));
    to_column = FFMIN((x + xcim->width), (width + x_off));
    for (line = FFMAX(y, y_off); line < to_line; line++) {
            for (column = FFMAX(x, x_off); column < to_column; column++) {
                    int  xcim_addr = (line - y) * xcim->width + column - x;
                    int image_addr = ((line - y_off) * width + column - x_off) * pixstride;
                    int r = (uint8_t)(xcim->pixels[xcim_addr] >>  0);
                    int g = (uint8_t)(xcim->pixels[xcim_addr] >>  8);
                    int b = (uint8_t)(xcim->pixels[xcim_addr] >> 16);
                    int a = (uint8_t)(xcim->pixels[xcim_addr] >> 24);
                    if (a == 255) {
                            pix[image_addr+0] = r;
                            pix[image_addr+1] = g;
                            pix[image_addr+2] = b;
                    } else if (a) {
                            pix[image_addr+0] = r + (pix[image_addr+0]*(255-a) + 255/2) / 255;
                            pix[image_addr+1] = g + (pix[image_addr+1]*(255-a) + 255/2) / 255;
                            pix[image_addr+2] = b + (pix[image_addr+2]*(255-a) + 255/2) / 255;
                    }
            }
    }
    XFree(xcim);
    xcim = NULL;
}

void *GetFrame_cp(ProgData *pdata){
    int init=0;
    char *pxl_data=NULL;
    if((init=InitializeDisplay(pdata))!=0){
        exit(init);
    }

    if((init=QueryExtensions(pdata->dpy,&pdata->shm_opcode))!=0){
        exit(init);
    }
    pxl_data=(char *)malloc(pdata->brwin.nbytes);

    pdata->image=XCreateImage(pdata->dpy,
                            pdata->specs.visual,
                            pdata->specs.depth,
                            ZPixmap,
                            0,
                            pxl_data,
                            pdata->brwin.rrect.width,
                            pdata->brwin.rrect.height,
                            8,
                            0);
     XInitImage(pdata->image);
     GetZPixmap(pdata->dpy,pdata->specs.root,
                   pdata->image->data,
                   pdata->brwin.rrect.x,
                   pdata->brwin.rrect.y,
                   pdata->brwin.rrect.width,
                   pdata->brwin.rrect.height);

    syslog(LOG_INFO, "start get image\n");
    while(pdata->running){
        pthread_mutex_lock(&pdata->time_mutex);
        pthread_cond_wait(&pdata->time_cond, &pdata->time_mutex);
        pthread_mutex_unlock(&pdata->time_mutex);

        pdata->capture_busy = TRUE;

        //display
        GetZPixmap(pdata->dpy,
                   pdata->specs.root,
                   pdata->image->data,
                   pdata->brwin.rrect.x,
                   pdata->brwin.rrect.y,
                   pdata->brwin.rrect.width,
                   pdata->brwin.rrect.height);

        //cursor
        //paint_cursor(pdata);
        //draw
        XPutImage(pdata->dpy, pdata->specs_target.root,pdata->specs_target.gc,pdata->image, 0, 0, 0, 0, pdata->brwin.rrect.width,pdata->brwin.rrect.height);
        pdata->capture_busy = FALSE;
    }

    pthread_exit(&errno);
}

void *GetFrame(ProgData *pdata){
    int init=0;
    if((init=InitializeDisplay(pdata))!=0){
        exit(init);
    }

    if((init=QueryExtensions(pdata->dpy,&pdata->shm_opcode))!=0){
        exit(init);
    }

    if((init=FirstFrame(pdata,&(pdata->image))!=0)){
        exit(init);
    }
    
    syslog(LOG_INFO, "start get image\n");
    while(pdata->running){
        pthread_mutex_lock(&pdata->time_mutex);
        pthread_cond_wait(&pdata->time_cond, &pdata->time_mutex);
        pthread_mutex_unlock(&pdata->time_mutex);

        pdata->capture_busy = TRUE;

        //display
        XShmGetImage(pdata->dpy,pdata->specs.root,
                    (pdata->image),
                    (pdata->brwin.rrect.x),
                    (pdata->brwin.rrect.y),AllPlanes);

        //cursor
        //paint_cursor(pdata);
        //draw
        XShmPutImage(pdata->dpy,pdata->specs_target.root,pdata->specs_target.gc,pdata->image,0,0,0,0,pdata->brwin.rrect.width,pdata->brwin.rrect.height,1);

        pdata->capture_busy = FALSE;
    }

    pthread_exit(&errno);
}

