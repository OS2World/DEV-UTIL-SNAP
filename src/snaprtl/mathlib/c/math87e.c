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
#include <stddef.h>
#include <math.h>
#include "mathcode.h"


_WMRTLINK double __math87_err( double x, unsigned char code )
/*************************************************/
    {
        unsigned int  err_code;

        switch( code ) {
        case FUNC_ACOS:
        case FUNC_ASIN:
        case FUNC_SQRT:
            return( __math1err( code | M_DOMAIN | V_ZERO, &x ) );
        case FUNC_EXP:
            if( x < 0.0 ) return( 0.0 );
        }
        err_code = code | M_OVERFLOW | V_HUGEVAL;
        if( code == FUNC_SINH ) {
            if( x < 0.0 )  err_code = code | M_OVERFLOW | V_NEG_HUGEVAL;
        }
        return( __math1err( err_code, &x ) );
    }
