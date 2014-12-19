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


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %     Copyright (C) 1992, by WATCOM International Inc.  All rights    %
// %     reserved.  No part of this software may be reproduced or        %
// %     used in any form or by any means - graphic, electronic or       %
// %     mechanical, including photocopying, recording, taping or        %
// %     information storage and retrieval systems - except with the     %
// %     written permission of WATCOM International Inc.                 %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Modified    By              Reason
//  ========    ==              ======
//  92/01/30    Steve McDowell  Initial implementation.
//  92/02/28    ...             Modified to delay allocation of buffers
//                              until overflow/underflow called.
//  92/09/08    Greg Bentz      Cleanup.
//  93/07/22    Greg Bentz      Make sure underflow() function sets up the
//                              get area
//  93/10/22    Raymond Tang    Split into separate files.
//  93/11/09    Greg Bentz      Make sure underflow() trashes the put area
//                              in the right order
//  93/11/12    Greg Bentz      Make input one character at a time
//  94/04/06    Greg Bentz      combine header files

#ifdef __SW_FH
#include "iost.h"
#else
#include "variety.h"
#include <unistd.h>
#include <stdio.h>
#include <iostream.h>
#include <stdiobuf.h>
#endif
#include "ioutil.h"
#include "sdfhdr.h"

int stdiobuf::underflow() {
/*************************/
// Handle allocating a buffer, if required.
// Handle filling the get area of the streambuf.
// Read more stuff from the input device.
// Return the first character read.

    char *ptr;
    int   len;

    __lock_it( __b_lock );

    // Flush any output waiting in the buffer:
    if( out_waiting() > 0 ) {
        if( sync() == EOF ) {
            return( EOF );
        }
    }
    // discard put area
    setp( NULL, NULL );

    // Try to allocate a buffer:
    if( base() == NULL ) {
        if( allocate() == EOF ) {
            return( EOF );
        }
        if( base() == NULL ) {
            // unbuffered special case
            ptr = __unbuffered_get_area + DEFAULT_PUTBACK_SIZE;
            setg( __unbuffered_get_area, ptr, ptr );
            int ch;
            ch = fgetc( __file_pointer );
            if( ch == EOF ) {
                return( EOF );
            } else {
                *egptr() = (char)ch;
            }
            setg( eback(), gptr(), egptr() + 1 );
            return( *gptr() );
        } else {
            ptr = base() + DEFAULT_PUTBACK_SIZE;
            setg( base(), ptr, ptr );
        }
    } else if( gptr() >= egptr() ) {
        ptr = base() + DEFAULT_PUTBACK_SIZE;
        setg( base(), ptr, ptr );
    }

    len = (__huge_ptr_int)(ebuf() - egptr());
    if( len > 0 ) {
        len = 1;
        int ch;
        ch = fgetc( __file_pointer );
        if( ch == EOF ) {
            return( EOF );
        } else {
            *egptr() = (char)ch;
        }
    } else {
        len = 0;
    }
    setg( eback(), gptr(), egptr() + len );
    return( gptr() < egptr() ? *gptr() : EOF );
}
