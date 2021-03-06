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

/* I used this lines for compiling library+example (and debugging)*****/
#include "TuxEip.h"
/**********************************************************************/
/* Use this one instead of the previous if you install TuxEip library */
//#include <tuxeip/TuxEip.h>
/**********************************************************************/
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define space printf("\n")
#define flush(header) _CipFlushBuffer(header,24+header->Length);

int main(int argc,char *argv[])
{
	//cip_debuglevel=LogDebug; // You can uncomment this line to see the data exchange between TuxEip and your Logic controller

	//char *IP="192.168.1.4\0";
	char *IP="192.168.1.115\0";
	BYTE path[]={1,0};
// 	char *IP="10.140.200.58\0";
// 	BYTE path[]={1,0};
	int res;
	int count=8;
	char *var;
	int tns=getpid();
	
	printf("Starting, Pid = %d\n",getpid());

	if (argc<2)
	{
		printf("You have to provide a tag name\n");
		return(1);
	}
	var=argv[1];
			
	/* Openning a session */	
	printf("entering OpenSession \n");

	Eip_Session *session=OpenSession(IP);
	if (session==NULL)
	{
		printf("Error : OpenSession %s (%d:%d)\n",cip_err_msg,cip_errno,cip_ext_errno);
		return(1);
	}
	printf("OpenSession Ok\n");
	printf("entering RegisterSession \n");
	res=RegisterSession(session);

	if (res!=Error)
	{ int i,val_int,val_bool;
	float val_float;
	printf("RegisterSession Ok\n");
	printf("entering ConnectPLCOverCNET\n");
	Eip_Connection *connection=ConnectPLCOverCNET(
	session, // session whe have open
	SLC500, // plc type
	0x12345678, // Target To Originator connection ID
	0x6789, // Connection Serial Number
	5000, // Request Packet Interval
	path, // Path to the ControlLogix
	sizeof(path) // path size
																							 );

	if (connection!=NULL)
	{
		printf("ConnectPLCOverCNET Ok, \n");
			
		printf("Reading %s\n",var);
		//LGX_Read *data=ReadPlcData(session,connection,var,1);
		PLC_Read *data=ReadPLCData(session,connection,NULL,NULL,0,SLC500,tns++,var,count);
		if (data!=NULL)
		{
			if (data->Varcount>0)
			{
				printf("ReadPLCData Ok :\n");
				printf("\telements :%d\n\tDatatype : %d\n\ttotalsize : %d\n\tElement size : %d\n",data->Varcount,data->type,data->totalsize,data->elementsize,data->mask);
				for (i=0;i<data->Varcount;i++)
				{
					val_bool=PCCC_GetValueAsBoolean(data,i);
					val_int=PCCC_GetValueAsInteger(data,i);
					val_float=PCCC_GetValueAsFloat(data,i);
					printf("Value [%d] = %f (%d) (%x)\n",i,val_float,val_int,val_bool);
				}
			} else printf("Error ReadPLCData : %s\n",cip_err_msg);
			free(data); // You have to free the data return by ReadLgxData

                        // pvbrowser begin
                        int pvb_data = 321;
                        if(argc == 3) pvb_data = atoi(argv[2]);
                        int ret;
                        ret = WritePLCData(session,connection,NULL,NULL,0,SLC500,tns++,"N7:7",PLC_INTEGER,&pvb_data,1);
                        printf("WritePLCData ret=%d\n", ret);
                        if(ret == Error) printf("WritePLCData Error\n");
                        else             printf("WritePLCData Success\n");
                        // pvbrowser end
		} else
		{
			printf("Error : ReadPLCData %s (%d:%d)\n",cip_err_msg,cip_errno,cip_ext_errno);
		}
		printf("entering Forward_Close\n");
		res=Forward_Close(connection);
		if (res!=Error)	printf("Forward_Close %s\n",cip_err_msg);
		else printf("Error : Forward_Close %s (%d:%d)\n",cip_err_msg,cip_errno,cip_ext_errno);
	} else
	{
		printf("Error : ConnectPLCOverCNET %s (%d:%d)\n",cip_err_msg,cip_errno,cip_ext_errno);
	}	
	UnRegisterSession(session);	
	printf("UnRegister : %s\n",cip_err_msg);
	}else 
	{
		printf("Error : RegisterSession %s (%d:%d)\n",cip_err_msg,cip_errno,cip_ext_errno);
	}
	printf("entering CloseSession\n");
	CloseSession(session);

	return(0);
}
