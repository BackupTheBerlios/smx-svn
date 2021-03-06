/* COPYRIGHT 1998 Prime Data Corp. 
	All Rights Reserved.  Use of this file without prior written 
	authorization is strictly prohibited.
*/
#ifdef WIN32
	#include <windows.h>
	#include <winbase.h>
#else
  #include "unix.h"
#endif

#ifndef BUFFER_GROW
#ifdef _DEBUG
#define BUFFER_GROW 256
#else
#define BUFFER_GROW 256
#endif
#endif

#ifndef BUFFER_CHAIN
	#define BUFFER_CHAIN 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "buf_ref.h"


//
// CBufRefChar - C buffer code, malloc/free
//

static int s_growby = BUFFER_GROW;

//#define GETX ((BUFFER_INFO *) m_buf - 1)

#if defined WIN32
bool CBufRefChar::_Free()				{ return DecRef(); }
//	long CBufRefChar::LokRef()			{ return (long) InterlockedCompareExchange(((void**)(&GETX->m_ref)), 0, (void *) 1); }
#else
bool CBufRefChar::_Free()			{ return DecRef(); }
//	long CBufRefChar::LokRef()			{ if (DecRef() == 1) {SetRef(0); return 1;} else return GetRef(); }
#endif

CBufRefChar::CBufRefChar(int n) 
{
	if (n > 0) {
		BUFFER_INFO* pX = (BUFFER_INFO*) malloc(n + sizeof(BUFFER_INFO));

		m_buf = pX->buf();
		m_buf[n] = '\0';
		pX->m_n = pX->m_x = n;

#if BUFFER_CHAIN
		SetRef(1);
#endif
	} else { 
		m_buf = 0;
	}
}

CBufRefChar::~CBufRefChar() 
{
	assert(!m_buf || GetRef() > 0);
	Free();
}

void *CBufRefChar::Grow(int n) 
{
	if (n > 0) {
#if BUFFER_CHAIN
		BUFFER_INFO* pX;

		int z;
		if (!m_buf)
			z = n;
		else
			z = n + max(s_growby, n>>1);

		if (!m_buf) {
			pX = (BUFFER_INFO*) malloc(z + sizeof(BUFFER_INFO));
      if (pX) {
  			m_buf = pX->buf();
  			SetRef(1);
  			SetAlloc( z );
      }
		} else if (GetRef() == 1) {
			if (n > Alloc()) {
				//z = (n << 1) + s_growby;
				pX = (BUFFER_INFO*) realloc(GetX(), z + sizeof(BUFFER_INFO));
				if (pX) {
					m_buf = pX->buf();
					SetAlloc( z );
				}
			}
		} else {
			assert(GetRef() >= 1);
			pX = (BUFFER_INFO*) malloc(z + sizeof(BUFFER_INFO));
			assert(pX != NULL);
			if (pX) {
				memcpy(pX->buf(), m_buf, Count() < n ? Count() : n);
				Free();
				m_buf = pX->buf();
				SetRef(1);
				SetAlloc( z );
			}
		}
		
		//SetAlloc( z );
#else
		m_buf = ((char *) realloc(GetX(), n + Alloc() + s_growby + sizeof(BUFFER_INFO))) + sizeof(BUFFER_INFO);
		SetAlloc(n + Alloc() + s_growby);
#endif
		if (m_buf)
	        SetCount(n);
		return m_buf;
	} else {
		if (m_buf) {
			if (GetRef() == 1) {
				SetCount(0);
			} else {
				Free();
			}
		}
		return m_buf;
	}
}

void CBufRefChar::Free() 
{
	if (m_buf) {
#if BUFFER_CHAIN
		if (_Free()) 
			free(GetX());
#endif
		m_buf = NULL;
	}
}

void CBufRefChar::Change()
{
#if BUFFER_CHAIN
	if (m_buf) {
		BUFFER_INFO* pX = (BUFFER_INFO*) malloc(Alloc() + sizeof(BUFFER_INFO));
		memcpy(pX, GetX(), Count() + sizeof(BUFFER_INFO));
		Free();
		m_buf = pX->buf();
		SetRef(1);
	}
#endif
}

CBufRefChar &CBufRefChar::Copy(const CBufRefChar &newBuf) 
{
#if BUFFER_CHAIN
	if (m_buf != newBuf.m_buf) {
		if (newBuf.m_buf) {
			assert(((CBufRefChar &)newBuf).GetRef() > 0);
			((CBufRefChar &)newBuf).IncRef();
			Free();
			m_buf = newBuf.m_buf;
		} else {
			Free();
		}
	}
#else
	Grow(newBuf.Count());
	memcpy(m_buf, newBuf.m_buf, newBuf.Count());
#endif
	return *this;
}
