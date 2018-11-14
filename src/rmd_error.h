/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#ifndef RMD_ERROR_H
#define RMD_ERROR_H 1

#include "rmd_types.h"


/*
 * Handling of X errors.
 * Ignores, bad access when registering shortcuts
 * and BadWindow on XQueryTree
 *
 * \param dpy Connection to the X Server
 *
 * \param e XErrorEvent struct containing error info
 *
 * \returns 0 on the two ignored cases, calls exit(1)
 *            otherwise.
 *
 */
int rmdErrorHandler(Display *dpy,XErrorEvent *e);


#endif
