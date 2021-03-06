#ifdef unix
#ifndef unix_h
#define unix_h

#define NO_LOAD_IO
#define NONAG
#define NOSSL

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>
#ifdef USE_ATOMIC_H
#include <asm/atomic.h>
#else
#include <bits/atomicity.h>
#endif

#include "unix_interlock.h"

inline char *strlwr(char * a) {if (a) {char *p; for(p=a;*p;++p) *p=tolower(*p); } return a;}
inline char *strupr(char * a) {if (a) {char *p; for(p=a;*p;++p) *p=toupper(*p); } return a;}

#ifndef BOOL
#define BOOL bool
#endif

#ifndef LPCTSTR
#define LPCTSTR const char *
#endif

#ifndef FALSE
#define FALSE false
#endif

#ifndef TRUE
#define TRUE true
#endif

typedef unsigned long DWORD;

char *itoa(int num, char *buf, int radix);
char *ltoa(long num, char *buf, int radix);
char *ultoa(unsigned long num, char *buf, int radix);
char *strrev(char *buffer);

/* shlwapi compatibility functions */
char *PathCanonicalize(char *dest, char *src);
bool PathMakePretty(char *dummy);
char *PathCombine(char *dest, const char *abs, const char *rel);

// returns result length
int SearchPath(const char *path, const char *fname, const char *ext, int fbuf_size, char *fbuf_result, char **file_component);

// temporary filename
int GetTempPath(int len, char *name);

        
#ifndef _tolower

//	#define _tolower(_c)    ( (_c)-'A'+'a' )
#endif

#ifndef _toupper
//	#define _toupper(_c)    ( (_c)-'a'+'A' )
#endif

#define _rotr(var,bits) (((var)>>bits)|((var)<<((sizeof(var)<<3)-bits)))
#define _rotl(var,bits) (((var)<<bits)|((var)>>((sizeof(var)<<3)-bits)))
#define __iscsymf(_c)   (isalpha(_c) || ((_c) == '_'))
#define __iscsym(_c)    (isalnum(_c) || ((_c) == '_'))


#define _vsnprintf vsnprintf
#define _snprintf snprintf
#define _gcvt gcvt
#define _fcvt fcvt

#define _dup dup
#define _pipe pipe
#define O_BINARY 0
#define _O_BINARY O_BINARY

/* implemented in unix.cpp */
#define _itoa itoa
#define _ltoa ltoa
#define _ultoa ultoa

#define strnicmp strncasecmp
#define stricmp strcasecmp
#define memicmp(a,b,l) strncasecmp((const char *)a,(const char *)b,l)

#define ZeroMemory(m, n) (memset(m, 0, n))

#define Sleep(s) usleep(s*1000)
#define os_sleep(s) usleep(s*1000)

#define OutputDebugStringA(buf) fputs(buf, stderr);

typedef void * HANDLE;
typedef void * HMODULE;
typedef void * HINSTANCE;
#define LoadLibrary(file) dlopen(file, RTLD_LAZY);
#define FreeLibrary(hM) dlclose(hM);
#define GetProcAddress(hM, name) dlsym(hM, name)
#define GetLastError() errno

#define _timezone timezone

#define MAX_PATH 1024

#define STDCALL

#include "qmail_resource.h"

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#endif //ifndef unix_h
#endif //ifdef unix
