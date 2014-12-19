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
#include <math.h>
#include <ifprag.h>

extern  void __fprem( double x, double y, int *quot, double *rem );
#if defined(_M_IX86)
  #pragma aux  __fprem "*_" parm [];
#endif

/*  The fmod function computes the floating-point remainder of x/y.
    It returns x if y is 0, otherwise it returns the value f that has
    the same sign as x, such that x == i*y + f for some integer i,
    where the magnitude of f is less than the magnitude of y.
*/

_WMRTLINK extern double _IF_dfmod( double, double );
#if defined(_M_IX86)
  #pragma aux (if_rtn) _IF_fmod "IF@FMOD";
  #pragma aux (if_rtn) _IF_dfmod "IF@DFMOD";
#endif

_WMRTLINK float _IF_fmod( float x, float y )
/********************************/
    {
        return( _IF_dfmod( x, y ) );
    }

_WMRTLINK double (fmod)( double x, double y )
/*********************************/
    {
        return( _IF_dfmod( x, y ) );
    }

_WMRTLINK double _IF_dfmod( double x, double y )
/***********************************/
    {
        auto int quot;
        auto double rem;

        __fprem( x, y, &quot, &rem );
        return( rem );
    }

