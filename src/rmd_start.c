/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#include "config.h"

#include "rmd_error.h"
#include "rmd_initialize_data.h"
#include "rmd_threads.h"
#include "rmd_types.h"

#include <X11/Xlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int rmd_start(){
    ProgData pdata;
    
    SetupDefaultArgs(&pdata.args);

    if(XInitThreads ()==0){
        fprintf(stderr,"Couldn't initialize thread support!\n");
        exit(7);
    }
    pdata.dpy = XOpenDisplay(pdata.args.display);
    XSetErrorHandler(rmdErrorHandler);
    if (pdata.dpy == NULL) {
        fprintf(stderr, "Cannot connect to X server %s\n",pdata.args.display);
        exit(9);
    }
    else{
        if(InitializeData(&pdata)==0){
            rmdThreads(&pdata);
            XCloseDisplay(pdata.dpy);
            fprintf(stderr,"Goodbye!\n");
            CleanUp();
        }
    }

    return 0;
}
