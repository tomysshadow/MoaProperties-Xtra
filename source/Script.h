/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#define _CRT_NO_VA_START_VALIDATION

#ifndef _H_Script
#define _H_Script

#include "shared.h"

#include "moastdif.h"
#include "mmixscrp.h"
#include "driservc.h"
#include "xmmvalue.h"
#include "drivalue.h"




enum ACCESS_PROPERTY {
	ACCESS_PROPERTY_GET = 0,
	ACCESS_PROPERTY_SET = 1
};




// {B1AC0940-7ED9-4767-881A-1C074368FD83}
DEFINE_GUID(CLSID_TStdXtra,
	0xb1ac0940, 0x7ed9, 0x4767, 0x88, 0x1a, 0x1c, 0x7, 0x43, 0x68, 0xfd, 0x83);

EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(TStdXtra)
	PIMoaDrPlayer drPlayerInterfacePointer = NULL;
	PIMoaMmValue mmValueInterfacePointer = NULL;
	PIMoaDrValue drValueInterfacePointer = NULL;
	PIMoaMmList mmListInterfacePointer = NULL;
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(TStdXtra, IMoaRegister)
	EXTERN_DEFINE_METHOD(MoaError, Register, (PIMoaCache, PIMoaXtraEntryDict))
EXTERN_END_DEFINE_CLASS_INTERFACE

EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(TStdXtra, IMoaMmXScript)
	EXTERN_DEFINE_METHOD(MoaError, Call, (PMoaDrCallInfo))
	private:
	EXTERN_DEFINE_METHOD(MoaError, AccessPlayerMoaProperty, (PMoaDrCallInfo, ACCESS_PROPERTY))
	EXTERN_DEFINE_METHOD(MoaError, AccessMovieMoaProperty, (PMoaDrCallInfo, ACCESS_PROPERTY))
	EXTERN_DEFINE_METHOD(MoaError, AccessCastLibMoaProperty, (PMoaDrCallInfo, ACCESS_PROPERTY))
	EXTERN_DEFINE_METHOD(MoaError, AccessMemberMoaProperty, (PMoaDrCallInfo, ACCESS_PROPERTY))
	EXTERN_DEFINE_METHOD(MoaError, AccessFrameMoaProperty, (PMoaDrCallInfo, ACCESS_PROPERTY))
	EXTERN_DEFINE_METHOD(MoaError, AccessSoundMoaProperty, (PMoaDrCallInfo, ACCESS_PROPERTY))
	EXTERN_DEFINE_METHOD(MoaError, AccessSpriteMoaProperty, (PMoaDrCallInfo, ACCESS_PROPERTY))
	EXTERN_DEFINE_METHOD(MoaError, TestValueVoid, (PMoaMmValue, bool*, MoaMmValueType))
	EXTERN_DEFINE_METHOD(MoaError, GetAProp, (ConstPMoaMmValue, ConstPMoaChar, PMoaMmValue))
	EXTERN_DEFINE_METHOD(MoaError, GetOptions, (PMoaDrCallInfo, ACCESS_PROPERTY, MoaLong, PIMoaDrMovie*, PIMoaDrScoreAccess*))
	EXTERN_DEFINE_METHOD(MoaError, GetChannelMoaPropertyArguments, (PMoaDrCallInfo, ACCESS_PROPERTY, PMoaLong, PMoaMmSymbol, PIMoaDrScoreAccess*))
EXTERN_END_DEFINE_CLASS_INTERFACE

#endif