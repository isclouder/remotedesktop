/******************************************************************************
*                            remoteDesktop                                    *
*******************************************************************************
*                                                                             *
******************************************************************************/

#include "config.h"
#include "rmd_queryextensions.h"
#include "rmd_types.h"

#include <stdlib.h>
#include <X11/extensions/Xdamage.h>


void QueryExtensions(Display *dpy,
                     ProgArgs *args,
                     int *damage_event,
                     int *damage_error){
    if((!(args->full_shots))&&(!XDamageQueryExtension( dpy, damage_event, damage_error))){
        fprintf(stderr,"XDamage extension not found!!!\n"
                       "Try again using the --full-shots option, though\n"
                       "enabling XDamage is highly recommended,\n"
                       "for performance reasons.\n");
        exit(4);
    }
}
