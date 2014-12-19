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
#include "pi.h"
#include "ifprag.h"
#include "mathcode.h"
#include "pdiv.h"


_WMRTLINK extern double _IF_dacos( double );
#if defined(_M_IX86)
 #pragma aux (if_rtn) _IF_acos "IF@ACOS";
 #pragma aux (if_rtn) _IF_dacos "IF@DACOS";
#endif

_WMRTLINK float _IF_acos( float x )
/*********************/
    {
        return( _IF_dacos( x ) );
    }

_WMRTLINK double (acos)( double x )
/***********************/
    {
        return( _IF_dacos( x ) );
    }

_WMRTLINK double _IF_dacos( double x )
/*************************/
    {
        auto double     z;

        z =  1.0 - x * x;
        if( z < 0.0 ) {
//            z = _matherr( DOMAIN, "acos", &x, &x, 0.0 );
            z = __math1err( FUNC_ACOS | M_DOMAIN | V_ZERO, &x );
        } else if( z == 0.0 ) {
            if( x < 0.0 ) {
                z = Pi;
            } else {
                z = 0.0;
            }
        } else {
            z = Piby2 - atan( PDIV( x , sqrt( z ) ) );
        }
        return( z );
    }
