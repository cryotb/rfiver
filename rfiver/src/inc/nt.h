#pragma once

namespace nt
{
    typedef LONG KPRIORITY;

    typedef struct _UNICODE_STRING {
        USHORT Length;
        USHORT MaximumLength;
        PWSTR  Buffer;
    } UNICODE_STRING, * PUNICODE_STRING;

    typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
    {
        USHORT UniqueProcessId;
        USHORT CreatorBackTraceIndex;
        UCHAR ObjectTypeIndex;
        UCHAR HandleAttributes;
        USHORT HandleValue;
        PVOID Object;
        ULONG GrantedAccess;
    } SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

    typedef struct _SYSTEM_HANDLE_INFORMATION
    {
        ULONG NumberOfHandles;
        SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
    } SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

    enum _LDR_DLL_LOAD_REASON
    {
        LoadReasonStaticDependency = 0,
        LoadReasonStaticForwarderDependency = 1,
        LoadReasonDynamicForwarderDependency = 2,
        LoadReasonDelayloadDependency = 3,
        LoadReasonDynamicLoad = 4,
        LoadReasonAsImageLoad = 5,
        LoadReasonAsDataLoad = 6,
        LoadReasonEnclavePrimary = 7,
        LoadReasonEnclaveDependency = 8,
        LoadReasonUnknown = -1
    };

    typedef struct _RTL_BALANCED_NODE {
        union {
            struct _RTL_BALANCED_NODE* Children[2];
            struct {
                struct _RTL_BALANCED_NODE* Left;
                struct _RTL_BALANCED_NODE* Right;
            } DUMMYSTRUCTNAME;
        } DUMMYUNIONNAME;

        union {
            UCHAR Red : 1;
            UCHAR Balance : 2;
            ULONG_PTR ParentValue;
        } DUMMYUNIONNAME2;
    } RTL_BALANCED_NODE, * PRTL_BALANCED_NODE;

    typedef struct _LDR_DATA_TABLE_ENTRY
    {
        struct _LIST_ENTRY InLoadOrderLinks; // 0x0
        struct _LIST_ENTRY InMemoryOrderLinks; // 0x10
        struct _LIST_ENTRY InInitializationOrderLinks; // 0x20
        PVOID DllBase; // 0x30
        PVOID EntryPoint; // 0x38
        ULONG SizeOfImage; // 0x40
        struct _UNICODE_STRING FullDllName; // 0x48
        struct _UNICODE_STRING BaseDllName; // 0x58
        UCHAR FlagGroup[4]; // 0x68
        union
        {
            ULONG Flags; // 0x68
            struct
            {
                ULONG PackagedBinary : 1; // 0x68
                ULONG MarkedForRemoval : 1; // 0x68
                ULONG ImageDll : 1; // 0x68
                ULONG LoadNotificationsSent : 1; // 0x68
                ULONG TelemetryEntryProcessed : 1; // 0x68
                ULONG ProcessStaticImport : 1; // 0x68
                ULONG InLegacyLists : 1; // 0x68
                ULONG InIndexes : 1; // 0x68
                ULONG ShimDll : 1; // 0x68
                ULONG InExceptionTable : 1; // 0x68
                ULONG ReservedFlags1 : 2; // 0x68
                ULONG LoadInProgress : 1; // 0x68
                ULONG LoadConfigProcessed : 1; // 0x68
                ULONG EntryProcessed : 1; // 0x68
                ULONG ProtectDelayLoad : 1; // 0x68
                ULONG ReservedFlags3 : 2; // 0x68
                ULONG DontCallForThreads : 1; // 0x68
                ULONG ProcessAttachCalled : 1; // 0x68
                ULONG ProcessAttachFailed : 1; // 0x68
                ULONG CorDeferredValidate : 1; // 0x68
                ULONG CorImage : 1; // 0x68
                ULONG DontRelocate : 1; // 0x68
                ULONG CorILOnly : 1; // 0x68
                ULONG ChpeImage : 1; // 0x68
                ULONG ReservedFlags5 : 2; // 0x68
                ULONG Redirected : 1; // 0x68
                ULONG ReservedFlags6 : 2; // 0x68
                ULONG CompatDatabaseProcessed : 1; // 0x68
            };
        };
        USHORT ObsoleteLoadCount; // 0x6C
        USHORT TlsIndex; // 0x6E
        struct _LIST_ENTRY HashLinks; // 0x70
        ULONG TimeDateStamp; // 0x80
        struct _ACTIVATION_CONTEXT* EntryPointActivationContext; // 0x88
        PVOID Lock; // 0x90
        struct _LDR_DDAG_NODE* DdagNode; // 0x98
        struct _LIST_ENTRY NodeModuleLink; // 0xA0
        struct _LDRP_LOAD_CONTEXT* LoadContext; // 0xB0
        PVOID ParentDllBase; // 0xB8
        PVOID SwitchBackContext; // 0xC0
        struct _RTL_BALANCED_NODE BaseAddressIndexNode; // 0xC8
        struct _RTL_BALANCED_NODE MappingInfoIndexNode; // 0xE0
        ULONG64 OriginalBase; // 0xF8
        _LARGE_INTEGER LoadTime; // 0x100
        ULONG BaseNameHashValue; // 0x108
        _LDR_DLL_LOAD_REASON LoadReason; // 0x10C
        ULONG ImplicitPathOptions; // 0x110
        ULONG ReferenceCount; // 0x114
        ULONG DependentLoadFlags; // 0x118
        UCHAR SigningLevel; // 0x11C
    } LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

    typedef enum _KWAIT_REASON {
        Executive,
        FreePage,
        PageIn,
        PoolAllocation,
        DelayExecution,
        Suspended,
        UserRequest,
        WrExecutive,
        WrFreePage,
        WrPageIn,
        WrPoolAllocation,
        WrDelayExecution,
        WrSuspended,
        WrUserRequest,
        WrSpare0,
        WrQueue,
        WrLpcReceive,
        WrLpcReply,
        WrVirtualMemory,
        WrPageOut,
        WrRendezvous,
        WrKeyedEvent,
        WrTerminated,
        WrProcessInSwap,
        WrCpuRateControl,
        WrCalloutStack,
        WrKernel,
        WrResource,
        WrPushLock,
        WrMutex,
        WrQuantumEnd,
        WrDispatchInt,
        WrPreempted,
        WrYieldExecution,
        WrFastMutex,
        WrGuardedMutex,
        WrRundown,
        WrAlertByThreadId,
        WrDeferredPreempt,
        WrPhysicalFault,
        MaximumWaitReason
    } KWAIT_REASON;

    typedef struct _CLIENT_ID {
        HANDLE UniqueProcess;
        HANDLE UniqueThread;
    } CLIENT_ID;

    typedef struct _SYSTEM_THREAD_INFORMATION
    {
        LARGE_INTEGER KernelTime;
        LARGE_INTEGER UserTime;
        LARGE_INTEGER CreateTime;
        ULONG WaitTime;
        PVOID StartAddress;
        CLIENT_ID ClientId;
        KPRIORITY Priority;
        LONG BasePriority;
        ULONG ContextSwitches;
        ULONG ThreadState;
        KWAIT_REASON WaitReason;
    } SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;

    typedef struct _SYSTEM_PROCESS_INFORMATION
    {
        ULONG NextEntryOffset;
        ULONG NumberOfThreads;
        LARGE_INTEGER WorkingSetPrivateSize; // since VISTA
        ULONG HardFaultCount; // since WIN7
        ULONG NumberOfThreadsHighWatermark; // since WIN7
        ULONGLONG CycleTime; // since WIN7
        LARGE_INTEGER CreateTime;
        LARGE_INTEGER UserTime;
        LARGE_INTEGER KernelTime;
        UNICODE_STRING ImageName;
        KPRIORITY BasePriority;
        HANDLE UniqueProcessId;
        HANDLE InheritedFromUniqueProcessId;
        ULONG HandleCount;
        ULONG SessionId;
        ULONG_PTR UniqueProcessKey; // since VISTA (requires SystemExtendedProcessInformation)
        SIZE_T PeakVirtualSize;
        SIZE_T VirtualSize;
        ULONG PageFaultCount;
        SIZE_T PeakWorkingSetSize;
        SIZE_T WorkingSetSize;
        SIZE_T QuotaPeakPagedPoolUsage;
        SIZE_T QuotaPagedPoolUsage;
        SIZE_T QuotaPeakNonPagedPoolUsage;
        SIZE_T QuotaNonPagedPoolUsage;
        SIZE_T PagefileUsage;
        SIZE_T PeakPagefileUsage;
        SIZE_T PrivatePageCount;
        LARGE_INTEGER ReadOperationCount;
        LARGE_INTEGER WriteOperationCount;
        LARGE_INTEGER OtherOperationCount;
        LARGE_INTEGER ReadTransferCount;
        LARGE_INTEGER WriteTransferCount;
        LARGE_INTEGER OtherTransferCount;
        SYSTEM_THREAD_INFORMATION Threads[1];
    } SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;

    typedef struct _PEB_LDR_DATA
    {
        ULONG Length;
        BOOLEAN Initialized;
        HANDLE SsHandle;
        LIST_ENTRY InLoadOrderModuleList;
        LIST_ENTRY InMemoryOrderModuleList;
        LIST_ENTRY InInitializationOrderModuleList;
        PVOID EntryInProgress;
        BOOLEAN ShutdownInProgress;
        HANDLE ShutdownThreadId;
    } PEB_LDR_DATA, * PPEB_LDR_DATA;

    typedef struct _PEB
    {
        UCHAR InheritedAddressSpace; // 0x0
        UCHAR ReadImageFileExecOptions; // 0x1
        UCHAR BeingDebugged; // 0x2
        union
        {
            UCHAR BitField; // 0x3
            struct
            {
                UCHAR ImageUsesLargePages : 1; // 0x3
                UCHAR IsProtectedProcess : 1; // 0x3
                UCHAR IsImageDynamicallyRelocated : 1; // 0x3
                UCHAR SkipPatchingUser32Forwarders : 1; // 0x3
                UCHAR IsPackagedProcess : 1; // 0x3
                UCHAR IsAppContainer : 1; // 0x3
                UCHAR IsProtectedProcessLight : 1; // 0x3
                UCHAR IsLongPathAwareProcess : 1; // 0x3
            };
        };
        UCHAR Padding0[4]; // 0x4
        PVOID Mutant; // 0x8
        PVOID ImageBaseAddress; // 0x10
        struct _PEB_LDR_DATA* Ldr; // 0x18
        struct _RTL_USER_PROCESS_PARAMETERS* ProcessParameters; // 0x20
        PVOID SubSystemData; // 0x28
        PVOID ProcessHeap; // 0x30
        struct _RTL_CRITICAL_SECTION* FastPebLock; // 0x38
        UCHAR AtlThunkSListPtr[0x8]; // 0x40
        PVOID IFEOKey; // 0x48
        union
        {
            ULONG CrossProcessFlags; // 0x50
            struct
            {
                ULONG ProcessInJob : 1; // 0x50
                ULONG ProcessInitializing : 1; // 0x50
                ULONG ProcessUsingVEH : 1; // 0x50
                ULONG ProcessUsingVCH : 1; // 0x50
                ULONG ProcessUsingFTH : 1; // 0x50
                ULONG ProcessPreviouslyThrottled : 1; // 0x50
                ULONG ProcessCurrentlyThrottled : 1; // 0x50
                ULONG ProcessImagesHotPatched : 1; // 0x50
                ULONG ReservedBits0 : 24; // 0x50
            };
        };
        UCHAR Padding1[4]; // 0x54
        PVOID KernelCallbackTable; // 0x58
        PVOID UserSharedInfoPtr; // 0x58
        ULONG SystemReserved; // 0x60
        ULONG AtlThunkSListPtr32; // 0x64
        PVOID ApiSetMap; // 0x68
        ULONG TlsExpansionCounter; // 0x70
        UCHAR Padding2[4]; // 0x74
        PVOID TlsBitmap; // 0x78
        ULONG TlsBitmapBits[2]; // 0x80
        PVOID ReadOnlySharedMemoryBase; // 0x88
        PVOID SharedData; // 0x90
        PVOID* ReadOnlyStaticServerData; // 0x98
        PVOID AnsiCodePageData; // 0xA0
        PVOID OemCodePageData; // 0xA8
        PVOID UnicodeCaseTableData; // 0xB0
        ULONG NumberOfProcessors; // 0xB8
        ULONG NtGlobalFlag; // 0xBC
        LARGE_INTEGER CriticalSectionTimeout; // 0xC0
        ULONG64 HeapSegmentReserve; // 0xC8
        ULONG64 HeapSegmentCommit; // 0xD0
        ULONG64 HeapDeCommitTotalFreeThreshold; // 0xD8
        ULONG64 HeapDeCommitFreeBlockThreshold; // 0xE0
        ULONG NumberOfHeaps; // 0xE8
        ULONG MaximumNumberOfHeaps; // 0xEC
        PVOID* ProcessHeaps; // 0xF0
        PVOID GdiSharedHandleTable; // 0xF8
        PVOID ProcessStarterHelper; // 0x100
        ULONG GdiDCAttributeList; // 0x108
        UCHAR Padding3[4]; // 0x10C
        struct _RTL_CRITICAL_SECTION* LoaderLock; // 0x110
        ULONG OSMajorVersion; // 0x118
        ULONG OSMinorVersion; // 0x11C
        USHORT OSBuildNumber; // 0x120
        USHORT OSCSDVersion; // 0x122
        ULONG OSPlatformId; // 0x124
        ULONG ImageSubsystem; // 0x128
        ULONG ImageSubsystemMajorVersion; // 0x12C
        ULONG ImageSubsystemMinorVersion; // 0x130
        UCHAR Padding4[4]; // 0x134
        ULONG64 ActiveProcessAffinityMask; // 0x138
        ULONG GdiHandleBuffer[60]; // 0x140
        void* PostProcessInitRoutine; // 0x230
        PVOID TlsExpansionBitmap; // 0x238
        ULONG TlsExpansionBitmapBits[32]; // 0x240
        ULONG SessionId; // 0x2C0
        UCHAR Padding5[4]; // 0x2C4
        UCHAR AppCompatFlags[0x4]; // 0x2C8
        UCHAR AppCompatFlagsUser[0x8]; // 0x2D0
        PVOID pShimData; // 0x2D8
        PVOID AppCompatInfo; // 0x2E0
        struct _UNICODE_STRING CSDVersion; // 0x2E8
        struct _ACTIVATION_CONTEXT_DATA* ActivationContextData; // 0x2F8
        struct _ASSEMBLY_STORAGE_MAP* ProcessAssemblyStorageMap; // 0x300
        struct _ACTIVATION_CONTEXT_DATA* SystemDefaultActivationContextData; // 0x308
        struct _ASSEMBLY_STORAGE_MAP* SystemAssemblyStorageMap; // 0x310
        ULONG64 MinimumStackCommit; // 0x318
        PVOID SparePointers[4]; // 0x320
        ULONG SpareUlongs[5]; // 0x340
        PVOID WerRegistrationData; // 0x358
        PVOID WerShipAssertPtr; // 0x360
        PVOID pUnused; // 0x368
        PVOID pImageHeaderHash; // 0x370
        union
        {
            ULONG TracingFlags; // 0x378
            struct
            {
                ULONG HeapTracingEnabled : 1; // 0x378
                ULONG CritSecTracingEnabled : 1; // 0x378
                ULONG LibLoaderTracingEnabled : 1; // 0x378
                ULONG SpareTracingBits : 29; // 0x378
            };
        };
        UCHAR Padding6[4]; // 0x37C
        ULONG64 CsrServerReadOnlySharedMemoryBase; // 0x380
        ULONG64 TppWorkerpListLock; // 0x388
        struct _LIST_ENTRY TppWorkerpList; // 0x390
        PVOID WaitOnAddressHashTable[128]; // 0x3A0
        PVOID TelemetryCoverageHeader; // 0x7A0
        ULONG CloudFileFlags; // 0x7A8
        ULONG CloudFileDiagFlags; // 0x7AC
        CHAR PlaceholderCompatibilityMode; // 0x7B0
        CHAR PlaceholderCompatibilityModeReserved[7]; // 0x7B1
        struct _LEAP_SECOND_DATA* LeapSecondData; // 0x7B8
        union
        {
            ULONG LeapSecondFlags; // 0x7C0
            struct
            {
                ULONG SixtySecondEnabled : 1; // 0x7C0
                ULONG Reserved : 31; // 0x7C0
            };
        };
        ULONG NtGlobalFlag2; // 0x7C4
    } PEB, * PPEB;

    typedef struct _PROCESS_BASIC_INFORMATION
    {
        NTSTATUS ExitStatus;
        PPEB PebBaseAddress;
        KAFFINITY AffinityMask;
        KPRIORITY BasePriority;
        HANDLE UniqueProcessId;
        HANDLE InheritedFromUniqueProcessId;
    } PROCESS_BASIC_INFORMATION, * PPROCESS_BASIC_INFORMATION;

    enum PROCESSINFOCLASS
    {
        ProcessBasicInformation = 0x00,
        ProcessQuotaLimits = 0x01,
        ProcessIoCounters = 0x02,
        ProcessVmCounters = 0x03,
        ProcessTimes = 0x04,
        ProcessBasePriority = 0x05,
        ProcessRaisePriority = 0x06,
        ProcessDebugPort = 0x07,
        ProcessExceptionPort = 0x08,
        ProcessAccessToken = 0x09,
        ProcessLdtInformation = 0x0A,
        ProcessLdtSize = 0x0B,
        ProcessDefaultHardErrorMode = 0x0C,
        ProcessIoPortHandlers = 0x0D,
        ProcessPooledUsageAndLimits = 0x0E,
        ProcessWorkingSetWatch = 0x0F,
        ProcessUserModeIOPL = 0x10,
        ProcessEnableAlignmentFaultFixup = 0x11,
        ProcessPriorityClass = 0x12,
        ProcessWx86Information = 0x13,
        ProcessHandleCount = 0x14,
        ProcessAffinityMask = 0x15,
        ProcessPriorityBoost = 0x16,
        ProcessDeviceMap = 0x17,
        ProcessSessionInformation = 0x18,
        ProcessForegroundInformation = 0x19,
        ProcessWow64Information = 0x1A,
        ProcessImageFileName = 0x1B,
        ProcessLUIDDeviceMapsEnabled = 0x1C,
        ProcessBreakOnTermination = 0x1D,
        ProcessDebugObjectHandle = 0x1E,
        ProcessDebugFlags = 0x1F,
        ProcessHandleTracing = 0x20,
        ProcessIoPriority = 0x21,
        ProcessExecuteFlags = 0x22,
        ProcessResourceManagement = 0x23,
        ProcessCookie = 0x24,
        ProcessImageInformation = 0x25,
        ProcessCycleTime = 0x26,
        ProcessPagePriority = 0x27,
        ProcessInstrumentationCallback = 0x28,
        ProcessThreadStackAllocation = 0x29,
        ProcessWorkingSetWatchEx = 0x2A,
        ProcessImageFileNameWin32 = 0x2B,
        ProcessImageFileMapping = 0x2C,
        ProcessAffinityUpdateMode = 0x2D,
        ProcessMemoryAllocationMode = 0x2E,
        ProcessGroupInformation = 0x2F,
        ProcessTokenVirtualizationEnabled = 0x30,
        ProcessConsoleHostProcess = 0x31,
        ProcessWindowInformation = 0x32,
        ProcessHandleInformation = 0x33,
        ProcessMitigationPolicy = 0x34,
        ProcessDynamicFunctionTableInformation = 0x35,
        ProcessHandleCheckingMode = 0x36,
        ProcessKeepAliveCount = 0x37,
        ProcessRevokeFileHandles = 0x38,
        ProcessWorkingSetControl = 0x39,
        ProcessHandleTable = 0x3A,
        ProcessCheckStackExtentsMode = 0x3B,
        ProcessCommandLineInformation = 0x3C,
        ProcessProtectionInformation = 0x3D,
        ProcessMemoryExhaustion = 0x3E,
        ProcessFaultInformation = 0x3F,
        ProcessTelemetryIdInformation = 0x40,
        ProcessCommitReleaseInformation = 0x41,
        ProcessDefaultCpuSetsInformation = 0x42,
        ProcessAllowedCpuSetsInformation = 0x43,
        ProcessSubsystemProcess = 0x44,
        ProcessJobMemoryInformation = 0x45,
        ProcessInPrivate = 0x46,
        ProcessRaiseUMExceptionOnInvalidHandleClose = 0x47,
        ProcessIumChallengeResponse = 0x48,
        ProcessChildProcessInformation = 0x49,
        ProcessHighGraphicsPriorityInformation = 0x4A,
        ProcessSubsystemInformation = 0x4B,
        ProcessEnergyValues = 0x4C,
        ProcessActivityThrottleState = 0x4D,
        ProcessActivityThrottlePolicy = 0x4E,
        ProcessWin32kSyscallFilterInformation = 0x4F,
        ProcessDisableSystemAllowedCpuSets = 0x50,
        ProcessWakeInformation = 0x51,
        ProcessEnergyTrackingState = 0x52,
        ProcessManageWritesToExecutableMemory = 0x53,
        ProcessCaptureTrustletLiveDump = 0x54,
        ProcessTelemetryCoverage = 0x55,
        ProcessEnclaveInformation = 0x56,
        ProcessEnableReadWriteVmLogging = 0x57,
        ProcessUptimeInformation = 0x58,
        ProcessImageSection = 0x59,
        ProcessDebugAuthInformation = 0x5A,
        ProcessSystemResourceManagement = 0x5B,
        ProcessSequenceNumber = 0x5C,
        ProcessLoaderDetour = 0x5D,
        ProcessSecurityDomainInformation = 0x5E,
        ProcessCombineSecurityDomainsInformation = 0x5F,
        ProcessEnableLogging = 0x60,
        ProcessLeapSecondInformation = 0x61,
        ProcessFiberShadowStackAllocation = 0x62,
        ProcessFreeFiberShadowStackAllocation = 0x63,
        MaxProcessInfoClass = 0x64
    };
}

EXTERN_C NTSTATUS NtQueryVirtualMemory(
    HANDLE                   ProcessHandle,
    PVOID                    BaseAddress,
    INT                      MemoryInformationClass,
    PVOID                    MemoryInformation,
    SIZE_T                   MemoryInformationLength,
    PSIZE_T                  ReturnLength
);

EXTERN_C NTSTATUS NtReadVirtualMemory(
    HANDLE               ProcessHandle,
    PVOID                BaseAddress,
    PVOID               Buffer,
    ULONG                NumberOfBytesToRead,
    PULONG              NumberOfBytesReaded
);

EXTERN_C PVOID NTAPI
RtlFindExportedRoutineByName(
    PVOID DllBase,
    PCHAR RoutineName);

EXTERN_C NTSTATUS NtDuplicateObject(
    HANDLE      SourceProcessHandle,
    HANDLE      SourceHandle,
    HANDLE      TargetProcessHandle,
    PHANDLE     TargetHandle,
    ACCESS_MASK DesiredAccess,
    ULONG       HandleAttributes,
    ULONG       Options
);


namespace nt
{
    inline auto query_virt_mem(void* proc, void* base, void* info)
    {
        size_t ret_len{ };

        return NtQueryVirtualMemory(
            proc,
            base,
            0, /* MEMORY_BASIC_INFO */
            info,
            0x30,
            &ret_len
        );
    }
}

#define NtCurrentThread ((HANDLE)-2)
#define API_SET_SCHEMA_ENTRY_FLAGS_SEALED 1

namespace nt
{
    typedef struct _API_SET_NAMESPACE {
        ULONG Version;
        ULONG Size;
        ULONG Flags;
        ULONG Count;
        ULONG EntryOffset;
        ULONG HashOffset;
        ULONG HashFactor;
    } API_SET_NAMESPACE, * PAPI_SET_NAMESPACE;

    typedef struct _API_SET_HASH_ENTRY {
        ULONG Hash;
        ULONG Index;
    } API_SET_HASH_ENTRY, * PAPI_SET_HASH_ENTRY;

    typedef struct _API_SET_NAMESPACE_ENTRY {
        ULONG Flags;
        ULONG NameOffset;
        ULONG NameLength;
        ULONG HashedLength;
        ULONG ValueOffset;
        ULONG ValueCount;
    } API_SET_NAMESPACE_ENTRY, * PAPI_SET_NAMESPACE_ENTRY;

    typedef struct _API_SET_VALUE_ENTRY {
        ULONG Flags;
        ULONG NameOffset;
        ULONG NameLength;
        ULONG ValueOffset;
        ULONG ValueLength;
    } API_SET_VALUE_ENTRY, * PAPI_SET_VALUE_ENTRY;

    typedef enum _SECTION_INHERIT {

        ViewShare = 1,
        ViewUnmap = 2
    } SECTION_INHERIT, * PSECTION_INHERIT;

    template < typename T >
    inline T FindExport(const std::string& name)
    {
        auto* const h_module = GetModuleHandleA("ntdll.dll");

        if (h_module == nullptr)
            return T{};

        return (T)GetProcAddress(h_module, name.c_str());
    }

    __forceinline NTSTATUS SetInfoThread(
        HANDLE          ThreadHandle,
        THREADINFOCLASS ThreadInformationClass,
        PVOID           ThreadInformation,
        ULONG           ThreadInformationLength
    )
    {
        auto* const function =
            FindExport<decltype(SetInfoThread)*>
            ("NtSetInformationThread");

        return function(
            ThreadHandle,
            ThreadInformationClass,
            ThreadInformation,
            ThreadInformationLength
        );
    }

    __forceinline NTSTATUS QueryInfoThread(
        HANDLE          ThreadHandle,
        THREADINFOCLASS ThreadInformationClass,
        PVOID           ThreadInformation,
        ULONG           ThreadInformationLength,
        PULONG          ReturnLength
    )
    {
        auto* const function =
            FindExport<decltype(QueryInfoThread)*>
            ("NtQueryInformationThread");

        return function(
            ThreadHandle,
            ThreadInformationClass,
            ThreadInformation,
            ThreadInformationLength,
            ReturnLength
        );
    }

    __forceinline NTSTATUS CreateSection(
        PHANDLE SectionHandle,
        ACCESS_MASK DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes,
        PLARGE_INTEGER MaximumSize,
        ULONG SectionPageProtection,
        ULONG AllocationAttributes,
        HANDLE FileHandle
    )
    {
        auto* const function =
            FindExport<decltype(CreateSection)*>
            ("NtCreateSection");

        return function(
            SectionHandle,
            DesiredAccess,
            ObjectAttributes,
            MaximumSize,
            SectionPageProtection,
            AllocationAttributes,
            FileHandle
        );
    }

    __forceinline NTSTATUS OpenSection(
        PHANDLE SectionHandle,
        ACCESS_MASK DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes
    )
    {
        auto* const function =
            FindExport<decltype(OpenSection)*>
            ("NtOpenSection");

        return function(
            SectionHandle,
            DesiredAccess,
            ObjectAttributes
        );
    }

    __forceinline NTSTATUS MapViewOfSection(

        HANDLE          SectionHandle,
        HANDLE          ProcessHandle,
        PVOID* BaseAddress,
        ULONG_PTR       ZeroBits,
        SIZE_T          CommitSize,
        PLARGE_INTEGER  SectionOffset,
        PSIZE_T         ViewSize,
        SECTION_INHERIT InheritDisposition,
        ULONG           AllocationType,
        ULONG           Win32Protect
    )
    {
        auto* const function =
            FindExport<decltype(MapViewOfSection)*>
            ("NtMapViewOfSection");

        return function(
            SectionHandle,
            ProcessHandle,
            BaseAddress,
            ZeroBits,
            CommitSize,
            SectionOffset,
            ViewSize,
            InheritDisposition,
            AllocationType,
            Win32Protect
        );
    }
}

namespace util
{
    __forceinline HANDLE create_section(const wchar_t* name, size_t length, DWORD protect)
    {
        NTSTATUS status;
        HANDLE section_handle;
        LARGE_INTEGER section_length;
        UNICODE_STRING section_name;
        OBJECT_ATTRIBUTES object_attribs;

        RtlInitUnicodeString(&section_name, name);

        section_length.QuadPart = length;

        InitializeObjectAttributes(&object_attribs, &section_name, OBJ_CASE_INSENSITIVE, nullptr, 0);

        if (!NT_SUCCESS(status = nt::CreateSection(
            &section_handle, SECTION_ALL_ACCESS, &object_attribs,
            &section_length, protect, SEC_COMMIT, nullptr
        )))
        {
            printf("create_section failed = %x.\n", status);
            return nullptr;
        }

        return section_handle;
    }

    __forceinline HANDLE open_section(const wchar_t* name)
    {
        NTSTATUS status;
        HANDLE section_handle;
        UNICODE_STRING section_name;
        OBJECT_ATTRIBUTES object_attribs;

        RtlInitUnicodeString(&section_name, name);

        InitializeObjectAttributes(&object_attribs, &section_name, OBJ_CASE_INSENSITIVE, nullptr, 0);

        if (!NT_SUCCESS(status = nt::OpenSection(&section_handle, SECTION_ALL_ACCESS, &object_attribs)))
            return nullptr;

        return section_handle;
    }

    __forceinline PVOID map_view_of_section(HANDLE section_handle, HANDLE process_handle, SIZE_T length, ULONG protect)
    {
        auto status = NTSTATUS{};
        auto result = PVOID{};
        auto view_length = length;

        if (!NT_SUCCESS(status =
            nt::MapViewOfSection(
                section_handle, process_handle,
                &result, 0,
                length, 0,
                &view_length,
                nt::ViewShare,
                0,
                protect)
        ))
        {
            printf("map_view_of_section failed = %x.\n", status);
            return nullptr;
        }

        return result;
    }
}

