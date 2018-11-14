/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#ifndef RMD_TIMER_H
#define RMD_TIMER_H 1

#include "rmd_types.h"


/**
* Loop ,signal timer cond var,sleep-\ 
*  ^                                 |
*  |________________________________/
*   
*
* \param pdata ProgData struct containing all program data
*/
void *rmdTimer(ProgData *pdata);


#endif
