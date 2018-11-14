/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#include "rmd_initialize_data.h"
#include "rmd_types.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int InitializeData(ProgData *pdata){
    fprintf(stderr,"Initializing...\n");


    pthread_mutex_init(&pdata->time_mutex, NULL);
    pthread_cond_init(&pdata->time_cond,NULL);
    pdata->image               = NULL;
    pdata->running             = TRUE;
    pdata->capture_busy        = FALSE;
    return 0;

}

void SetupDefaultArgs(ProgArgs *args) {
    
    args->windowid             = 0;
    args->x                    = 0;
    args->y                    = 0;
    args->width                = 0;
    args->height               = 0;
    args->full_shots           = 0;
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