#ifndef __GLOB_DEFS_H__
#define __GLOB_DEFS_H__
//application depended define
#define	PRIVATE_PROFILE_NAME	"mint.ini"
#define ENVFILE_EXTENT			"mnt"
#define NORMAL_NAME				"•W€Ý’è"
#define CWinApplication CTheApp
#define CWinDocument CTheDoc
#define CWinView CTheView

////////////////////////////////////////////////////////////////////////////
// static defines
#define szEOF	"[EOF]"
#define DIVMOD(a,b)	(((a)/(b)) + ((((a)%(b))>0) ? 1:0))
#define SIZEMAC(size)		(size==0 ? 0:size-1)
typedef unsigned long OBJOFF;	//file size
typedef unsigned long OBJSIZE;	//file size
typedef unsigned int DSP_Y;
typedef unsigned int DSP_X;
typedef unsigned int LGC_Y;
typedef unsigned int LGC_X;

#define NPOS	((unsigned int)-1)		//maximum index - this value is used flg of special meens;
#define null NULL
#include "util.h"
#include "chara.h"
#include "version.h"
#endif//__GLOB_DEFS_H__
