/****************************************************************************
*
*                          SciTech SNAP Graphics
*
*  ========================================================================
*
*   Copyright (C) 1991-2004 SciTech Software, Inc. All rights reserved.
*
*   This file may be distributed and/or modified under the terms of the
*   GNU General Public License version 2.0 as published by the Free
*   Software Foundation and appearing in the file LICENSE.GPL included
*   in the packaging of this file.
*
*   Licensees holding a valid Commercial License for this product from
*   SciTech Software, Inc. may use this file in accordance with the
*   Commercial License Agreement provided with the Software.
*
*   This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
*   THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
*   PURPOSE.
*
*   See http://www.scitechsoft.com/license/ for information about
*   the licensing options available and how to purchase a Commercial
*   License Agreement.
*
*   Contact license@scitechsoft.com if any conditions of this licensing
*   are not clear to you, or you have questions about licensing options.
*
*  ========================================================================
*
* Language:     ANSI C
* Environment:  MSDOS, Win16, Win32
*
* Description:  Include the correct version number information for
*               the OS we are building for.
*
****************************************************************************/

#if     defined(__LINUX__) || defined(__XFREE86__)
#include "snap/graphics/linux/ver.h"
#elif   defined(__BEOS__)
#include "snap/graphics/beos/ver.h"
#elif   defined(__OS2__)
#include "snap/graphics/os2/ver.h"
#elif   defined(__QNX__)
#include "snap/graphics/qnx/ver.h"
#else
#include "snap/graphics/win9x/ver.h"
#endif

