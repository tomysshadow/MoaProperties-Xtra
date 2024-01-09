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

#define INITGUID 1

#include "script.h"

#include "moastdif.h"
#include "mmixscrp.h"
#include "driservc.h"
#include "xmmvalue.h"
#include "drivalue.h"
#include "xclassver.h"
#include "moatry.h"




/* Begin Xtra */
/*******************************************************************************
* SCRIPTING XTRA MESSAGE TABLE DESCRIPTION.
*
* the general format is:
* xtra <nameOfXtra>
* new object me [ args ... ]
* <otherHandlerDefintions>
* --
* the first line must give the name of the Scripting xtra.
* the remaining lines give the names of the handlers that this xtra implements
* along with the required types of the arguments for each handler.
*
* -- Pre-defined handler new
* the new handler will be called when a child object is created,
* the first argument is always the child object and you defined any remaining arguments.
* the new handler is the place to initialize any memory required by the child object.
*
* -- Simple Handler Definitions
* Each handler definition line is format like this:
* <handlerName> <argType1> <argName1>, <argType2> <argName2> ...
* the first word is the handler name. Following this are types description for
* the argument to the handler, each separated by a comma.
* the argument name <argName>, may be omited.
* Permited argument types are:
* 	integer
* 	float
* 	string
* 	symbol
* 	object
* 	any
* 	*
* For integer, float, string, symbol, and object, the type of the argument must
* match. the type any means allow any type. the asterisk (*) means any number and
* any type of arguments.
*
* the first argument is the child object and is always declared to be of type object.
*
* -- Global Handlers
* An asterisk (*) preceeding the handler name signifies a global handler.
* This handler is at the global scope level and can be called from any
* movie.
*
* -- Xtra level handlers
* A plus (+) preceeding the handler name signifies an Xtra level handler.
* This kind of handler can be called directly from the Xtra reference,
* without creating a child object.
*
* the enumerated list that follows must correspond directly with the msgTable
* (i.e. they must be in the same order).
*
*******************************************************************************/

/* This is the list of handlers for the Xtra. The versionInfo string is combined
/*	with the msgTable string in the Register method to create a single string that
/* is used when registering the Xtra as a Scripting Xtra. */

static char versionInfo[] = "xtra MoaProperties -- version %s.%s.%s\n";
static char msgTable[] = {
	"new object me\n" /* standard first handler entry in all message tables */
	"-- Get MOA Properties\n"
	"* getPlayerMoaProperty symbol name -- gets a MOA Property of the Player\n"
	"* getMovieMoaProperty symbol name, * options -- gets a MOA Property of a Movie\n"
	"* getCastLibMoaProperty object castLib, symbol name, * options -- gets a MOA Property of a Cast Library\n"
	"* getMemberMoaProperty object member, symbol name, * options -- gets a MOA Property of a Member\n"
	"* getFrameMoaProperty symbol name, * options -- gets a MOA Property of a Frame\n"
	"* getSoundMoaProperty integer channel, symbol name, * options -- gets a MOA Property of a Sound\n"
	"* getSpriteMoaProperty integer channel, symbol name, * options -- gets a MOA Property of a Sprite\n"
	"-- Set MOA Properties\n"
	"* setPlayerMoaProperty symbol name, any value -- sets a MOA Property of the Player\n"
	"* setMovieMoaProperty symbol name, any value, * options -- sets a MOA Property of a Movie\n"
	"* setCastLibMoaProperty object castLib, symbol name, any value, * options -- sets a MOA Property of a Cast Library\n"
	"* setMemberMoaProperty object member, symbol name, any value, * options -- sets a MOA Property of a Member\n"
	"* setFrameMoaProperty symbol name, any value, * options -- sets a MOA Property of a Frame\n"
	"* setSoundMoaProperty integer channel, symbol name, any value, * options -- sets a MOA Property of a Sound\n"
	"* setSpriteMoaProperty integer channel, symbol name, any value, * options -- sets a MOA Property of a Sprite"
};

enum {
	m_new = 0, /* standard first entry */
	m_getPlayerMoaProperty,
	m_getMovieMoaProperty,
	m_getCastLibMoaProperty,
	m_getMemberMoaProperty,
	m_getFrameMoaProperty,
	m_getSoundMoaProperty,
	m_getSpriteMoaProperty,
	m_setPlayerMoaProperty,
	m_setMovieMoaProperty,
	m_setCastLibMoaProperty,
	m_setMemberMoaProperty,
	m_setFrameMoaProperty,
	m_setSoundMoaProperty,
	m_setSpriteMoaProperty,
	m_XXXX /* standard last entry */
};




// this is where the structure of the Xtra is defined
// this says that this is a Standard Xtra and
// that said Standard Xtra class provides the
// interfaces for IMoaRegister and IMoaMmXScript, as well as
// says which version of the XDK we're using
#define XTRA_VERSION_NUMBER XTRA_CLASS_VERSION

BEGIN_XTRA
BEGIN_XTRA_DEFINES_CLASS(TStdXtra, XTRA_CLASS_VERSION)
CLASS_DEFINES_INTERFACE(TStdXtra, IMoaRegister, XTRA_VERSION_NUMBER)
CLASS_DEFINES_INTERFACE(TStdXtra, IMoaMmXScript, XTRA_VERSION_NUMBER)
END_XTRA_DEFINES_CLASS
END_XTRA

// when the Standard Xtra is Created, we Query for Interfaces which
// are provided by Director - similar to how Director will
// query this Standard Xtra for the IMoaRegister and IMoaMmXScript interfaces
// that it provides to Director for its use
STDMETHODIMP_(MoaError) MoaCreate_TStdXtra(TStdXtra* This) {
	moa_try

	ThrowNull(This);

	ThrowErr(This->pCallback->QueryInterface(&IID_IMoaDrPlayer, (PPMoaVoid)&This->drPlayerInterfacePointer));
	ThrowNull(This->drPlayerInterfacePointer);

	ThrowErr(This->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid)&This->mmValueInterfacePointer));
	ThrowNull(This->mmValueInterfacePointer);

	ThrowErr(This->pCallback->QueryInterface(&IID_IMoaDrValue, (PPMoaVoid)&This->drValueInterfacePointer));
	ThrowNull(This->drValueInterfacePointer);

	ThrowErr(This->pCallback->QueryInterface(&IID_IMoaMmList, (PPMoaVoid)&This->mmListInterfacePointer));
	ThrowNull(This->mmListInterfacePointer);

	moa_catch
	moa_catch_end
	moa_try_end
}

// when the Standard Xtra is Destroyed, it'll Release the
// interfaces we Queried for from Director before
STDMETHODIMP_(void) MoaDestroy_TStdXtra(TStdXtra* This) {
	moa_try

	ThrowNull(This);

	if (This->drPlayerInterfacePointer) {
		This->drPlayerInterfacePointer->Release();
		This->drPlayerInterfacePointer = NULL;
	}

	if (This->mmValueInterfacePointer) {
		This->mmValueInterfacePointer->Release();
		This->mmValueInterfacePointer = NULL;
	}

	if (This->drValueInterfacePointer) {
		This->drValueInterfacePointer->Release();
		This->drValueInterfacePointer = NULL;
	}

	if (This->mmListInterfacePointer) {
		This->mmListInterfacePointer->Release();
		This->mmListInterfacePointer = NULL;
	}

	moa_catch
	moa_catch_end
	// we use moa_try_end_void instead of moa_try_end here since this method returns a void type
	moa_try_end_void
}




// this says that the Constructor/Deconstructor for
// the IMoaRegister interface provided by
// the Standard Xtra class will not do anything
STD_INTERFACE_CREATE_DESTROY(TStdXtra, IMoaRegister)

// this allows Director to
// Query and Release the IMoaRegister interface provided
// by the Standard Xtra class
BEGIN_DEFINE_CLASS_INTERFACE(TStdXtra, IMoaRegister)
END_DEFINE_CLASS_INTERFACE

STDMETHODIMP TStdXtra_IMoaRegister::Register(PIMoaCache cacheInterfacePointer, PIMoaXtraEntryDict xtraEntryDictInterfacePointer) {
	PMoaVoid memoryStringPointer = NULL;

	moa_try

	ThrowNull(cacheInterfacePointer);
	ThrowNull(xtraEntryDictInterfacePointer);

	{
		// this interface should NOT be released
		PIMoaRegistryEntryDict registryEntryDictInterfacePointer = NULL;
		ThrowErr(cacheInterfacePointer->AddRegistryEntry(xtraEntryDictInterfacePointer, &CLSID_TStdXtra, &IID_IMoaMmXScript, &registryEntryDictInterfacePointer));
		ThrowNull(registryEntryDictInterfacePointer);

		const char* VER_MAJORVERSION_STRING = "1";
		const char* VER_MINORVERSION_STRING = "0";
		const char* VER_BUGFIXVERSION_STRING = "3";

		const size_t VERSION_STRING_SIZE = min(256, kMoaMmMaxXtraMessageTable);
		char versionString[VERSION_STRING_SIZE] = "";

		if (sprintf_s(versionString, VERSION_STRING_SIZE, versionInfo, VER_MAJORVERSION_STRING, VER_MINORVERSION_STRING, VER_BUGFIXVERSION_STRING) == -1) {
			Throw(kMoaErr_OutOfMem);
		}

		MoaUlong versionStringSize = stringSizeMax(versionString, VERSION_STRING_SIZE);
		MoaUlong memoryStringSize = versionStringSize + strnlen_s(msgTable, kMoaMmMaxXtraMessageTable - versionStringSize);

		memoryStringPointer = pObj->pCalloc->NRAlloc(memoryStringSize);
		ThrowNull(memoryStringPointer);

		if (strncpy_s((char*)memoryStringPointer, memoryStringSize, versionString, versionStringSize)) {
			Throw(kMoaErr_OutOfMem);
		}

		if (strcat_s((char*)memoryStringPointer, memoryStringSize, msgTable)) {
			Throw(kMoaErr_OutOfMem);
		}

		ThrowErr(registryEntryDictInterfacePointer->Put(kMoaDrDictType_MessageTable, memoryStringPointer, memoryStringSize, kMoaDrDictKey_MessageTable));
	}

	moa_catch
	moa_catch_end

	// always do this, whether there is an error or not
	if (memoryStringPointer) {
		pObj->pCalloc->NRFree(memoryStringPointer);
		memoryStringPointer = NULL;
	}

	moa_try_end
}




// this says that the Constructor/Deconstructor for
// the IMoaMmXScript interface provided by
// the Standard Xtra class will not do anything
STD_INTERFACE_CREATE_DESTROY(TStdXtra, IMoaMmXScript)

// this allows Director to
// Query and Release the IMoaMmXScript interface provided
// by the Standard Xtra class
BEGIN_DEFINE_CLASS_INTERFACE(TStdXtra, IMoaMmXScript)
END_DEFINE_CLASS_INTERFACE

// the IMoaMmXScript interface is expected
// to implement a method called Call which
// will be called by Director when
// a Lingo Script attempts to call
// one of the handlers
// defined in the Method Table which
// was registered in the Register method
// implemented by the IMoaRegister interface
// Director will call this method and
// pass in the argument for it, which is
// a pointer with data about the call, including
// which handler was called and the arguments passed
// to the handler from the Lingo Script that attempted
// to call it
STDMETHODIMP TStdXtra_IMoaMmXScript::Call(PMoaDrCallInfo callPtr) {
	moa_try

	ThrowNull(callPtr);

	// switch statement with all the handlers that may be called - we arrive here from Lingo first
	switch (callPtr->methodSelector) {
		case m_getPlayerMoaProperty:
		ThrowErr(AccessPlayerMoaProperty(callPtr, ACCESS_PROPERTY_GET));
		break;
		case m_getMovieMoaProperty:
		ThrowErr(AccessMovieMoaProperty(callPtr, ACCESS_PROPERTY_GET));
		break;
		case m_getCastLibMoaProperty:
		ThrowErr(AccessCastLibMoaProperty(callPtr, ACCESS_PROPERTY_GET));
		break;
		case m_getMemberMoaProperty:
		ThrowErr(AccessMemberMoaProperty(callPtr, ACCESS_PROPERTY_GET));
		break;
		case m_getFrameMoaProperty:
		ThrowErr(AccessFrameMoaProperty(callPtr, ACCESS_PROPERTY_GET));
		break;
		case m_getSoundMoaProperty:
		ThrowErr(AccessSoundMoaProperty(callPtr, ACCESS_PROPERTY_GET));
		break;
		case m_getSpriteMoaProperty:
		ThrowErr(AccessSpriteMoaProperty(callPtr, ACCESS_PROPERTY_GET));
		break;
		case m_setPlayerMoaProperty:
		ThrowErr(AccessPlayerMoaProperty(callPtr, ACCESS_PROPERTY_SET));
		break;
		case m_setMovieMoaProperty:
		ThrowErr(AccessMovieMoaProperty(callPtr, ACCESS_PROPERTY_SET));
		break;
		case m_setCastLibMoaProperty:
		ThrowErr(AccessCastLibMoaProperty(callPtr, ACCESS_PROPERTY_SET));
		break;
		case m_setMemberMoaProperty:
		ThrowErr(AccessMemberMoaProperty(callPtr, ACCESS_PROPERTY_SET));
		break;
		case m_setFrameMoaProperty:
		ThrowErr(AccessFrameMoaProperty(callPtr, ACCESS_PROPERTY_SET));
		break;
		case m_setSoundMoaProperty:
		ThrowErr(AccessSoundMoaProperty(callPtr, ACCESS_PROPERTY_SET));
		break;
		case m_setSpriteMoaProperty:
		ThrowErr(AccessSpriteMoaProperty(callPtr, ACCESS_PROPERTY_SET));
	}

	moa_catch
	moa_catch_end
	moa_try_end
}




// these methods are called from the Call method
// implemented by the IMoaMmXScript interface
// and correspond to handlers in Lingo upon
// the Method Table being registered by
// the Register method implemented by
// the IMoaRegister interface
MoaError TStdXtra_IMoaMmXScript::AccessPlayerMoaProperty(PMoaDrCallInfo callPtr, ACCESS_PROPERTY accessProperty) {
	MoaMmValue argumentValue = kVoidMoaMmValueInitializer;

	moa_try

	ThrowNull(callPtr);

	AccessArgByIndex(1, &argumentValue);

	MoaMmSymbol nameSymbol = 0;
	ThrowErr(pObj->mmValueInterfacePointer->ValueToSymbol(&argumentValue, &nameSymbol));

	switch (accessProperty) {
		case ACCESS_PROPERTY_GET:
		ThrowErr(pObj->drPlayerInterfacePointer->GetProp(nameSymbol, &callPtr->resultValue));
		break;
		case ACCESS_PROPERTY_SET:
		AccessArgByIndex(2, &argumentValue);

		ThrowErr(pObj->drPlayerInterfacePointer->SetProp(nameSymbol, &argumentValue));
	}

	moa_catch
	moa_catch_end
	moa_try_end
}

MoaError TStdXtra_IMoaMmXScript::AccessMovieMoaProperty(PMoaDrCallInfo callPtr, ACCESS_PROPERTY accessProperty) {
	PIMoaDrMovie drMovieInterfacePointer = NULL;

	MoaMmValue argumentValue = kVoidMoaMmValueInitializer;

	moa_try

	ThrowNull(callPtr);

	AccessArgByIndex(1, &argumentValue);

	MoaMmSymbol nameSymbol = 0;
	ThrowErr(pObj->mmValueInterfacePointer->ValueToSymbol(&argumentValue, &nameSymbol));

	ThrowErr(GetOptions(callPtr, accessProperty, 2, &drMovieInterfacePointer, NULL));
	ThrowNull(drMovieInterfacePointer);

	switch (accessProperty) {
		case ACCESS_PROPERTY_GET:
		ThrowErr(drMovieInterfacePointer->GetProp(nameSymbol, &callPtr->resultValue));
		break;
		case ACCESS_PROPERTY_SET:
		AccessArgByIndex(2, &argumentValue);

		ThrowErr(drMovieInterfacePointer->SetProp(nameSymbol, &argumentValue));
	}

	moa_catch
	moa_catch_end

	if (drMovieInterfacePointer) {
		drMovieInterfacePointer->Release();
		drMovieInterfacePointer = NULL;
	}

	moa_try_end
}

MoaError TStdXtra_IMoaMmXScript::AccessCastLibMoaProperty(PMoaDrCallInfo callPtr, ACCESS_PROPERTY accessProperty) {
	PIMoaDrMovie drMovieInterfacePointer = NULL;
	PIMoaDrCast drCastInterfacePointer = NULL;

	MoaMmValue argumentValue = kVoidMoaMmValueInitializer;
	MoaDrCastLibRef castLibRef = {};

	moa_try

	ThrowNull(callPtr);

	AccessArgByIndex(1, &argumentValue);

	ThrowErr(pObj->drValueInterfacePointer->ValueToCastLibRef(&argumentValue, &castLibRef));

	AccessArgByIndex(2, &argumentValue);

	MoaMmSymbol nameSymbol = 0;
	ThrowErr(pObj->mmValueInterfacePointer->ValueToSymbol(&argumentValue, &nameSymbol));

	ThrowErr(GetOptions(callPtr, accessProperty, 3, &drMovieInterfacePointer, NULL));
	ThrowNull(drMovieInterfacePointer);

	// a castLibRef by itself does not reference a particular movie, so
	// we need to pass it through the movie interface to get the actual cast interface
	ThrowErr(drMovieInterfacePointer->GetNthCast(CastLibRef_GetCastLibIndex(&castLibRef), &drCastInterfacePointer));
	ThrowNull(drCastInterfacePointer);

	switch (accessProperty) {
		case ACCESS_PROPERTY_GET:
		ThrowErr(drCastInterfacePointer->GetProp(nameSymbol, &callPtr->resultValue));
		break;
		case ACCESS_PROPERTY_SET:
		AccessArgByIndex(3, &argumentValue);

		ThrowErr(drCastInterfacePointer->SetProp(nameSymbol, &argumentValue));
	}

	moa_catch
	moa_catch_end

	if (drMovieInterfacePointer) {
		drMovieInterfacePointer->Release();
		drMovieInterfacePointer = NULL;
	}

	if (drCastInterfacePointer) {
		drCastInterfacePointer->Release();
		drCastInterfacePointer = NULL;
	}

	moa_try_end
}

MoaError TStdXtra_IMoaMmXScript::AccessMemberMoaProperty(PMoaDrCallInfo callPtr, ACCESS_PROPERTY accessProperty) {
	PIMoaDrMovie drMovieInterfacePointer = NULL;
	PIMoaDrCastMem drCastMemInterfacePointer = NULL;

	MoaMmValue argumentValue = kVoidMoaMmValueInitializer;
	MoaDrCMRef cmRef = {};

	moa_try

	ThrowNull(callPtr);

	AccessArgByIndex(1, &argumentValue);

	ThrowErr(pObj->drValueInterfacePointer->ValueToCMRef(&argumentValue, &cmRef));

	AccessArgByIndex(2, &argumentValue);

	MoaMmSymbol nameSymbol = 0;
	ThrowErr(pObj->mmValueInterfacePointer->ValueToSymbol(&argumentValue, &nameSymbol));

	ThrowErr(GetOptions(callPtr, accessProperty, 3, &drMovieInterfacePointer, NULL));
	ThrowNull(drMovieInterfacePointer);

	ThrowErr(drMovieInterfacePointer->GetCastMemFromCMRef(&cmRef, &drCastMemInterfacePointer));
	ThrowNull(drCastMemInterfacePointer);

	switch (accessProperty) {
		case ACCESS_PROPERTY_GET:
		ThrowErr(drCastMemInterfacePointer->GetProp(nameSymbol, &callPtr->resultValue));
		break;
		case ACCESS_PROPERTY_SET:
		AccessArgByIndex(3, &argumentValue);

		ThrowErr(drCastMemInterfacePointer->SetProp(nameSymbol, &argumentValue));
	}

	moa_catch
	moa_catch_end

	if (drMovieInterfacePointer) {
		drMovieInterfacePointer->Release();
		drMovieInterfacePointer = NULL;
	}

	if (drCastMemInterfacePointer) {
		drCastMemInterfacePointer->Release();
		drCastMemInterfacePointer = NULL;
	}

	moa_try_end
}

MoaError TStdXtra_IMoaMmXScript::AccessFrameMoaProperty(PMoaDrCallInfo callPtr, ACCESS_PROPERTY accessProperty) {
	PIMoaDrScoreAccess drScoreAccessInterfacePointer = NULL;
	PIMoaDrScoreFrame drScoreFrameInterfacePointer = NULL;

	MoaMmValue argumentValue = kVoidMoaMmValueInitializer;

	moa_try

	ThrowNull(callPtr);

	AccessArgByIndex(1, &argumentValue);

	MoaMmSymbol nameSymbol = 0;
	ThrowErr(pObj->mmValueInterfacePointer->ValueToSymbol(&argumentValue, &nameSymbol));

	ThrowErr(GetOptions(callPtr, accessProperty, 2, NULL, &drScoreAccessInterfacePointer));
	ThrowNull(drScoreAccessInterfacePointer);

	ThrowErr(drScoreAccessInterfacePointer->GetFrame(&drScoreFrameInterfacePointer));

	switch (accessProperty) {
		case ACCESS_PROPERTY_GET:
		ThrowErr(drScoreFrameInterfacePointer->GetProp(nameSymbol, &callPtr->resultValue));
		break;
		case ACCESS_PROPERTY_SET:
		// we need to be in update mode to set frame properties
		// (otherwise we get kMoaErr_NotInUpdateMode when calling SetProp)
		ThrowErr(drScoreAccessInterfacePointer->BeginUpdate());

		AccessArgByIndex(2, &argumentValue);

		ThrowErr(drScoreFrameInterfacePointer->SetProp(nameSymbol, &argumentValue));
		ThrowErr(drScoreAccessInterfacePointer->UpdateFrame());

		ThrowErr(drScoreAccessInterfacePointer->EndUpdate());
		ThrowErr(drScoreAccessInterfacePointer->Commit());
	}

	moa_catch
	moa_catch_end

	if (drScoreAccessInterfacePointer) {
		drScoreAccessInterfacePointer->Release();
		drScoreAccessInterfacePointer = NULL;
	}

	if (drScoreFrameInterfacePointer) {
		drScoreFrameInterfacePointer->Release();
		drScoreFrameInterfacePointer = NULL;
	}

	moa_try_end
}

MoaError TStdXtra_IMoaMmXScript::AccessSoundMoaProperty(PMoaDrCallInfo callPtr, ACCESS_PROPERTY accessProperty) {
	PIMoaDrScoreAccess drScoreAccessInterfacePointer = NULL;
	PIMoaDrScoreSound drScoreSoundInterfacePointer = NULL;

	moa_try

	ThrowNull(callPtr);

	// unlike the frame, which is an option and otherwise defaults to the current frame
	// the channel must be specified explicitly
	MoaLong channelIndex = 0;
	MoaMmSymbol nameSymbol = 0;
	ThrowErr(GetChannelMoaPropertyArguments(callPtr, accessProperty, &channelIndex, &nameSymbol, &drScoreAccessInterfacePointer));
	ThrowNull(drScoreAccessInterfacePointer);

	ThrowErr(drScoreAccessInterfacePointer->GetSound(channelIndex, &drScoreSoundInterfacePointer));

	switch (accessProperty) {
		case ACCESS_PROPERTY_GET:
		ThrowErr(drScoreSoundInterfacePointer->GetProp(nameSymbol, &callPtr->resultValue));
		break;
		case ACCESS_PROPERTY_SET:
		ThrowErr(drScoreAccessInterfacePointer->BeginUpdate());

		MoaMmValue argumentValue = kVoidMoaMmValueInitializer;
		AccessArgByIndex(3, &argumentValue);

		ThrowErr(drScoreSoundInterfacePointer->SetProp(nameSymbol, &argumentValue));
		ThrowErr(drScoreAccessInterfacePointer->UpdateFrame());

		ThrowErr(drScoreAccessInterfacePointer->EndUpdate());
		ThrowErr(drScoreAccessInterfacePointer->Commit());
	}

	moa_catch
	moa_catch_end

	if (drScoreAccessInterfacePointer) {
		drScoreAccessInterfacePointer->Release();
		drScoreAccessInterfacePointer = NULL;
	}

	if (drScoreSoundInterfacePointer) {
		drScoreSoundInterfacePointer->Release();
		drScoreSoundInterfacePointer = NULL;
	}

	moa_try_end
}

MoaError TStdXtra_IMoaMmXScript::AccessSpriteMoaProperty(PMoaDrCallInfo callPtr, ACCESS_PROPERTY accessProperty) {
	PIMoaDrScoreAccess drScoreAccessInterfacePointer = NULL;
	PIMoaDrScoreSprite drScoreSpriteInterfacePointer = NULL;

	moa_try

	ThrowNull(callPtr);

	MoaLong channelIndex = 0;
	MoaMmSymbol nameSymbol = 0;
	ThrowErr(GetChannelMoaPropertyArguments(callPtr, accessProperty, &channelIndex, &nameSymbol, &drScoreAccessInterfacePointer));
	ThrowNull(drScoreAccessInterfacePointer);

	ThrowErr(drScoreAccessInterfacePointer->GetSprite(channelIndex, &drScoreSpriteInterfacePointer));

	switch (accessProperty) {
		case ACCESS_PROPERTY_GET:
		ThrowErr(drScoreSpriteInterfacePointer->GetProp(nameSymbol, &callPtr->resultValue));
		break;
		case ACCESS_PROPERTY_SET:
		ThrowErr(drScoreAccessInterfacePointer->BeginUpdate());

		MoaMmValue argumentValue = kVoidMoaMmValueInitializer;
		AccessArgByIndex(3, &argumentValue);

		ThrowErr(drScoreSpriteInterfacePointer->SetProp(nameSymbol, &argumentValue));
		ThrowErr(drScoreAccessInterfacePointer->UpdateFrame());

		ThrowErr(drScoreAccessInterfacePointer->EndUpdate());
		ThrowErr(drScoreAccessInterfacePointer->Commit());
	}

	moa_catch
	moa_catch_end

	if (drScoreAccessInterfacePointer) {
		drScoreAccessInterfacePointer->Release();
		drScoreAccessInterfacePointer = NULL;
	}

	if (drScoreSpriteInterfacePointer) {
		drScoreSpriteInterfacePointer->Release();
		drScoreSpriteInterfacePointer = NULL;
	}

	moa_try_end
}

MoaError TStdXtra_IMoaMmXScript::TestValueVoid(PMoaMmValue testValuePointer, bool* testValueVoidPointer, MoaMmValueType testValueType) {
	moa_try

	ThrowNull(testValuePointer);
	ThrowNull(testValueVoidPointer);

	MoaMmValueType valueType = kMoaMmValueType_Void;
	ThrowErr(pObj->mmValueInterfacePointer->ValueType(testValuePointer, &valueType));

	bool &testValueVoid = *testValueVoidPointer;
	testValueVoid = valueType == kMoaMmValueType_Void;

	if (!testValueVoid && testValueType != kMoaMmValueType_Void && valueType != testValueType) {
		Throw(kMoaMmErr_ValueTypeMismatch);
	}

	moa_catch

	if (testValueVoidPointer) {
		*testValueVoidPointer = true;
	}

	moa_catch_end
	moa_try_end
}

MoaError TStdXtra_IMoaMmXScript::GetAProp(ConstPMoaMmValue propertyListValuePointer, ConstPMoaChar propertyNameStringPointer, PMoaMmValue propertyValuePointer) {
	MoaMmValue propertyNameValue = kVoidMoaMmValueInitializer;

	moa_try

	ThrowNull(propertyListValuePointer);
	ThrowNull(propertyNameStringPointer);
	ThrowNull(propertyValuePointer);

	*propertyValuePointer = kVoidMoaMmValueInitializer;

	MoaMmSymbol propertyNameSymbol = 0;
	ThrowErr(pObj->mmValueInterfacePointer->StringToSymbol(propertyNameStringPointer, &propertyNameSymbol));

	ThrowErr(pObj->mmValueInterfacePointer->SymbolToValue(propertyNameSymbol, &propertyNameValue));

	// if the value isn't found we don't throw, instead the property value is void
	MoaError err = pObj->mmListInterfacePointer->GetValueByProperty(propertyListValuePointer, &propertyNameValue, propertyValuePointer);

	if (err != kMoaDrErr_HandlerNotDefined) {
		ThrowErr(err);
	}

	moa_catch
	moa_catch_end

	pObj->mmValueInterfacePointer->ValueRelease(&propertyNameValue);

	moa_try_end
}

MoaError TStdXtra_IMoaMmXScript::GetOptions(PMoaDrCallInfo callPtr, ACCESS_PROPERTY accessProperty, MoaLong optionsArgumentIndex,
	PIMoaDrMovie* drMovieInterfacePointerPointer,
	PIMoaDrScoreAccess* drScoreAccessInterfacePointerPointer) {
	PIMoaDrMovie drMovieInterfacePointer = NULL;
	PIMoaDrCastMem filmLoopCastMemInterfacePointer = NULL;

	MoaMmValue argumentValue = kVoidMoaMmValueInitializer;
	MoaMmValue movieValue = kVoidMoaMmValueInitializer;
	MoaMmValue filmLoopValue = kVoidMoaMmValueInitializer;
	MoaMmValue frameValue = kVoidMoaMmValueInitializer;

	moa_try

	ThrowNull(callPtr);

	// one or the other can be NULL, but not both
	if (!drMovieInterfacePointerPointer && !drScoreAccessInterfacePointerPointer) {
		Throw(kMoaErr_OutOfMem);
	}

	if (drMovieInterfacePointerPointer) {
		*drMovieInterfacePointerPointer = NULL;
	}

	if (drScoreAccessInterfacePointerPointer) {
		*drScoreAccessInterfacePointerPointer = NULL;
	}

	bool optionsValueVoid = true;

	optionsArgumentIndex += accessProperty;

	// if an optional argument was specified
	if (callPtr->nargs >= optionsArgumentIndex) {
		// if the optional argument is a property list
		AccessArgByIndex(optionsArgumentIndex, &argumentValue);
		ThrowErr(TestValueVoid(&argumentValue, &optionsValueVoid, kMoaMmValueType_PropList));
	}

	bool movieValueVoid = true;

	if (!optionsValueVoid) {
		// first, try and get the #movie property from the options property list
		// and test it's an integer
		ThrowErr(GetAProp(&argumentValue, "Movie", &movieValue));
		ThrowErr(TestValueVoid(&movieValue, &movieValueVoid, kMoaMmValueType_Void));
	}

	if (movieValueVoid) {
		// if the #movie property is void, use the active movie
		ThrowErr(pObj->drPlayerInterfacePointer->GetActiveMovie(&drMovieInterfacePointer));
	} else {
		// if the #movie property isn't void, use the #movie property from the options property list
		MoaLong movieIndex = 0;
		ThrowErr(pObj->mmValueInterfacePointer->ValueToInteger(&movieValue, &movieIndex));

		ThrowErr(pObj->drPlayerInterfacePointer->GetNthMovie(movieIndex, &drMovieInterfacePointer));
	}

	ThrowNull(drMovieInterfacePointer);

	if (drScoreAccessInterfacePointerPointer) {
		bool filmLoopValueVoid = true;

		if (!optionsValueVoid) {
			// first, try and get the #filmLoop property from the options property list
			// and test it's a member
			ThrowErr(GetAProp(&argumentValue, "FilmLoop", &filmLoopValue));
			ThrowErr(TestValueVoid(&filmLoopValue, &filmLoopValueVoid, kMoaMmValueType_Void));
		}

		PIMoaDrScoreAccess drScoreAccessInterfacePointer = NULL;

		if (filmLoopValueVoid) {
			// if the #filmLoop property is void, use the score
			ThrowErr(drMovieInterfacePointer->GetScoreAccess(&drScoreAccessInterfacePointer));
		} else {
			// if the #filmLoop property isn't void, use the #filmLoop property from the options property list
			MoaDrCMRef filmLoopCMRef = {};
			ThrowErr(pObj->drValueInterfacePointer->ValueToCMRef(&filmLoopValue, &filmLoopCMRef));
			ThrowErr(drMovieInterfacePointer->GetCastMemFromCMRef(&filmLoopCMRef, &filmLoopCastMemInterfacePointer));
			ThrowNull(filmLoopCastMemInterfacePointer);

			ThrowErr(filmLoopCastMemInterfacePointer->GetScoreAccess(&drScoreAccessInterfacePointer));
		}

		ThrowNull(drScoreAccessInterfacePointer);

		bool frameValueVoid = true;

		if (!optionsValueVoid) {
			// first, try and get the #frame property from the options property list
			// and test it's an integer
			ThrowErr(GetAProp(&argumentValue, "Frame", &frameValue));
			ThrowErr(TestValueVoid(&frameValue, &frameValueVoid, kMoaMmValueType_Void));
		}

		MoaLong frameIndex = 0;

		if (frameValueVoid) {
			// if the #frame property is void
			if (filmLoopValueVoid) {
				// for the score, default to the frame property of the movie
				MoaMmSymbol frameSymbol = 0;
				ThrowErr(pObj->mmValueInterfacePointer->StringToSymbol("Frame", &frameSymbol));

				ThrowErr(drMovieInterfacePointer->GetProp(frameSymbol, &frameValue));

				ThrowErr(pObj->mmValueInterfacePointer->ValueToInteger(&frameValue, &frameIndex));
			} else {
				// for film loops, default to one
				frameIndex = 1;
			}
		} else {
			// if the #frame property isn't void, use the #frame property from the options property list
			ThrowErr(pObj->mmValueInterfacePointer->ValueToInteger(&frameValue, &frameIndex));
		}

		// the frame is not set by default, so we must always explicitly specify it
		ThrowErr(drScoreAccessInterfacePointer->SetCurFrameIndex(frameIndex));

		*drScoreAccessInterfacePointerPointer = drScoreAccessInterfacePointer;
	}

	moa_catch
	moa_catch_end

	if (drMovieInterfacePointerPointer) {
		*drMovieInterfacePointerPointer = drMovieInterfacePointer;
	} else {
		drMovieInterfacePointer->Release();
		drMovieInterfacePointer = NULL;
	}

	if (filmLoopCastMemInterfacePointer) {
		filmLoopCastMemInterfacePointer->Release();
		filmLoopCastMemInterfacePointer = NULL;
	}

	pObj->mmValueInterfacePointer->ValueRelease(&movieValue);
	pObj->mmValueInterfacePointer->ValueRelease(&filmLoopValue);
	pObj->mmValueInterfacePointer->ValueRelease(&frameValue);

	moa_try_end
}

MoaError TStdXtra_IMoaMmXScript::GetChannelMoaPropertyArguments(PMoaDrCallInfo callPtr, ACCESS_PROPERTY accessProperty, PMoaLong channelIndexPointer, PMoaMmSymbol nameSymbolPointer, PIMoaDrScoreAccess* drScoreAccessInterfacePointerPointer) {
	MoaMmValue argumentValue = kVoidMoaMmValueInitializer;

	moa_try

	ThrowNull(callPtr);
	ThrowNull(channelIndexPointer);
	ThrowNull(nameSymbolPointer);
	ThrowNull(drScoreAccessInterfacePointerPointer);

	// get the arguments for methods that take in a channel index
	AccessArgByIndex(1, &argumentValue);

	ThrowErr(pObj->mmValueInterfacePointer->ValueToInteger(&argumentValue, channelIndexPointer));

	AccessArgByIndex(2, &argumentValue);

	MoaMmSymbol nameSymbol = 0;
	ThrowErr(pObj->mmValueInterfacePointer->ValueToSymbol(&argumentValue, nameSymbolPointer));

	ThrowErr(GetOptions(callPtr, accessProperty, 3, NULL, drScoreAccessInterfacePointerPointer));
	ThrowNull(*drScoreAccessInterfacePointerPointer);

	moa_catch

	if (channelIndexPointer) {
		*channelIndexPointer = 0;
	}

	if (nameSymbolPointer) {
		*nameSymbolPointer = 0;
	}

	if (drScoreAccessInterfacePointerPointer) {
		*drScoreAccessInterfacePointerPointer = NULL;
	}

	moa_catch_end
	moa_try_end
}