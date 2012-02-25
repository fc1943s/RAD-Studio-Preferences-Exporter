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
#define FullDebugMode
  #define LoadDebugDLLDynamically
  #define RawStackTraces

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

typedef unsigned __fastcall (*TGetCppVirtObjSizeByTypeIdPtrFunc)(void * APointer);

typedef void * __fastcall (*TGetCppVirtObjTypeIdPtrFunc)(void * APointer, unsigned ASize);

typedef char * __fastcall (*TGetCppVirtObjTypeNameFunc)(void * APointer, unsigned ASize);

typedef char * __fastcall (*TGetCppVirtObjTypeNameByTypeIdPtrFunc)(void * APointer);

typedef char * __fastcall (*TGetCppVirtObjTypeNameByVTablePtrFunc)(void * AVTablePtr, unsigned AVTablePtrOffset);

typedef System::StaticArray<unsigned, 11> TStackTrace;

typedef TStackTrace *PStackTrace;

#pragma option push -b-
enum TBlockOperation : unsigned char { boBlockCheck, boGetMem, boFreeMem, boReallocMem };
#pragma option pop

struct TFullDebugBlockHeader;
typedef TFullDebugBlockHeader *PFullDebugBlockHeader;

struct DECLSPEC_DRECORD TFullDebugBlockHeader
{
	
public:
	void *Reserved1;
	void *Reserved2;
	void *AllocatedByRoutine;
	unsigned AllocationGroup;
	unsigned AllocationNumber;
	TStackTrace AllocationStackTrace;
	unsigned AllocatedByThread;
	unsigned FreedByThread;
	TStackTrace FreeStackTrace;
	unsigned UserSize;
	unsigned PreviouslyUsedByClass;
	unsigned HeaderCheckSum;
};


class DELPHICLASS TFreedObject;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TFreedObject : public System::TObject
{
	typedef System::TObject inherited;
	
public:
	void __fastcall GetVirtualMethodIndex(void);
	void __fastcall VirtualMethodError(void);
public:
	/* TObject.Create */ inline __fastcall TFreedObject(void) : System::TObject() { }
	/* TObject.Destroy */ inline __fastcall virtual ~TFreedObject(void) { }
	
};

#pragma pack(pop)

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
static const System::Int8 StackTraceDepth = System::Int8(0xb);
static const System::Word AllocationGroupStackSize = System::Word(0x3e8);
static const System::Byte MaxFakeVMTEntries = System::Byte(0xc8);
static const System::Byte DebugFillByte = System::Byte(0x80);
static const unsigned DebugFillPattern = unsigned(0x80808080);
static const unsigned DebugReservedAddress = unsigned(0x80800000);
extern PACKAGE System::TClass __fastcall DetectClassInstance(void * APointer);
extern PACKAGE void __fastcall SetMMLogFileName(char * ALogFileName = (char *)(0x0));
extern PACKAGE unsigned __fastcall GetCurrentAllocationGroup(void);
extern PACKAGE void __fastcall PushAllocationGroup(unsigned ANewCurrentAllocationGroup);
extern PACKAGE void __fastcall PopAllocationGroup(void);
extern PACKAGE void * __fastcall DebugGetMem(int ASize);
extern PACKAGE int __fastcall DebugFreeMem(void * APointer);
extern PACKAGE void * __fastcall DebugReallocMem(void * APointer, int ANewSize);
extern PACKAGE void * __fastcall DebugAllocMem(int ASize);
extern PACKAGE void __fastcall LogAllocatedBlocksToFile(unsigned AFirstAllocationGroupToLog, unsigned ALastAllocationGroupToLog);
extern PACKAGE void __fastcall ScanMemoryPoolForCorruptions(void);
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
