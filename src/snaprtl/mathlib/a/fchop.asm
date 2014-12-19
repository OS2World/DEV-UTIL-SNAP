;*****************************************************************************
;*
;*                            Open Watcom Project
;*
;*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
;*
;*  ========================================================================
;*
;*    This file contains Original Code and/or Modifications of Original
;*    Code as defined in and that are subject to the Sybase Open Watcom
;*    Public License version 1.0 (the 'License'). You may not use this file
;*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
;*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
;*    provided with the Original Code and Modifications, and is also
;*    available at www.sybase.com/developer/opensource.
;*
;*    The Original Code and all software distributed under the License are
;*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
;*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
;*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
;*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
;*    NON-INFRINGEMENT. Please see the License for the specific language
;*    governing rights and limitations under the License.
;*
;*  ========================================================================
;*
;* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
;*               DESCRIBE IT HERE!
;*
;*****************************************************************************


;
;  Name:        CHOP
;  Operation:   chop float in ST(0) towards 0
;  Inputs:      ST(0)   float
;  Outputs:     ST(0)   chopped float, condition codes unmodified
;  Volatile:    none
;
ifdef __386__
 .386
 .387
else
 .8087
endif
include mdef.inc
include math387.inc

        xref            __8087  ; indicate that NDP instructions are present

        modstart fchop

        xdefp   __CHP

        defp    __CHP
ifdef __386__
        push    EAX                     ; allocate temporary
        fstcw   word ptr [ESP]          ; get 8087 control word
        fwait                           ; wait until store complete
        push    dword ptr [ESP]         ; remember old control word
        mov     byte ptr 1[ESP],1Fh     ; set control word to truncate
        fldcw   word ptr [ESP]          ; load new control word
        frndint                         ; truncate top of stack
        fldcw   word ptr 4[ESP]         ; restore old control word
        fwait                           ; wait until load complete
        lea     ESP,8[ESP]              ; remove temporaries
else
        push    _AX                     ; save a register
        push    _BP                     ; allocate 2 bytes on stack
        mov     _BP,_SP                 ; get access to stack
        push    _AX                     ; allocate temporary
        fstcw   -2[_BP]                 ; get 8087 control word
        fwait                           ; wait until store complete
        mov     AX,-2[_BP]              ; remember old control word
        mov     byte ptr -1[_BP],1Fh    ; set control word to truncate
        fldcw   -2[_BP]                 ; load new control word
        frndint                         ; truncate top of stack
        mov     -2[_BP],AX              ; restore old 8087 control word
        fldcw   -2[_BP]                 ; ...
        fwait                           ; wait until load complete
        mov     _SP,_BP                 ; remove temporary
        pop     _BP                     ; restore BP
        pop     _AX                     ; restore AX
endif
        ret                             ; return
        endproc __CHP

        endmod
        end