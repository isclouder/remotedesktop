/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#ifndef RMDTHREADS_H
#define RMDTHREADS_H 1

#include "rmd_types.h"


/**
* Launch and wait capture threads.
* Also creates and waits the encoding threads when
* encode-on-the-fly is enabled.
*
* \param pdata ProgData struct containing all program data
*/
void rmdThreads(ProgData *pdata);


#endif
