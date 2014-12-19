/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "variety.h"
#include "widechar.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

extern  CHAR_TYPE *__CVTBuffer();
extern  char *__cvt( double, int, int *, int *, int, char * );

// LDBL_DIG + decimal point + 'e' + exponent (what about '\0'?)
#define MaxPrec (LDBL_DIG+1+1+3)

static char *fixup( char *p, int n )
{
    char *start = p;
    if( n < MaxPrec && isdigit( *p ) ) {
        while( *p ) {
            p++;
            n--;
        }
        while( n > 0 ) {
            *p++ = '0';
            n--;
        }
        *p = '\0';
    }
    return( start );
}

_WMRTLINK CHAR_TYPE *__F_NAME(ecvt,_wecvt)(double value,int ndigits, int *dec, int *sign )
{
    /* ndigits represents the number of significant digits */
    CHAR_TYPE   *buf;

    buf = __CVTBuffer();
    buf = (CHAR_TYPE *)__cvt( value, ndigits, dec, sign, 'G', (char *)buf );
    return( _AToUni( buf, fixup( (char *)buf, ndigits ) ) );
}

_WMRTLINK CHAR_TYPE *__F_NAME(fcvt,_wfcvt)(double value,int ndigits, int *dec, int *sign )
{
    /* ndigits represents the number of decimal places */
    CHAR_TYPE   *buf;

    buf = __CVTBuffer();
    buf = (CHAR_TYPE *)__cvt( value, ndigits, dec, sign, 'F', (char *)buf );
    return( _AToUni( buf, fixup( (char *)buf, ndigits+(*dec) ) ) );
}
