//
//  iostream.h  I/O streams
//
//                          Open Watcom Project
//
//    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
//
//  ========================================================================
//
//    This file contains Original Code and/or Modifications of Original
//    Code as defined in and that are subject to the Sybase Open Watcom
//    Public License version 1.0 (the 'License'). You may not use this file
//    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
//    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
//    provided with the Original Code and Modifications, and is also
//    available at www.sybase.com/developer/opensource.
//
//    The Original Code and all software distributed under the License are
//    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
//    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
//    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
//    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
//    NON-INFRINGEMENT. Please see the License for the specific language
//    governing rights and limitations under the License.
//
//  ========================================================================
//
#ifndef _IOSTREAM_H_INCLUDED
#define _IOSTREAM_H_INCLUDED
#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif


#ifndef __cplusplus
#error iostream.h is for use with C++
#endif
#ifndef _COMDEF_H_INCLUDED
 #include <_comdef.h>
#endif

#ifndef _WATCOM_EXCEPTION_DEFINED
#define _WATCOM_EXCEPTION_DEFINED
#define _WATCOM_EXCEPTION_DEFINED_
struct __WATCOM_exception {
#if defined(__AXP__) || defined(__PPC__)
    void *__filler;
#endif
};
#endif

// For multi-thread support.
#ifndef __lock_it

class __lock;

#ifdef __SW_BM
class _WPRTLINK __get_lock {
public:
    __get_lock( __lock * );
    ~__get_lock();
private:
    __lock *__lk;
};
#define __lock_it( __l ) __get_lock __lock_name( __LINE__ )( __l )
#define __lock_name( __ln ) __lock_glue( __lock__, __ln )
#define __lock_glue( __pre, __lin ) __pre ## __lin
#else
#define __lock_it( __l )
#endif
#endif

// Define EOF to be the same as that used with C, so that a user mixing
// C and C++ won't get burned.
#if !defined( EOF )
    #define EOF (-1)
#endif

// __NOT_EOF is useful for those functions that return "something other
// than EOF" to indicate that everything is OK.
#define __NOT_EOF 0

// Position in the stream (absolute value, 0 is first byte):
typedef long streampos;

// Offset from current position in the stream:
typedef long streamoff;

enum {
    DEFAULT_PUTBACK_SIZE = 4,
    DEFAULT_MAINBUF_SIZE = 512,
    DEFAULT_BUF_SIZE     = DEFAULT_MAINBUF_SIZE + DEFAULT_PUTBACK_SIZE
};

// These are referred to in class ios, but are defined later, or elsewhere:
class _WPRTLINK __WATCOM_ios;
class _WPRTLINK istream;
class _WPRTLINK ostream;
class _WPRTLINK streambuf;

// **************************** IOS ******************************************
#if defined(_M_IX86)
  #pragma pack(__push,1);
#else
  #pragma pack(__push,8);
#endif
class _WPRTLINK ios {
public:
    enum io_state {                     // Error state
        goodbit = 0x00,                 // - no errors
        badbit  = 0x01,                 // - operation failed, may not proceed
        failbit = 0x02,                 // - operation failed, may proceed
        eofbit  = 0x04                  // - end of file
    };
    typedef int iostate;
    enum open_mode {                    // How to open a stream
        in        = 0x0001,             // - open for input
        out       = 0x0002,             // - open for output
        atend     = 0x0004,             // - seek to end after opening
        append    = 0x0008,             // - open for output, append to the end
        truncate  = 0x0010,             // - discard contents after opening
        nocreate  = 0x0020,             // - open only an existing file
        noreplace = 0x0040,             // - open only a new file
        text      = 0x0080,             // - open as text file
        binary    = 0x0100,             // - open as binary file

        app       = append,             // Historical purposes
        ate       = atend,
        trunc     = truncate
    };
    typedef int openmode;
    enum seek_dir {                     // Seek direction
        beg       = 0x00,               // - seek from beginning
        cur       = 0x01,               // - seek from current position
        end       = 0x02                // - seek from end
    };
    typedef int seekdir;
    enum fmt_flags {                    // Format flags
        skipws     = 0x0001,            // - skip whitespace
        left       = 0x0002,            // - align field to left edge
        right      = 0x0004,            // - align field to right edge
        internal   = 0x0008,            // - sign at left, value at right
        dec        = 0x0010,            // - decimal conversion for integers
        oct        = 0x0020,            // - octal conversion for integers
        hex        = 0x0040,            // - hexadecimal conversion for integers
        showbase   = 0x0080,            // - show dec/octal/hex base on output
        showpoint  = 0x0100,            // - show decimal and digits on output
        uppercase  = 0x0200,            // - use E,X (not e,x) on output numbers
        showpos    = 0x0400,            // - use + for output positive numbers
        scientific = 0x0800,            // - use scientific notation for output
        fixed      = 0x1000,            // - use floating notation for output
        unitbuf    = 0x2000,            // - flush stream after output
        stdio      = 0x4000,            // - flush stdout/stderr after output

        #define _LAST_FORMAT_FLAG 0x00004000
        #define _LAST_FLAG_BIT    0x80000000

        basefield  = dec | oct | hex,
        adjustfield= left | right | internal,
        floatfield = scientific | fixed,
    };
    typedef long fmtflags;

    class failure : public __WATCOM_exception {
        iostate __cause;
    public:
        failure( iostate );             // - set the cause of failure
        iostate cause() const;          // - query the cause of failure
    };

    ios( streambuf *__sb );
    ios( ios const & );
    virtual ~ios();

    ios &operator = ( ios const & );
        operator void * () const;
    int operator !      () const;

    ostream   *tie( ostream *__ostrm );
    ostream   *tie() const;
    streambuf *rdbuf() const;
    iostate    rdstate() const;
    iostate    clear( iostate __state = 0 );
    int        good() const;
    int        bad()  const;
    int        fail() const;
    int        eof()  const;
    iostate    exceptions( iostate __enable );
    iostate    exceptions() const;
    fmtflags   setf( fmtflags __onbits, fmtflags __mask );
    fmtflags   setf( fmtflags __onbits );
    fmtflags   unsetf( fmtflags __offbits );
    fmtflags   flags( fmtflags __bits );
    fmtflags   flags() const;
    char       fill( char __fillchar );
    char       fill() const;
    int        precision( int __precision );
    int        precision() const;
    int        width( int __width );
    int        width() const;
    long      &iword( int __index );
    void     *&pword( int __index );

    static void sync_with_stdio( void ) {};     // obsolete function

    static fmtflags bitalloc();
    static int      xalloc();

    __lock *__i_lock;        // ios data member operations
    static __lock *__x_lock; // xalloc/bitalloc operations

protected:
    ios();

    void init( streambuf *__sb );
    void setstate( ios::iostate __orbits );

private:
    streambuf *__strmbuf;
    ostream   *__tied_stream;
    long       __format_flags;
    int        __error_state;
    int        __enabled_exceptions;
    int        __float_precision;
    int        __field_width;
    void      *__xalloc_list;
    char       __fill_character;

    static int       __xalloc_index;
    static fmtflags  __last_format_flag;

    friend class __WATCOM_ios;
    int : 0;
};
#pragma pack(__pop);

inline streambuf *ios::rdbuf() const {
    return( __strmbuf );
}

inline ios::iostate ios::rdstate() const {
    return( __error_state );
}

inline int ios::good() const {
    return( __error_state == 0 );
}

inline int ios::bad() const {
    return( __error_state & ios::badbit );
}

inline int ios::fail() const {
    return( __error_state & (ios::failbit|ios::badbit) );
}

inline int ios::eof() const {
    return( __error_state & ios::eofbit );
}

inline ios::iostate ios::exceptions() const {
    return( __enabled_exceptions );
}

inline ios::operator void * () const {
    return( (void *) (fail()==0) );
}

inline int ios::operator ! () const {
    return( fail() );
}

inline ios::fmtflags ios::setf( ios::fmtflags __onbits ) {
    __lock_it( __i_lock );
    ios::fmtflags __old_flags = __format_flags;
    __format_flags           |= __onbits;
    return( __old_flags );
}

inline ios::fmtflags ios::setf( ios::fmtflags __onbits, ios::fmtflags __mask ) {
    __lock_it( __i_lock );
    ios::fmtflags __old_flags = __format_flags;
    __format_flags           &= ~__mask;
    __format_flags           |= __onbits & __mask;
    return( __old_flags );
}

inline ios::fmtflags ios::unsetf( ios::fmtflags __offbits ) {
    __lock_it( __i_lock );
    ios::fmtflags __old_flags = __format_flags;
    __format_flags           &= ~__offbits;
    return( __old_flags );
}

inline ios::fmtflags ios::flags( ios::fmtflags __flags ) {
    __lock_it( __i_lock );
    ios::fmtflags __old_flags = __format_flags;
    __format_flags            = __flags;
    return( __old_flags );
}

inline ios::fmtflags ios::flags() const {
    return( __format_flags );
}

inline char ios::fill( char __fillchar ) {
    __lock_it( __i_lock );
    char __old_fill  = __fill_character;
    __fill_character = __fillchar;
    return( __old_fill );
}

inline char ios::fill() const {
    return( __fill_character );
}

inline int ios::precision( int __precision ) {
    __lock_it( __i_lock );
    int __old_precision = __float_precision;
    __float_precision   = __precision;
    return( __old_precision );
}

inline int ios::precision() const {
    return( __float_precision );
}

inline int ios::width( int __width ) {
    __lock_it( __i_lock );
    int __old_width = __field_width;
    __field_width   = __width;
    return( __old_width );
}

inline int ios::width() const {
    return( __field_width );
}

inline ostream *ios::tie( ostream *__ostrm ) {
    __lock_it( __i_lock );
    ostream *__old_tie = __tied_stream;
    __tied_stream      = __ostrm;
    return( __old_tie );
}

inline ostream *ios::tie() const {
    return( __tied_stream );
}

// **************************** STREAMBUF ************************************
// For compatibility with other vendors who include "streambuf" in IOSTREAM.H
#ifndef _STREAMBUF_H_INCLUDED
 #include <streambu.h>
#endif

// **************************** ISTREAM **************************************
#if defined(_M_IX86)
  #pragma pack(__push,1);
#else
  #pragma pack(__push,8);
#endif
class _WPRTLINK istream : virtual public ios {
public:
    istream( streambuf *__sb );
    istream( istream const &__istrm );
    virtual ~istream();

    istream &operator = ( streambuf * __sb );
    istream &operator = ( istream const &__istrm );
    istream &operator >> (             char * __buf );
    istream &operator >> (      signed char * __buf );
    istream &operator >> (    unsigned char * __buf );
    istream &operator >> (             char & __c );
    istream &operator >> (      signed char & __c );
    istream &operator >> (    unsigned char & __c );
    istream &operator >> (     signed short & __i );
    istream &operator >> (   unsigned short & __i );
    istream &operator >> (       signed int & __i );
    istream &operator >> (     unsigned int & __i );
    istream &operator >> (      signed long & __i );
    istream &operator >> (    unsigned long & __i );
    istream &operator >> (   signed __int64 & __i );
    istream &operator >> ( unsigned __int64 & __i );
    istream &operator >> (            float & __f );
    istream &operator >> (           double & __f );
    istream &operator >> (      long double & __f );
    istream &operator >> (        streambuf * __sb );
    istream &operator >> ( ios &(*__f)( ios & ) );
    istream &operator >> ( istream &(*__f)( istream & ) );

    int        ipfx( int __noskipws = 0 );
    void       isfx();
    int        get();
    istream   &get(          char *__buf, int __len, char __delim = '\n' );
    istream   &get(   signed char *__buf, int __len, char __delim = '\n' );
    istream   &get( unsigned char *__buf, int __len, char __delim = '\n' );
    istream   &get(          char &__c );
    istream   &get(   signed char &__c );
    istream   &get( unsigned char &__c );
    istream   &get( streambuf &__sb, char __delim = '\n' );
    istream   &getline(          char *__buf, int __len, char __delim = '\n' );
    istream   &getline(   signed char *__buf, int __len, char __delim = '\n' );
    istream   &getline( unsigned char *__buf, int __len, char __delim = '\n' );
    istream   &ignore( int __num = 1, int __delim = EOF );
    istream   &read(          char *__buf, int __len );
    istream   &read(   signed char *__buf, int __len );
    istream   &read( unsigned char *__buf, int __len );
    istream   &seekg( streampos __position );
    istream   &seekg( streamoff __offset, ios::seekdir __direction );
    istream   &putback( char __c );
    streampos  tellg();
    int        gcount() const;
    int        peek();
    int        sync();

protected:
    istream();
    void     eatwhite();
    istream &do_get( char &__c );
    istream &do_rshift( char &__c );
    istream &do_read( char *__buf, int __len );
    int      ipfx0( void );
    int      ipfx1( void );
    int      do_ipfx( int __noskipws );

private:
    int __last_read_length;
};
#pragma pack(__pop);

inline istream &istream::operator >> ( signed char *__buf ) {
    return( *this >> (char *) __buf );
}

inline istream &istream::operator >> ( unsigned char *__buf ) {
    return( *this >> (char *) __buf );
}

#ifdef __BIG_INLINE__
inline istream &istream::operator >> ( char &__c ) {
    __lock_it( __i_lock );
    if( ipfx0() ) {
        if( rdbuf()->in_avail() ) {
            __c = (char)(rdbuf()->sgetchar());
        } else {
            do_rshift( __c );
        }
        isfx();
    }
    return( *this );
}
#endif

inline istream &istream::operator >> ( signed char &__c ) {
    return( *this >> (char &) __c );
}

inline istream &istream::operator >> ( unsigned char &__c ) {
    return( *this >> (char &) __c );
}

inline istream &istream::get( signed char *__buf, int __len, char __delim ) {
    return( get( (char *)__buf, __len, __delim ) );
}

inline istream &istream::get( unsigned char *__buf, int __len, char __delim ) {
    return( get( (char *)__buf, __len, __delim ) );
}

#ifdef __BIG_INLINE__
inline istream &istream::get( char &__c ) {
    __lock_it( __i_lock );
    if( ipfx1() ) {
        if( rdbuf()->in_avail() ) {
            __c = (char)(rdbuf()->sgetchar());
        } else {
            do_get( __c );
        }
        isfx();
    } else {
        __last_read_length = 0;
    }
    return( *this );
}
#endif

inline istream &istream::get( signed char &__c ) {
    return( get( (char &) __c ) );
}

inline istream &istream::get( unsigned char &__c ) {
    return( get( (char &) __c ) );
}

inline istream &istream::getline( signed char *__buf, int __len,
    char __delim ) {
    return( getline( (char *)__buf, __len, __delim ) );
}

inline istream &istream::getline( unsigned char *__buf, int __len,
    char __delim ) {
    return( getline( (char *)__buf, __len, __delim ) );
}

#ifdef __BIG_INLINE__
inline istream &istream::read( char *__buf, int __len ) {
    __lock_it( __i_lock );
    if( ipfx1() ) {
        if( rdbuf()->in_avail() > __len ) {
            __last_read_length = rdbuf()->sgetn( __buf, __len );
        } else {
            do_read( __buf , __len );
        }
        isfx();
    } else {
        __last_read_length = 0;
    }
    return( *this );
}
#endif

inline istream &istream::read( signed char *__buf, int __len ) {
    return( read( (char *) __buf, __len ) );
}

inline istream &istream::read( unsigned char *__buf, int __len ) {
    return( read( (char *) __buf, __len ) );
}

inline int istream::ipfx0( void ) {
    __lock_it( __i_lock );
    return( ((flags()&ios::skipws) || !good() || tie()) ? do_ipfx( 0 ) : 1);
}

inline int istream::ipfx1( void ) {
    __lock_it( __i_lock );
    return( (!good() || tie()) ? do_ipfx( 1 ) : 1);
}

inline void istream::isfx() {
}

inline int istream::gcount() const {
    return( __last_read_length );
}

// **************************** OSTREAM **************************************
#if defined(_M_IX86)
  #pragma pack(__push,1);
#else
  #pragma pack(__push,8);
#endif
class _WPRTLINK ostream : virtual public ios {
public:
    ostream( streambuf *__sb );
    ostream( ostream const &__ostrm );
    virtual ~ostream();

    ostream &operator = ( streambuf *__sb );
    ostream &operator = ( ostream const &__ostrm );
    ostream &operator << (                char  __c );
    ostream &operator << (         signed char  __c );
    ostream &operator << (       unsigned char  __c );
    ostream &operator << (        signed short  __s );
    ostream &operator << (      unsigned short  __s );
    ostream &operator << (          signed int  __i );
    ostream &operator << (        unsigned int  __i );
    ostream &operator << (         signed long  __l );
    ostream &operator << (       unsigned long  __l );
    ostream &operator << (      signed __int64  __l );
    ostream &operator << (    unsigned __int64  __l );
    ostream &operator << (               float  __f );
    ostream &operator << (              double  __f );
    ostream &operator << (         long double  __f );
    ostream &operator << (                void *__p );
    ostream &operator << (           streambuf *__sb );
    ostream &operator << (          char const *__buf );
    ostream &operator << (   signed char const *__buf );
    ostream &operator << ( unsigned char const *__buf );
    ostream &operator << ( ostream &(*__f)( ostream & ) );
    ostream &operator << ( ios &(*__f)( ios & ) );

    int        opfx();
    void       osfx();
    ostream   &put(          char __c );
    ostream   &put(   signed char __c );
    ostream   &put( unsigned char __c );
    ostream   &write(          char const *__buf, int __len );
    ostream   &write(   signed char const *__buf, int __len );
    ostream   &write( unsigned char const *__buf, int __len );
    ostream   &flush();
    ostream   &seekp( streampos __position );
    ostream   &seekp( streamoff __offset, ios::seekdir __direction );
    streampos  tellp();

protected:
    ostream();
    ostream &__outfloat( long double const & );
    ostream &do_lshift( char __c);
    int      do_opfx();
};
#pragma pack(__pop);

#ifdef __BIG_INLINE__
inline ostream &ostream::operator << ( char __c ) {
    __lock_it( __i_lock );
    if( opfx() ) {
        if( width() == 0 ) {
            if( rdbuf()->sputc( __c ) == EOF ) {
                setstate( ios::failbit );
            }
        } else {
            do_lshift( __c );
        }
        osfx();
    }
    return( *this );
}
#endif

inline ostream &ostream::operator << ( signed char __c ) {
    return( *this << (char) __c );
}

inline ostream &ostream::operator << ( unsigned char __c ) {
    return( *this << (char) __c );
}

inline ostream &ostream::operator << ( signed short __s ) {
    return( *this << (signed long) __s );
}

inline ostream &ostream::operator << ( unsigned short __s ) {
    return( *this << (unsigned long) __s );
}

inline ostream &ostream::operator << ( signed int __i ) {
    return( *this << (signed long) __i );
}

inline ostream &ostream::operator << ( unsigned int __i ) {
    return( *this << (unsigned long) __i );
}

inline ostream &ostream::operator << ( float __f ) {
    return( __outfloat( (long double)__f ) );
}

inline ostream &ostream::operator << ( double __f ) {
    return( __outfloat( (long double)__f ) );
}

inline ostream &ostream::operator << ( long double __f ) {
    return( __outfloat( __f ) );
}

inline ostream &ostream::operator << ( signed char const *__buf ) {
    return( *this << (char const *) __buf );
}

inline ostream &ostream::operator << ( unsigned char const *__buf ) {
    return( *this << (char const *) __buf );
}

#ifdef __BIG_INLINE__
inline ostream &ostream::put( char __c ) {
    __lock_it( __i_lock );
    if( opfx() ) {
        if( rdbuf()->sputc( __c ) == EOF ) {
            setstate( ios::failbit );
        }
        osfx();
    }
    return( *this );
}
#endif

inline ostream &ostream::put( signed char __c ) {
    return( put( (char) __c ) );
}

inline ostream &ostream::put( unsigned char __c ) {
    return( put( (char) __c ) );
}

#ifdef __BIG_INLINE__
inline ostream &ostream::write( char const *__buf, int __len ) {
    __lock_it( __i_lock );
    if( opfx() ) {
        if( __len  ) {
            if( rdbuf()->sputn( __buf, __len ) != __len ) {
                setstate( ios::failbit );
            }
        }
        osfx();
    }
    return( *this );
}
#endif

inline ostream &ostream::write( signed char const *__buf, int __len ) {
    return( write( (char const *) __buf, __len ) );
}

inline ostream &ostream::write( unsigned char const *__buf, int __len ) {
    return( write( (char const *) __buf, __len ) );
}

inline int ostream::opfx() {
    __lock_it( __i_lock );
    if( !good() ) {
        return 0;
    } else if( tie() || ( flags() & ios::stdio ) ) {
        return do_opfx();
    } else {
        return 1;
    }
}

inline void ostream::osfx() {
    __lock_it( __i_lock );
    if( flags() & ios::unitbuf ) {
        flush();
    }
}

// **************************** IOSTREAM *************************************
#if defined(_M_IX86)
  #pragma pack(__push,1);
#else
  #pragma pack(__push,8);
#endif
class _WPRTLINK iostream : public istream, public ostream {
public:
    iostream( streambuf *__sb );
    iostream( ios const &__strm );
    virtual ~iostream();

    iostream & operator = ( streambuf *__sb );
    iostream & operator = ( ios const &__strm );

protected:
    iostream();
};
#pragma pack(__pop);

// **************************** MANIPULATORS *********************************
_WPRTLINK extern ios &dec( ios & );
_WPRTLINK extern ios &hex( ios & );
_WPRTLINK extern ios &oct( ios & );
_WPRTLINK extern istream &   ws( istream & );
_WPRTLINK extern ostream & endl( ostream & );
_WPRTLINK extern ostream & ends( ostream & );
_WPRTLINK extern ostream &flush( ostream & );

// **************************** PREDEFINED STREAMS ***************************
 _WPRTLINK extern istream _WCDATA cin;
 _WPRTLINK extern ostream _WCDATA cout;
 _WPRTLINK extern ostream _WCDATA cerr;
 _WPRTLINK extern ostream _WCDATA clog;

#endif
