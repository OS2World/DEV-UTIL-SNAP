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
#include "libsupp.h"
#include <stddef.h>
#include <math.h>
#include <limits.h>
#include "ifprag.h"
#include "mathcode.h"

extern  int     __sgn(double);


_WMRTLINK extern double _IF_dpow( double, double );
#if defined(_M_IX86)
  #pragma aux (if_rtn) _IF_pow "IF@POW";
  #pragma aux (if_rtn) _IF_dpow "IF@DPOW";
#endif

_WMRTLINK float _IF_pow( float x, float y )
/********************************/
    {
        return( _IF_dpow( x, y ) );
    }

_WMRTLINK double (pow)( double x, double y )
/********************************/
    {
        return( _IF_dpow( x, y ) );
    }

_WMRTLINK double _IF_dpow( double x, double y )
/**********************************/
{
        unsigned int    err_code;
        register int    sign;
        double          z;
        double          fraction;
        long            exponent;

    sign = __sgn( x );
    if( sign == 0 ) {
        if( y < 0.0 ) {
//            x = _matherr( DOMAIN, "pow", &x, &y, HUGE_VAL );
            x = __math2err( FUNC_POW | M_DOMAIN | V_HUGEVAL, &x, &y );
        } else if( y == 0.0 ) {
//            x = _matherr( DOMAIN, "pow", &x, &y, 1.0 );
            x = __math2err( FUNC_POW | M_DOMAIN | V_ONE, &x, &y );
        }
    } else {
        fraction = modf( y, &z );
        exponent = z;
        if( sign < 0 ) {
            if( fraction != 0.0 ) {     /* negative ** fraction */
//                return( _matherr( DOMAIN, "pow", &x, &y, 0.0 ) );
                return( __math2err( FUNC_POW | M_DOMAIN | V_ZERO, &x, &y ) );
            }
            x = -x;
            if( (exponent & 1) == 0 ) {         /* 01-mar-89 */
                sign = 0;                       /* if # is even, change sign */
            }
        }
        z = log( x ) * y;
        if( fabs( z ) > 709.782712893384 ) {/* if argument is too large */
            if( z < 0.0 ) {                 /* - if argument < 0 */
                /* FPStatus = FPS_UNDERFLOW;   - - underflow */
                __set_ERANGE();
                x = 0.0;                    /* - - set result to 0 */
            } else {                        /* - else */
//                z = (sign > 0) ? HUGE_VAL : - HUGE_VAL;
//                x = _matherr( OVERFLOW, "pow", &x, &y, z );
                if( sign > 0 ) {
                    err_code = FUNC_POW | M_OVERFLOW | V_HUGEVAL;
                } else {
                    err_code = FUNC_POW | M_OVERFLOW | V_NEG_HUGEVAL;
                }
                x = __math2err( err_code, &x, &y );
            }
        } else {
            if( fraction == 0.0 && exponent != LONG_MIN ) {  /* x ** integer */
                if( exponent < 0 ) {
                    exponent = - exponent;
                    x = 1 / x;
                }
                if( exponent != 1 ) {
                    z = 1.0;
                    while( exponent != 0 ) {
                        if( exponent & 1 )  z *= x;
                        x *= x;
                        exponent >>= 1;
                    }
                    x = z;
                }
            } else {
                x = exp( z );
            }
            if( sign < 0 ) {
                x = -x;
            }
        }
    }
    return( x );
}
