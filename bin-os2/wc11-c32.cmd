@echo off
REM Setup for compiling with Watcom C/C++ 11.0 in 32 bit mode

if .%CHECKED%==.1 goto checked_build
SET LIB=%SCITECH_LIB%\LIB\RELEASE\WIN32\WC11;%WC11_PATH%\LIB386;%WC11_PATH%\LIB386\NT;.
echo Release build enabled.
goto setvars

:checked_build
SET LIB=%SCITECH_LIB%\LIB\DEBUG\WIN32\WC11;%WC11_PATH%\LIB386;%WC11_PATH%\LIB386\NT;.
echo Checked debug build enabled.
goto setvars

:setvars
SET EDPATH=%WC11_PATH%\EDDAT
SET C_INCLUDE=%WC11_PATH%\H;%WC11_PATH%\H\NT
SET INCLUDE=INCLUDE;%SCITECH%\INCLUDE;%PRIVATE%\INCLUDE;%C_INCLUDE%
SET WATCOM=%WC11_PATH%
SET MAKESTARTUP=%SCITECH%\MAKEDEFS\WC32.MK
SET USE_TNT=
SET USE_X32=
SET USE_X32VM=
SET USE_WIN16=
SET USE_WIN32=1
SET USE_WIN386=
SET WIN32_GUI=
SET USE_OS216=
SET USE_OS232=
SET USE_OS2GUI=
SET USE_SNAP=
SET WC_LIBBASE=wc11
SET EDPATH=%WC11_PATH%\EDDAT
PATH %SCITECH%\bin-os2;%WC11_PATH%\binp;%WC11_PATH%\binw;%DEFPATH%%WC_CD_PATH%
SET BEGINLIBPATH=%WC11_PATH%\binp\dll
SET USE_LIB_PATH=%LIB%

REM: Enable Win32 SDK if desired (sdk on command line)
if NOT .%1%==.sdk goto done
call win32sdk.bat

:done
echo Watcom C/C++ 11.0 Win32 console compilation environment set up.
