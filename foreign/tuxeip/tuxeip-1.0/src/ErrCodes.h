/***************************************************************************
 *   Copyright (C) 2006 by TuxPLC					                                 *
 *   Author Stephane JEANNE s.jeanne@tuxplc.net                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _ERRCODES_H
#define _ERRCODES_H

#ifdef __cplusplus
extern "C"
{
#endif
	
#ifndef VALGRING /* Define this to use with valgrind */
	#define THREAD_VAR __thread
#else
	#define THREAD_VAR
#endif

#define MAX_ERR_MSG_LEN 255
#define MAX_CDE_LEN 20

/********** Internal codes ******************/
#define Success 0
//#define Error -1
#define EX_Error -1
#define E_SeeErrorno -2
#define E_PendingBufferFull -3
#define E_NoMoreData -4
#define E_TimeOut -5
#define E_ConnectionFailed -6
#define E_UnsolicitedMsg -7
#define E_NothingToSend -8
#define E_NoReply -9
#define E_ItemUnknow -10

#define E_InvalidReply -20
#define E_OutOfRange -21
#define E_UnsupportedService -22
//#define E_RW	-23
#define E_UnsupportedDataType -24
		
#define E_InternalMemory -30
#define E_BufferTooSmall -31

#define E_SendTimeOut -40
#define E_RcvTimeOut -41
#define E_SendError -42
#define E_RcvError -43

#define E_MR	-100
#define E_PLC -101
#define E_LGX -102


/******************** AB error codes *********************/

typedef enum _Error_type{Internal_Error,Sys_Error,EIP_Error,MR_Error,CM_Error,AB_Error,PCCC_Error} Error_type;
typedef enum _LogLevel{LogNone,LogError,LogTrace,LogDebug} LogLevel;

/*typedef struct _Error{
											Error_type type;
											unsigned int errorcode;
											unsigned int extended;
											char description[100];
											}Error;*/

extern THREAD_VAR int _cip_debuglevel;
extern THREAD_VAR unsigned int _cip_errno;
extern THREAD_VAR unsigned int _cip_ext_errno;
extern THREAD_VAR Error_type _cip_err_type;
extern THREAD_VAR char _cip_err_msg[MAX_ERR_MSG_LEN+1];

void FlushCipBuffer(int level,void *buffer,int size);
void LogCip(int level,char *format,...);

char *CIPGetInternalErrMsg(unsigned int ErrorCode);
char *CIPGetEipErrMsg(unsigned int ErrorCode);
char *CIPGetMRErrMsg(unsigned int ErrorCode,unsigned int Ext_ErrorCode);
char *CIPGetABErrMsg(unsigned int ErrorCode,unsigned int Ext_ErrorCode);
char *CIPGetPCCCErrMsg(unsigned int ErrorCode,unsigned int Ext_ErrorCode);

char *CIPGetErrMsg(Error_type s_err_type,unsigned int s_errno,unsigned int Ext_ErrorCode);

#define CIPERROR(type,no,ext_no) {_cip_err_type=type;_cip_errno=no;_cip_ext_errno=ext_no;memcpy(&_cip_err_msg,CIPGetErrMsg(_cip_err_type,_cip_errno,_cip_ext_errno),MAX_ERR_MSG_LEN);}
#define cip_debuglevel _cip_debuglevel
#define cip_errno _cip_errno
#define cip_ext_errno _cip_ext_errno
#define cip_err_type _cip_err_type
#define cip_err_msg _cip_err_msg

#ifdef __cplusplus
}
#endif

#endif /* _ERRCODES_H */
