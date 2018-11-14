/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#include "rmd_timer.h"
#include "rmd_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


void *rmdTimer(ProgData *pdata){
    
    long unsigned int usecs_tw=(1000000)/pdata->args.fps;

    while(pdata->timer_alive){

        pthread_mutex_lock(&pdata->time_mutex);
        pthread_cond_broadcast(&pdata->time_cond);
        pthread_mutex_unlock(&pdata->time_mutex);
        
        usleep(usecs_tw);
    }

    pthread_exit(&errno);
}



