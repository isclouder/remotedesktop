/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#ifndef INITIALIZE_DATA_H
#define INITIALIZE_DATA_H 1

#include "rmd_types.h"


/**
* initialize lists,mutexes,image buffers, take first screenshot,
* and anything else needed before launching the capture threads.
*
* \param pdata ProgData struct containing all program data
*
* \param enc_data reference to enc_data structure
*
* \param cache_data reference to cache_data structure
*
* \returns 0 on success, other values must cause the program to exit
*/
int InitializeData(ProgData *pdata);

/**
* Sets up the ProgArgs structure to default values.
*/
void SetupDefaultArgs(ProgArgs *args);

/**
*   Currently only frees some memory
*   (y,u,v blocks)
*
*/
void CleanUp(void);


#endif
