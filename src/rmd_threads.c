/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#include "config.h"
#include "rmd_threads.h"

#include "rmd_get_frame.h"
#include "rmd_timer.h"
#include "rmd_types.h"

#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void rmdThreads(ProgData *pdata){
    pthread_t   image_capture_t,
                timer_t;

    /*start threads*/
    pthread_create(&image_capture_t,
                   NULL,
                   (void *)GetFrame,
                   (void *)pdata);

    pdata->timer_alive=1;
    pthread_create(&timer_t,
                   NULL,
                   (void *)rmdTimer,
                   (void *)pdata);

    //wait all threads to finish
    pthread_join(image_capture_t,NULL);
    fprintf(stderr,"Shutting down.");

    //Now that we are done with recording we cancel the timer
    pdata->timer_alive=0;
    pthread_join(timer_t,NULL);


}
