/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#ifndef SETBRWINDOW_H
#define SETBRWINDOW_H 1

#include "rmd_types.h"


/**
* Check and align window size
*
* \param dpy Connection to the server
*
* \param brwin BRWindow struct contaning the initial and final window
*
* \param specs DisplaySpecs struct with
*              information about the display to be recorded
*
* \param args ProgArgs struct containing the user-set options
*
* \returns 0 on Success 1 on Failure
*/
int SetBRWindow(Display *dpy,
                BRWindow *brwin,
                DisplaySpecs *specs,
                ProgArgs *args);


#endif
