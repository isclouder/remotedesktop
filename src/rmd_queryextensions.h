/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#ifndef QUERYEXTENSIONS_H
#define QUERYEXTENSIONS_H 1

#include "rmd_types.h"


/**
* Check if needed extensions are present
*
* \param dpy Connection to the server
*
* \param args ProgArgs struct containing the user-set options
*
* \param damage_event gets filled with damage event number
*
* \param damage_error gets filled with damage error number
*
* \note Can be an exit point if extensions are not found
*/
void QueryExtensions(Display *dpy,
                     ProgArgs *args,
                     int *damage_event,
                     int *damage_error);


#endif
