/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#include "rmd_initialize_data.h"
#include "rmd_types.h"

#include <syslog.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int InitializeData(ProgData *pdata){
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

void SetupDefaultArgs(ProgArgs *args) {
    
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


void CleanUp(void){


}
