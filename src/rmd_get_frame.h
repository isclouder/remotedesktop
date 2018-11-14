/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#ifndef GET_FRAME_H
#define GET_FRAME_H 1

#include "rmd_types.h"


/**
* Retrieve frame form xserver, and transform to a yuv buffer,
* either directly(full shots) or by calling UpdateImage.
* \param pdata ProgData struct containing all program data
*/
int InitializeDisplay(ProgData *pdata);
int FirstFrame(ProgData *pdata, XImage **image);
void *GetFrame(ProgData *pdata);


#endif
