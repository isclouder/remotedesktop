bin_PROGRAMS = remotedesktop

remotedesktop_SOURCES = \
	rmd.c \
	rmd_start.c \
	rmd_start.h \
	rmd_get_frame.c \
	rmd_get_frame.h \
	rmd_setbrwindow.c \
	rmd_setbrwindow.h \
	rmd_timer.c \
	rmd_timer.h \
	rmd_types.h 

remotedesktop_CPPFLAGS = -D_THREAD_SAFE -pthread -Wall
remotedesktop_LDFLAGS  = @X_LIBS@ @X_EXTRA_LIBS@ @X_PRE_LIBS@

