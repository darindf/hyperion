/* HSCUTL.H     (c) Copyright Roger Bowler, 1999-2012                */
/*              Host-specific functions for Hercules                 */
/*                                                                   */
/*   Released under "The Q Public License Version 1"                 */
/*   (http://www.hercules-390.org/herclic.html) as modifications to  */
/*   Hercules.                                                       */

/*********************************************************************/
/* HSCUTL.H   --   Implementation of functions used in hercules that */
/* may be missing on some platform ports, or other convenient mis-   */
/* laneous global utility functions.                                 */
/*********************************************************************/


#ifndef __HSCUTL_H__
#define __HSCUTL_H__

#include "hercules.h"

#ifndef _HSCUTL_C_
#ifndef _HUTIL_DLL_
#define HUT_DLL_IMPORT DLL_IMPORT
#else   /* _HUTIL_DLL_ */
#define HUT_DLL_IMPORT extern
#endif  /* _HUTIL_DLL_ */
#else
#define HUT_DLL_IMPORT DLL_EXPORT
#endif

#ifndef _HSCUTL2_C_
#ifndef _HUTIL_DLL_
#define HU2_DLL_IMPORT DLL_IMPORT
#else   /* _HUTIL_DLL_ */
#define HU2_DLL_IMPORT extern
#endif  /* _HUTIL_DLL_ */
#else
#define HU2_DLL_IMPORT DLL_EXPORT
#endif

#ifndef _HEXDUMPE_C_
#ifndef _HUTIL_DLL_
#define HEXDMP_DLL_IMPORT DLL_IMPORT
#else   /* _HUTIL_DLL_ */
#define HEXDMP_DLL_IMPORT extern
#endif  /* _HUTIL_DLL_ */
#else
#define HEXDMP_DLL_IMPORT DLL_EXPORT
#endif

/*********************************************************************
  The following couple of Hercules 'utility' functions may be defined
  elsewhere depending on which host platform we're being built for...
  For Windows builds (e.g. MingW32), the functionality for the below
  functions is defined in 'w32util.c'. For other host build platforms
  (e.g. Linux, Apple, etc), the functionality for the below functions
  is defined right here in 'hscutil.c'...
 *********************************************************************/

#if defined(_MSVC_)

  /* The w32util.c module will provide the below functionality... */

#else

  /* THIS module (hscutil.c) is to provide the below functionality.. */

  /*
    Returns outpath as a host filesystem compatible filename path.
    This is a Cygwin-to-MSVC transitional period helper function.
    On non-Windows platforms it simply copies inpath to outpath.
    On Windows it converts inpath of the form "/cygdrive/x/foo.bar"
    to outpath in the form "x:/foo.bar" for Windows compatibility.
  */
  char *hostpath( char *outpath, const char *inpath, size_t buffsize );

  /* Poor man's  "fcntl( fd, F_GETFL )"... */
  /* (only returns access-mode flags and not any others) */
  int get_file_accmode_flags( int fd );

  /* Initialize/Deinitialize sockets package... */
  int socket_init( void );
  int socket_deinit( void );

  /* Set socket to blocking or non-blocking mode... */
  int socket_set_blocking_mode( int sfd, int blocking_mode );

  /* Determine whether a file descriptor is a socket or not... */
  /* (returns 1==true if it's a socket, 0==false otherwise)    */
  int socket_is_socket( int sfd );

  /* Set the SO_KEEPALIVE option and timeout values for a
     socket connection to detect when client disconnects.
     Returns 0==success, +1==warning, -1==failure
     (*) Warning failure means function only partially
         succeeded (not all requested values were set)
  */
  int set_socket_keepalive( int sfd, int idle_time, int probe_interval,
                            int probe_count );

  /* Function to retrieve keepalive values. 0==success, -1=failure */
  int get_socket_keepalive( int sfd, int* idle_time, int* probe_interval,
                            int* probe_count );

  /* Determine whether process is running "elevated" or not */
  /* (returns 1==true running elevated, 0==false otherwise) */
  int is_elevated();

#endif // !defined(_MSVC_)

/*********************************************************************/

#if !defined(_MSVC_) && !defined(HAVE_STRERROR_R)
  HUT_DLL_IMPORT void strerror_r_init(void);
  HUT_DLL_IMPORT int  strerror_r(int, char *, size_t);
#endif

#if defined(ENABLE_SYSTEM_SYMBOLS)
  HUT_DLL_IMPORT const char *get_symbol(const char *);
  HUT_DLL_IMPORT char *resolve_symbol_string(const char *);

#if defined(ENABLE_BUILTIN_SYMBOLS)
  HUT_DLL_IMPORT void set_symbol(const char *,const char *);
  HUT_DLL_IMPORT void del_symbol(const char *);
  HUT_DLL_IMPORT void list_all_symbols(void);
#endif /* #if defined( ENABLE_BUILTIN_SYMBOLS ) */

#endif /* #if defined(ENABLE_SYSTEM_SYMBOLS) */

#ifdef _MSVC_

  #ifndef HAVE_ID_T
  #define HAVE_ID_T
    typedef unsigned long id_t;
  #endif

  HU2_DLL_IMPORT int getpriority(int, id_t);
  HU2_DLL_IMPORT int setpriority(int, id_t, int);

  #ifndef HAVE_SYS_RESOURCE_H
    #define  PRIO_PROCESS  0
    #define  PRIO_PGRP     1
    #define  PRIO_USER     2
  #endif

  // Win32 'winbase.h' updates

  //        REALTIME_PRIORITY_CLASS        256      //   -20
  //        HIGH_PRIORITY_CLASS            128      //   -15

  #ifndef   ABOVE_NORMAL_PRIORITY_CLASS
    #define ABOVE_NORMAL_PRIORITY_CLASS  32768      //    -8
  #endif

  //        NORMAL_PRIORITY_CLASS           32      //     0

  #ifndef   BELOW_NORMAL_PRIORITY_CLASS
    #define BELOW_NORMAL_PRIORITY_CLASS  16384      //     8
  #endif
  //        IDLE_PRIORITY_CLASS             64      //    15

#endif // _MSVC_

#if !defined(HAVE_STRLCPY)
/* $OpenBSD: strlcpy.c,v 1.8 2003/06/17 21:56:24 millert Exp $ */
/*
 * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 */
/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
/*  ** NOTE **  'siz' is size of DESTINATION buffer, NOT src!  */
/*  ** NOTE **  returns 'size_t' and NOT 'char*' like strncpy! */
HUT_DLL_IMPORT size_t
strlcpy(char *dst, const char *src, size_t siz);
#endif

#if !defined(HAVE_STRLCAT)
/* $OpenBSD: strlcat.c,v 1.11 2003/06/17 21:56:24 millert Exp $ */
/*
 * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 */
/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz <= strlen(dst)).
 * Returns strlen(src) + MIN(siz, strlen(initial dst)).
 * If retval >= siz, truncation occurred.
 */
/*  ** NOTE **  'siz' is size of DESTINATION buffer (disregarding
                any existing data!), NOT size of src argument! */
/*  ** NOTE **  returns 'size_t' and NOT 'char*' like strncat! */
HUT_DLL_IMPORT size_t
strlcat(char *dst, const char *src, size_t siz);
#endif

/* Subtract/add gettimeofday struct timeval */
HUT_DLL_IMPORT int timeval_subtract (struct timeval *beg_timeval, struct timeval *end_timeval, struct timeval *dif_timeval);
HUT_DLL_IMPORT int timeval_add      (struct timeval *dif_timeval, struct timeval *accum_timeval);

/*
  Easier to use timed_wait_condition that waits for
  the specified relative amount of time without you
  having to build an absolute timeout time yourself
*/
HUT_DLL_IMPORT int timed_wait_condition_relative_usecs
(
    COND*            pCOND,     // ptr to condition to wait on
    LOCK*            pLOCK,     // ptr to controlling lock (must be held!)
    U32              usecs,     // max #of microseconds to wait
    struct timeval*  pTV        // [OPTIONAL] ptr to tod value (may be NULL)
);

/* Read/write to socket functions */
HUT_DLL_IMPORT int hprintf(int s,char *fmt,...) ATTR_PRINTF(2,3);
HUT_DLL_IMPORT int hwrite(int s,const char *,size_t);
HUT_DLL_IMPORT int hgetc(int s);
HUT_DLL_IMPORT char *hgets(char *b,size_t c,int s);


/* Posix 1003.e capabilities */
#if defined(OPTION_CAPABILITIES)
HUT_DLL_IMPORT int drop_privileges(int c);
HUT_DLL_IMPORT int drop_all_caps(void);
#endif

/* inline functions for byte swaps */

static __inline__ U16 swap_byte_U16(U16 s)
{
    return( (U16)bswap_16( (uint16_t) s ) );
}
static __inline__ U32 swap_byte_U32(U32 i)
{
    return( (U32)bswap_32( (uint32_t) i ) );
}
static __inline__ U64 swap_byte_U64(U64 ll )
{
    return( (U64)bswap_64( (uint64_t) ll ) );
}

/* Hercules page-aligned calloc/free */
HUT_DLL_IMPORT  void*  hpcalloc ( BYTE type, size_t size );
HUT_DLL_IMPORT  void   hpcfree  ( BYTE type, void*  ptr  );

/* Hercules low-level file open */
HUT_DLL_IMPORT  int hopen( const char* path, int oflag, ... );

/* Trim path information from __FILE__ macro */
#if defined( _MSVC_ )
HUT_DLL_IMPORT const char* trimloc( const char* loc );
#define  TRIMLOC(_loc)     trimloc( _loc )
#else
#define  TRIMLOC(_loc)            ( _loc )
#endif

/*********************************************************************/
/* Format TIMEVAL to printable value: "YYYY-MM-DD HH:MM:SS.uuuuuu",  */
/* being exactly 26 characters long (27 bytes with null terminator). */
/* pTV points to the TIMEVAL to be formatted. If NULL is passed then */
/* the curent time of day as returned by a call to 'gettimeofday' is */
/* used instead. buf must point to a char work buffer where the time */
/* is formatted into and must not be NULL. bufsz is the size of buf  */
/* and must be >= 2. If successful then the value of buf is returned */
/* and is always zero terminated. If an error occurs or an invalid   */
/* parameter is passed then NULL is returned instead.                */
/*********************************************************************/
HUT_DLL_IMPORT char* FormatTIMEVAL( const TIMEVAL* pTV, char* buf, int bufsz );

/*-------------------------------------------------------------------*/
/* fmt_memsize_rounded:   128K,  64M,  8G,  etc...                   */
/*-------------------------------------------------------------------*/
HUT_DLL_IMPORT char *fmt_memsize_rounded( const U64 memsize, char* buf, const size_t bufsz );

/*-------------------------------------------------------------------*/
/* Standard Utility Initialization                                   */
/*-------------------------------------------------------------------*/
HUT_DLL_IMPORT int initialize_utility( int argc, char* argv[],
                                       char*  defpgm,
                                       char*  desc,
                                       char** pgm );

/*********************************************************************/
/* Dump  storage, usually variables on the stack, but anything goes. */
/* 32 bytes to the line, no translation.                             */
/*********************************************************************/

HUT_DLL_IMPORT
void
dumpStorageHow( void * what, size_t length, char * msg, int reverse);
#define dumpStorage( what, length, msg) dumpStorageHow( what, length, msg, 0 )
#define dumpStorageReversed( what, length, msg) dumpStorageHow( what, length, msg, 1 )

#endif /* __HSCUTL_H__ */
