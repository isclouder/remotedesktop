/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#include "config.h"
#include "rmd_error.h"

#include <X11/Xlib.h>
#include <X11/Xlibint.h>

#include <stdio.h>
#include <stdlib.h>



int rmdErrorHandler( Display *dpy, XErrorEvent *e )
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



