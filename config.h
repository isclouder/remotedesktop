/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Define to 1 if the `closedir' function returns void instead of `int'. */
/* #undef CLOSEDIR_VOID */

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `asound' library (-lasound). */
/* #undef HAVE_LIBASOUND */

/* Define to 1 if you have the `ICE' library (-lICE). */
#define HAVE_LIBICE 1

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the `ogg' library (-logg). */
#define HAVE_LIBOGG 1

/* Define to 1 if you have the `pthread' library (-lpthread). */
#define HAVE_LIBPTHREAD 1

/* Define to 1 if you have the `SM' library (-lSM). */
#define HAVE_LIBSM 1

/* Define to 1 if you have the `theora' library (-ltheora). */
#define HAVE_LIBTHEORA 1

/* Define to 1 if you have the `vorbis' library (-lvorbis). */
#define HAVE_LIBVORBIS 1

/* Define to 1 if you have the `vorbisenc' library (-lvorbisenc). */
#define HAVE_LIBVORBISENC 1

/* Define to 1 if you have the `vorbisfile' library (-lvorbisfile). */
#define HAVE_LIBVORBISFILE 1

/* Define to 1 if you have the `X11' library (-lX11). */
#define HAVE_LIBX11 1

/* Define to 1 if you have the `Xdamage' library (-lXdamage). */
#define HAVE_LIBXDAMAGE 1

/* Define to 1 if you have the `Xext' library (-lXext). */
#define HAVE_LIBXEXT 1

/* Define to 1 if you have the `Xfixes' library (-lXfixes). */
#define HAVE_LIBXFIXES 1

/* Define to 1 if you have the `z' library (-lz). */
#define HAVE_LIBZ 1

/* endian.h in $include_path/machine/ subdirectory */
/* #undef HAVE_MACHINE_ENDIAN_H */

/* Define to 1 if your system has a GNU libc compatible `malloc' function, and
   to 0 otherwise. */
#define HAVE_MALLOC 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_DIR_H */

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_NDIR_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if the system has the type `u_int16_t'. */
#define HAVE_U_INT16_T 1

/* Define to 1 if the system has the type `u_int32_t'. */
#define HAVE_U_INT32_T 1

/* Define to 1 if the system has the type `u_int64_t'. */
#define HAVE_U_INT64_T 1

/* Define to 1 if you have the <vorbis/vorbisfile.h> header file. */
#define HAVE_VORBIS_VORBISFILE_H 1

/* Name of package */
#define PACKAGE "remotedesktop"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "remotedesktop"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "remotedesktop 0.3.8.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "remotedesktop"

/* Define to the home page for this package. */
/* #undef PACKAGE_URL */

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.3.8.1"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "0.3.8.1"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* Define to rpl_malloc if the replacement function should be used. */
/* #undef malloc */
