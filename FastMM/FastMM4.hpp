// CodeGear C++Builder
// Copyright (c) 1995, 2011 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'FastMM4.pas' rev: 23.00 (Win32)

#ifndef Fastmm4HPP
#define Fastmm4HPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

#define PatchBCBTerminate

#define EnableMemoryLeakReporting

#define DetectMMOperationsAfterUninstall

#define CheckCppObjectTypeEnabled

namespace Fastmm4
{
//-- type declarations -------------------------------------------------------
struct DECLSPEC_DRECORD TSmallBlockTypeState
{
	
public:
	unsigned InternalBlockSize;
	unsigned UseableBlockSize;
	unsigned AllocatedBlockCount;
	unsigned ReservedAddressSpace;
};


typedef System::StaticArray<TSmallBlockTypeState, 56> TSmallBlockTypeStates;

struct DECLSPEC_DRECORD TMemoryManagerState
{
	
public:
	TSmallBlockTypeStates SmallBlockTypeStates;
	unsigned AllocatedMediumBlockCount;
	unsigned TotalAllocatedMediumBlockSize;
	unsigned ReservedMediumBlockAddressSpace;
	unsigned AllocatedLargeBlockCount;
	unsigned TotalAllocatedLargeBlockSize;
	unsigned ReservedLargeBlockAddressSpace;
};


struct DECLSPEC_DRECORD TMemoryManagerUsageSummary
{
	
public:
	unsigned AllocatedBytes;
	unsigned OverheadBytes;
	double EfficiencyPercentage;
};


#pragma option push -b-
enum TChunkStatus : unsigned char { csUnallocated, csAllocated, csReserved, csSysAllocated, csSysReserved };
#pragma option pop

typedef System::StaticArray<TChunkStatus, 65536> TMemoryMap;

struct DECLSPEC_DRECORD TRegisteredMemoryLeak
{
	
public:
	void *LeakAddress;
	System::TClass LeakedClass;
	void *LeakedCppTypeIdPtr;
	int LeakSize;
	int LeakCount;
};


typedef System::DynamicArray<TRegisteredMemoryLeak> TRegisteredMemoryLeaks;

#pragma option push -b-
enum TStringDataType : unsigned char { stUnknown, stAnsiString, stUnicodeString };
#pragma option pop

typedef void __fastcall (*TWalkAllocatedBlocksCallback)(void * APBlock, int ABlockSize, void * AUserData);

typedef unsigned __fastcall (*TGetCppVirtObjSizeByTypeIdPtrFunc)(void * APointer);

typedef void * __fastcall (*TGetCppVirtObjTypeIdPtrFunc)(void * APointer, unsigned ASize);

typedef char * __fastcall (*TGetCppVirtObjTypeNameFunc)(void * APointer, unsigned ASize);

typedef char * __fastcall (*TGetCppVirtObjTypeNameByTypeIdPtrFunc)(void * APointer);

typedef char * __fastcall (*TGetCppVirtObjTypeNameByVTablePtrFunc)(void * AVTablePtr, unsigned AVTablePtrOffset);

//-- var, const, procedure ---------------------------------------------------
#define FastMMVersion L"4.99"
static const System::Int8 NumSmallBlockTypes = System::Int8(0x38);
extern PACKAGE bool FullDebugModeScanMemoryPoolBeforeEveryOperation;
extern PACKAGE bool FullDebugModeRegisterAllAllocsAsExpectedMemoryLeak;
extern PACKAGE bool SuppressMessageBoxes;
extern PACKAGE int *pCppDebugHook;
extern PACKAGE TGetCppVirtObjSizeByTypeIdPtrFunc GetCppVirtObjSizeByTypeIdPtrFunc;
extern PACKAGE TGetCppVirtObjTypeIdPtrFunc GetCppVirtObjTypeIdPtrFunc;
extern PACKAGE TGetCppVirtObjTypeNameFunc GetCppVirtObjTypeNameFunc;
extern PACKAGE TGetCppVirtObjTypeNameByTypeIdPtrFunc GetCppVirtObjTypeNameByTypeIdPtrFunc;
extern PACKAGE TGetCppVirtObjTypeNameByVTablePtrFunc GetCppVirtObjTypeNameByVTablePtrFunc;
extern PACKAGE System::TClass __fastcall DetectClassInstance(void * APointer);
extern PACKAGE void * __fastcall FastGetMem(int ASize);
extern PACKAGE int __fastcall FastFreeMem(void * APointer);
extern PACKAGE void * __fastcall FastReallocMem(void * APointer, int ANewSize);
extern PACKAGE void * __fastcall FastAllocMem(int ASize);
extern PACKAGE bool __fastcall RegisterExpectedMemoryLeak(void * ALeakedPointer)/* overload */;
extern PACKAGE bool __fastcall RegisterExpectedMemoryLeak(System::TClass ALeakedObjectClass, int ACount = 0x1)/* overload */;
extern PACKAGE bool __fastcall RegisterExpectedMemoryLeak(void * ALeakedCppVirtObjTypeIdPtr, int ACount)/* overload */;
extern PACKAGE bool __fastcall RegisterExpectedMemoryLeak(int ALeakedBlockSize, int ACount = 0x1)/* overload */;
extern PACKAGE bool __fastcall UnregisterExpectedMemoryLeak(void * ALeakedPointer)/* overload */;
extern PACKAGE bool __fastcall UnregisterExpectedMemoryLeak(System::TClass ALeakedObjectClass, int ACount = 0x1)/* overload */;
extern PACKAGE bool __fastcall UnregisterExpectedMemoryLeak(void * ALeakedCppVirtObjTypeIdPtr, int ACount)/* overload */;
extern PACKAGE bool __fastcall UnregisterExpectedMemoryLeak(int ALeakedBlockSize, int ACount = 0x1)/* overload */;
extern PACKAGE TRegisteredMemoryLeaks __fastcall GetRegisteredMemoryLeaks(void);
extern PACKAGE TStringDataType __fastcall DetectStringData(void * APMemoryBlock, int AAvailableSpaceInBlock);
extern PACKAGE void __fastcall WalkAllocatedBlocks(TWalkAllocatedBlocksCallback ACallBack, void * AUserData);
extern PACKAGE bool __fastcall LogMemoryManagerStateToFile(const System::UnicodeString AFileName, const System::UnicodeString AAdditionalDetails = System::UnicodeString());
extern PACKAGE void __fastcall GetMemoryManagerState(TMemoryManagerState &AMemoryManagerState);
extern PACKAGE void __fastcall GetMemoryManagerUsageSummary(TMemoryManagerUsageSummary &AMemoryManagerUsageSummary);
extern PACKAGE void __fastcall GetMemoryMap(TChunkStatus *AMemoryMap);
extern PACKAGE System::THeapStatus __fastcall FastGetHeapStatus(void);
extern PACKAGE void __fastcall FreeAllMemory(void);
extern PACKAGE bool __fastcall CheckCanInstallMemoryManager(void);
extern PACKAGE void __fastcall InitializeMemoryManager(void);
extern PACKAGE void __fastcall InstallMemoryManager(void);
extern PACKAGE void __fastcall FinalizeMemoryManager(void);
extern PACKAGE void __fastcall RunInitializationCode(void);

}	/* namespace Fastmm4 */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_FASTMM4)
using namespace Fastmm4;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Fastmm4HPP
