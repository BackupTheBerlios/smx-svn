/* COPYRIGHT 1998 Prime Data Corp. 
	All Rights Reserved.  Duplication of this file without prior written 
	authorization is strictly prohibited.
*/
#ifndef _EX_H_
#define _EX_H_

#ifndef _INC_STDIO
 #ifndef _STDIO_H
	#include <stdio.h>
 #endif
#endif

#ifndef _QLIB_H_
	#include "qlib.h"
#endif

#include "res.h"

#define QEX_PREFIX "ERROR #%d: "
#define QEX_MAXMSG 2048

struct CEx {
	int  id;
	char msg[QEX_MAXMSG + 1];
};

CEx *qExV(int id, char *msg, va_list vargs);
CEx *qEx(int id, char *msg, ...);
CEx *qExF(int id, char *msg, ...);
CEx *qExRc(int id, ...);
CEx *qExRcF(int id, ...);
CEx *qExLast();

#ifndef TLS
	#define TLS __declspec(thread)
#endif

extern TLS CEx gEx;
extern char    gExPrefix[];
extern int     gExPrefixLen;

#endif //#ifndef _EX_H_

/* CEx : simple exception info:

	try {
		... 
		throw(qEx(4, "Error on port #%d. %y", port, GetLastError())) ;
		...
	} catch (CEx *ex) {
	... 
	}

	
	your catch handler then gets an id 
		and a nicely formatted message for printing

	the format string is standard printf 
	... with the addition of a "%y" code
		which takes a GetLastError() code and translates it to a 
		error description
	
	
	qExV is for building new wrapper Ex functions

	rcEx assumes the id is a resource-string-id and uses it 
		to grab a message from the resource fork
		windows resources, however, aren't very portable

	rcExF adds "ERROR %d : " to the fromt of the message

*/
