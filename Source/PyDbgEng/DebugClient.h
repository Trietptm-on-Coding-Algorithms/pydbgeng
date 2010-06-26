#pragma once

#include <string>
#include <map>

#include <atlbase.h>
#include <atlcom.h>

#include "DebugObject.h"

#include "DebugEngine.h"
#include "DebugControl.h"
#include "DebugRegisters.h"
#include "DebugSymbols.h"
#include "DebugDataSpaces.h"
#include "DebugSystemObjects.h"
#include "DebugAdvanced.h"

class PYDBGENG_API CDebugClient
  : public CDebugObject<IDebugClient>
{
  typedef CDebugObject<DefaultInterface> __inherited;
public:
  static const ULONG DEFAULT_PROCESS_SERVER = 0;

  static CComQIPtr<DefaultInterface> s_current;

  enum CreateFlags
  {
    CREATE_DEFAULT                  = 0,
    CREATE_ATTACH_PROCESS           = DEBUG_PROCESS,
    CREATE_ATTACH_ONLY_THIS_PROCESS = DEBUG_ONLY_THIS_PROCESS,
    CREATE_SUSPENDED_               = CREATE_SUSPENDED,
    CREATE_NO_DEBUG_HEAP            = DEBUG_CREATE_PROCESS_NO_DEBUG_HEAP,
    CREATE_THROUGH_RTL              = DEBUG_CREATE_PROCESS_THROUGH_RTL
  };

  enum AttachUserFlags
  {
    ATTACH_DEFAULT                    = DEBUG_ATTACH_DEFAULT,
    ATTACH_NONINVASIVE                = DEBUG_ATTACH_NONINVASIVE,
    ATTACH_EXISTING                   = DEBUG_ATTACH_EXISTING,
    ATTACH_NONINVASIVE_NO_SUSPEND     = DEBUG_ATTACH_NONINVASIVE_NO_SUSPEND,
    ATTACH_INVASIVE_NO_INITIAL_BREAK  = DEBUG_ATTACH_INVASIVE_NO_INITIAL_BREAK,
    ATTACH_INVASIVE_RESUME_PROCESS    = DEBUG_ATTACH_INVASIVE_RESUME_PROCESS,
    ATTACH_NONINVASIVE_ALLOW_PARTIAL  = DEBUG_ATTACH_NONINVASIVE_ALLOW_PARTIAL
  };

  enum AttachKernelFlag
  {
    ATTACH_KERNEL_CONNECTION  = DEBUG_ATTACH_KERNEL_CONNECTION,
    ATTACH_LOCAL_KERNEL       = DEBUG_ATTACH_LOCAL_KERNEL,
    ATTACH_EXDI_DRIVER        = DEBUG_ATTACH_EXDI_DRIVER
  };

  enum ChangeStateFlags
  {
    CDS_ALL       = DEBUG_CDS_ALL,
    CDS_REGISTERS = DEBUG_CDS_REGISTERS,
    CDS_DATA      = DEBUG_CDS_DATA
  };

  enum DebugSessionStatus
  {
    SESSION_ACTIVE                       = DEBUG_SESSION_ACTIVE,                       
    SESSION_END_SESSION_ACTIVE_TERMINATE = DEBUG_SESSION_END_SESSION_ACTIVE_TERMINATE, 
    SESSION_END_SESSION_ACTIVE_DETACH    = DEBUG_SESSION_END_SESSION_ACTIVE_DETACH,    
    SESSION_END_SESSION_PASSIVE          = DEBUG_SESSION_END_SESSION_PASSIVE,          
    SESSION_END                          = DEBUG_SESSION_END,                          
    SESSION_REBOOT                       = DEBUG_SESSION_REBOOT,                       
    SESSION_HIBERNATE                    = DEBUG_SESSION_HIBERNATE,                    
    SESSION_FAILURE                      = DEBUG_SESSION_FAILURE                      
  };

  enum DebugEvent
  {
    EVENT_BREAKPOINT            = DEBUG_EVENT_BREAKPOINT,
    EVENT_EXCEPTION             = DEBUG_EVENT_EXCEPTION,
    EVENT_CREATE_THREAD         = DEBUG_EVENT_CREATE_THREAD,
    EVENT_EXIT_THREAD           = DEBUG_EVENT_EXIT_THREAD,
    EVENT_CREATE_PROCESS        = DEBUG_EVENT_CREATE_PROCESS,
    EVENT_EXIT_PROCESS          = DEBUG_EVENT_EXIT_PROCESS,
    EVENT_LOAD_MODULE           = DEBUG_EVENT_LOAD_MODULE,
    EVENT_UNLOAD_MODULE         = DEBUG_EVENT_UNLOAD_MODULE,
    EVENT_SYSTEM_ERROR          = DEBUG_EVENT_SYSTEM_ERROR,
    EVENT_SESSION_STATUS        = DEBUG_EVENT_SESSION_STATUS,
    EVENT_CHANGE_DEBUGGEE_STATE = DEBUG_EVENT_CHANGE_DEBUGGEE_STATE,
    EVENT_CHANGE_ENGINE_STATE   = DEBUG_EVENT_CHANGE_ENGINE_STATE,
    EVENT_CHANGE_SYMBOL_STATE   = DEBUG_EVENT_CHANGE_SYMBOL_STATE
  };

  enum DumpFormat
  {
    FORMAT_DEFAULT                              = DEBUG_FORMAT_DEFAULT,
    FORMAT_CAB_SECONDARY_ALL_IMAGES             = DEBUG_FORMAT_CAB_SECONDARY_ALL_IMAGES,
    FORMAT_WRITE_CAB                            = DEBUG_FORMAT_WRITE_CAB,
    FORMAT_CAB_SECONDARY_FILES                  = DEBUG_FORMAT_CAB_SECONDARY_FILES,
    FORMAT_NO_OVERWRITE                         = DEBUG_FORMAT_NO_OVERWRITE,
    FORMAT_USER_SMALL_FULL_MEMORY               = DEBUG_FORMAT_USER_SMALL_FULL_MEMORY,
    FORMAT_USER_SMALL_HANDLE_DATA               = DEBUG_FORMAT_USER_SMALL_HANDLE_DATA,
    FORMAT_USER_SMALL_UNLOADED_MODULES          = DEBUG_FORMAT_USER_SMALL_UNLOADED_MODULES,
    FORMAT_USER_SMALL_INDIRECT_MEMORY           = DEBUG_FORMAT_USER_SMALL_INDIRECT_MEMORY,
    FORMAT_USER_SMALL_DATA_SEGMENTS             = DEBUG_FORMAT_USER_SMALL_DATA_SEGMENTS,
    FORMAT_USER_SMALL_FILTER_MEMORY             = DEBUG_FORMAT_USER_SMALL_FILTER_MEMORY,
    FORMAT_USER_SMALL_FILTER_PATHS              = DEBUG_FORMAT_USER_SMALL_FILTER_PATHS,
    FORMAT_USER_SMALL_PROCESS_THREAD_DATA       = DEBUG_FORMAT_USER_SMALL_PROCESS_THREAD_DATA,
    FORMAT_USER_SMALL_PRIVATE_READ_WRITE_MEMORY = DEBUG_FORMAT_USER_SMALL_PRIVATE_READ_WRITE_MEMORY,
    FORMAT_USER_SMALL_NO_OPTIONAL_DATA          = DEBUG_FORMAT_USER_SMALL_NO_OPTIONAL_DATA,
    FORMAT_USER_SMALL_FULL_MEMORY_INFO          = DEBUG_FORMAT_USER_SMALL_FULL_MEMORY_INFO,
    FORMAT_USER_SMALL_THREAD_INFO               = DEBUG_FORMAT_USER_SMALL_THREAD_INFO,
    FORMAT_USER_SMALL_CODE_SEGMENTS             = DEBUG_FORMAT_USER_SMALL_CODE_SEGMENTS,
    FORMAT_USER_SMALL_NO_AUXILIARY_STATE        = DEBUG_FORMAT_USER_SMALL_NO_AUXILIARY_STATE,
    FORMAT_USER_SMALL_FULL_AUXILIARY_STATE      = DEBUG_FORMAT_USER_SMALL_FULL_AUXILIARY_STATE      
  };

  class CEventArgs
  {
    CDebugClient *m_owner;
  protected:
    CEventArgs(CDebugClient *owner) : m_owner(owner)
    {

    }
  public:
    const CDebugClient GetOwner(void) const { return CDebugClient(*m_owner); }
  };

  class CBreakpointEventArgs : public CEventArgs
  {
    PDEBUG_BREAKPOINT m_bp;
  public:
    CBreakpointEventArgs(CDebugClient *owner, PDEBUG_BREAKPOINT bp)
      : CEventArgs(owner), m_bp(bp)
    {

    }

    const CDebugControl::CBreakpoint GetBreakpoint(void) const 
    { 
      return CDebugControl::CBreakpoint(NULL, m_bp);
    }
  };

  class CExceptionEventArgs : public CEventArgs
  {
    PEXCEPTION_RECORD64 m_pRecord;
    ULONG m_bFirstChance;
  public:
    CExceptionEventArgs(CDebugClient *owner, PEXCEPTION_RECORD64 pException, ULONG bFirstChance)
      : CEventArgs(owner), m_pRecord(pException), m_bFirstChance(bFirstChance)
    {
    }

    DWORD GetExceptionCode(void) const { return m_pRecord->ExceptionCode; }
    DWORD GetExceptionFlags(void) const { return m_pRecord->ExceptionFlags; }
    DWORD64 GetExceptionAddress(void) const { return m_pRecord->ExceptionAddress; }
    const list GetParameters(void) const;

    bool IsFirstChance(void) const { return m_bFirstChance; }
  };

  class CCreateThreadEventArgs : public CEventArgs
  {
    ULONG64 m_handle, m_dataOffset, m_startOffset;
  public:
    CCreateThreadEventArgs(CDebugClient *owner, ULONG64 Handle, ULONG64 DataOffset, ULONG64 StartOffset)
      : CEventArgs(owner), m_handle(Handle), m_dataOffset(DataOffset), m_startOffset(StartOffset)
    {

    }

    ULONG64 GetHandle(void) const { return m_handle; } 
    ULONG64 GetDataOffset(void) const { return m_dataOffset; }
    ULONG64 GetStartOffset(void) const { return m_startOffset; }
  };

  class CExitThreadEventArgs : public CEventArgs
  {
    ULONG m_exitCode;
  public:
    CExitThreadEventArgs(CDebugClient *owner, ULONG ExitCode)
      : CEventArgs(owner), m_exitCode(ExitCode)
    {

    }

    ULONG GetExitCode(void) const { return m_exitCode; }
  };

  class CCreateProcessEventArgs : public CEventArgs
  {
    ULONG64 m_uImageFileHandle, m_uHandle, m_uBaseOffset,
            m_uInitialThreadHandle, m_uThreadDataOffset, m_uStartOffset;
    ULONG m_uModuleSize, m_uCheckSum, m_uTimeDateStamp;
    PCSTR m_szModuleName, m_szImageName;
  public:
    CCreateProcessEventArgs(CDebugClient *owner, ULONG64 ImageFileHandle, 
      ULONG64 Handle, ULONG64 BaseOffset, ULONG ModuleSize, PCSTR ModuleName, 
      PCSTR ImageName, ULONG CheckSum, ULONG TimeDateStamp, 
      ULONG64 InitialThreadHandle, ULONG64 ThreadDataOffset, ULONG64 StartOffset)
      : CEventArgs(owner), m_uImageFileHandle(ImageFileHandle), m_uHandle(Handle),
        m_uBaseOffset(BaseOffset), m_uModuleSize(ModuleSize), m_szModuleName(ModuleName),
        m_szImageName(ImageName), m_uCheckSum(CheckSum), m_uTimeDateStamp(TimeDateStamp),
        m_uInitialThreadHandle(InitialThreadHandle), m_uThreadDataOffset(ThreadDataOffset),
        m_uStartOffset(StartOffset)
    {

    }

    ULONG64 GetImageFileHandle(void) const { return m_uImageFileHandle; }
    ULONG64 GetHandle(void) const { return m_uHandle; }
    ULONG64 GetBaseOffset(void) const { return m_uBaseOffset; }
    ULONG GetModuleSize(void) const { return m_uModuleSize; }
    PCSTR GetModuleName(void) const { return m_szModuleName; } 
    PCSTR GetImageName(void) const { return m_szImageName; }
    ULONG GetCheckSum(void) const { return m_uCheckSum; }
    ULONG GetTimeDateStamp(void) const { return m_uTimeDateStamp; } 
    ULONG64 GetInitialThreadHandle(void) const { return m_uInitialThreadHandle; }
    ULONG64 GetThreadDataOffset(void) const { return m_uThreadDataOffset; }
    ULONG64 GetStartOffset(void) const { return m_uStartOffset; }
  };

  class CExitProcessEventArgs : public CEventArgs
  {
    ULONG m_exitCode;
  public:
    CExitProcessEventArgs(CDebugClient *owner, ULONG ExitCode)
      : CEventArgs(owner), m_exitCode(ExitCode)
    {

    }

    ULONG GetExitCode(void) const { return m_exitCode; }
  };

  class CLoadModuleEventArgs : public CEventArgs
  {
    ULONG64 m_uImageFileHandle, m_uBaseOffset;
    ULONG m_uModuleSize, m_uCheckSum, m_uTimeDateStamp;
    PCSTR m_szModuleName, m_szImageName;
  public:
    CLoadModuleEventArgs(CDebugClient *owner, ULONG64 ImageFileHandle, 
      ULONG64 BaseOffset, ULONG ModuleSize, PCSTR ModuleName, PCSTR ImageName, 
      ULONG CheckSum, ULONG TimeDateStamp)
      : CEventArgs(owner), m_uImageFileHandle(ImageFileHandle),
        m_uBaseOffset(BaseOffset), m_uModuleSize(ModuleSize),
        m_szModuleName(ModuleName), m_szImageName(ImageName),
        m_uCheckSum(CheckSum), m_uTimeDateStamp(TimeDateStamp)
    {

    }

    ULONG64 GetImageFileHandle(void) const { return m_uImageFileHandle; }
    ULONG64 GetBaseOffset(void) const { return m_uBaseOffset; }
    ULONG GetModuleSize(void) const { return m_uModuleSize; }
    PCSTR GetModuleName(void) const { return m_szModuleName; }
    PCSTR GetImageName(void) const { return m_szImageName; } 
    ULONG GetCheckSum(void) const { return m_uCheckSum; }
    ULONG GetTimeDateStamp(void) const { return m_uTimeDateStamp; }
  };

  class CUnloadModuleEventArgs : public CEventArgs
  {
    PCSTR m_szImageBaseName;
    ULONG64 m_uBaseOffset;
  public:
    CUnloadModuleEventArgs(CDebugClient *owner, PCSTR ImageBaseName, ULONG64 BaseOffset)
      : CEventArgs(owner), m_szImageBaseName(ImageBaseName), m_uBaseOffset(BaseOffset)
    {

    }

    PCSTR GetImageBaseName(void) const { return m_szImageBaseName; }
    ULONG64 GetBaseOffset(void) const { return m_uBaseOffset; }
  };

  class CSystemErrorEventArgs : public CEventArgs
  {
    ULONG m_uError, m_uLevel;
  public:
    CSystemErrorEventArgs(CDebugClient *owner, ULONG Error, ULONG Level)
      : CEventArgs(owner), m_uError(Error), m_uLevel(Level)
    {

    }

    ULONG GetError(void) const { return m_uError; }
    ULONG GetLevel(void) const { return m_uLevel; }
  };

  class CSessionStatusEventArgs : public CEventArgs
  {
    ULONG m_uStatus;
  public:
    CSessionStatusEventArgs(CDebugClient *owner, ULONG Status)
      : CEventArgs(owner), m_uStatus(Status)
    {

    }

    DebugSessionStatus GetStatus(void) const { return (DebugSessionStatus) m_uStatus; }
  };

  class CStateChangeEventArgs : public CEventArgs
  {
    ULONG m_uFlags;
    ULONG64 m_uArgument;
  public:
    CStateChangeEventArgs(CDebugClient *owner, ULONG Flags, ULONG64 Argument)
      : CEventArgs(owner), m_uFlags(Flags), m_uArgument(Argument)
    {

    }

    ChangeStateFlags GetFlags(void) const { return (ChangeStateFlags) m_uFlags; }
    ULONG64 GetArgument(void) const { return m_uArgument; }
  };

  class CDebugCallbacksBase : public CComObjectRootEx<CComMultiThreadModel>
  {
  protected:
    CDebugClient *m_owner;
  public:
    virtual CDebugClient *GetOwner(void) const { return m_owner; }
    virtual void SetOwner(CDebugClient *owner) { m_owner = owner; }
/*
    virtual void Attach(void) = 0;
    virtual void Detach(void) = 0;
*/
     void Attach(void) {}
     void Detach(void) {}
  };

  template <typename I>
  class CDebugCallbacks : public CDebugCallbacksBase, public I
  {
  protected:
    I *m_callback;
  };

  class CDebugEventCallbacks : public CDebugCallbacks<IDebugEventCallbacks>
  {
    typedef std::map<ULONG, object> CallbackMap;

    CallbackMap m_eventCallbacks;
  public:
    BEGIN_COM_MAP(CDebugEventCallbacks)
      COM_INTERFACE_ENTRY(IDebugEventCallbacks)
    END_COM_MAP()

    // IDebugEventCallbacks
    STDMETHOD(GetInterestMask)(__out PULONG Mask);

    STDMETHOD(Breakpoint)(__in PDEBUG_BREAKPOINT Bp);

    STDMETHOD(Exception)(__in PEXCEPTION_RECORD64 Exception, __in ULONG FirstChance);

    STDMETHOD(CreateThread)(__in ULONG64 Handle, __in ULONG64 DataOffset, __in ULONG64 StartOffset);
    STDMETHOD(ExitThread)(__in ULONG ExitCode);

    STDMETHOD(CreateProcess)(__in ULONG64 ImageFileHandle, __in ULONG64 Handle, __in ULONG64 BaseOffset,
      __in ULONG ModuleSize, __in_opt PCSTR ModuleName, __in_opt PCSTR ImageName, __in ULONG CheckSum,
      __in ULONG TimeDateStamp, __in ULONG64 InitialThreadHandle, __in ULONG64 ThreadDataOffset, __in ULONG64 StartOffset);
    STDMETHOD(ExitProcess)(__in ULONG ExitCode);

    STDMETHOD(LoadModule)(__in ULONG64 ImageFileHandle, __in ULONG64 BaseOffset, __in ULONG ModuleSize,
      __in_opt PCSTR ModuleName, __in_opt PCSTR ImageName, __in ULONG CheckSum, __in ULONG TimeDateStamp);
    STDMETHOD(UnloadModule)(__in_opt PCSTR ImageBaseName, __in ULONG64 BaseOffset);

    STDMETHOD(SystemError)(__in ULONG Error, __in ULONG Level);

    STDMETHOD(SessionStatus)(__in ULONG Status);
    STDMETHOD(ChangeDebuggeeState)(__in ULONG Flags, __in ULONG64 Argument);    
    STDMETHOD(ChangeEngineState)(__in ULONG Flags, __in ULONG64 Argument);    
    STDMETHOD(ChangeSymbolState)(__in ULONG Flags, __in ULONG64 Argument);
  private:
    void UpdateEventCallbacks(IDebugEventCallbacks *callbacks);
    
    template <typename T>
    HRESULT CallEventCallback(ULONG id, const T& args)
    {
      CallbackMap::const_iterator it = m_eventCallbacks.find(id);

      if (it == m_eventCallbacks.end()) return E_NOTIMPL;

      try
      {
        it->second(args);

        return S_OK;
      }
      catch (error_already_set)
      {
        PyErr_Print();

        return E_FAIL;
      }    
    }
  public:
    object GetEvent(ULONG id) const;
    void SetEvent(ULONG id, object event);

    void Attach(void) { UpdateEventCallbacks(this); }    
    void Detach(void) { UpdateEventCallbacks(NULL); }

    #define DECLARE_EVENT(name, id) \
      object Get##name(void) const { return GetEvent(id); } \
      void Set##name(object event) { SetEvent(id, event); };

    DECLARE_EVENT(Breakpoint, DEBUG_EVENT_BREAKPOINT);
    DECLARE_EVENT(Exception, DEBUG_EVENT_EXCEPTION);
    DECLARE_EVENT(CreateThread, DEBUG_EVENT_CREATE_THREAD);
    DECLARE_EVENT(ExitThread, DEBUG_EVENT_EXIT_THREAD);
    DECLARE_EVENT(CreateProcess, DEBUG_EVENT_CREATE_PROCESS);
    DECLARE_EVENT(ExitProcess, DEBUG_EVENT_EXIT_PROCESS);
    DECLARE_EVENT(LoadModule, DEBUG_EVENT_LOAD_MODULE);
    DECLARE_EVENT(UnloadModule, DEBUG_EVENT_UNLOAD_MODULE);
    DECLARE_EVENT(SystemError, DEBUG_EVENT_SYSTEM_ERROR);
    DECLARE_EVENT(SessionStatus, DEBUG_EVENT_SESSION_STATUS);
    DECLARE_EVENT(ChangeDebuggeeState, DEBUG_EVENT_CHANGE_DEBUGGEE_STATE);
    DECLARE_EVENT(ChangeEngineState, DEBUG_EVENT_CHANGE_ENGINE_STATE);
    DECLARE_EVENT(ChangeSymbolState, DEBUG_EVENT_CHANGE_SYMBOL_STATE);
  };

  class CInputEventArgs : public CEventArgs
  {
    ULONG m_uBufferSize;
  public:
    CInputEventArgs(CDebugClient *owner, ULONG BufferSize)
      : CEventArgs(owner), m_uBufferSize(BufferSize)
    {

    }

    ULONG GetBufferSize(void) const { return m_uBufferSize; }
  };

  class CDebugInputCallbacks : public CDebugCallbacks<IDebugInputCallbacks>      
  {    
    object m_input;

    void UpdateIntputCallbacks(IDebugInputCallbacks *callbacks);
  public:
    BEGIN_COM_MAP(CDebugInputCallbacks)
      COM_INTERFACE_ENTRY(IDebugInputCallbacks)
    END_COM_MAP()

    STDMETHOD(StartInput)(__in ULONG BufferSize);    
    STDMETHOD(EndInput)(void);

    const object GetInput(void) const { return m_input; }
    void SetInput(const object& obj) { m_input = obj; }

    void Attach(void) { UpdateIntputCallbacks(this); }    
    void Detach(void) { UpdateIntputCallbacks(NULL); }
  };

  class COutputEventArgs : public CEventArgs
  {
    ULONG m_mask;
    std::string m_text;
  public:
    COutputEventArgs(CDebugClient *owner, ULONG Mask, __in PCSTR Text)
      : CEventArgs(owner), m_mask(Mask), m_text(Text)
    {

    }

    OutputMask GetMask(void) const { return (OutputMask) m_mask; }
    const std::string GetText(void) const { return m_text; }
  };

  class CDebugOutputCallbacks : public CDebugCallbacks<IDebugOutputCallbacks>      
  {
    object m_output;    

    void UpdateOutputCallbacks(IDebugOutputCallbacks *callbacks);
  public:
    BEGIN_COM_MAP(CDebugOutputCallbacks)
      COM_INTERFACE_ENTRY(IDebugOutputCallbacks)
    END_COM_MAP()

    STDMETHOD(Output)(__in ULONG Mask, __in PCSTR Text);

    const object GetOutput(void) const { return m_output; }
    void SetOutput(const object& obj) { m_output = obj; }

    void Attach(void) { UpdateOutputCallbacks(this); }    
    void Detach(void) { UpdateOutputCallbacks(NULL); }
  };

  struct Scope
  {
    CComPtr<DefaultInterface> m_outer;

    Scope(PDEBUG_CLIENT4 Client) : m_outer(CDebugClient::s_current)
    {
      CDebugClient::s_current = Client;
    }
    ~Scope(void)
    {
      CDebugClient::s_current = m_outer;
    }
  };
private:
  CComObjectStackEx<CDebugEventCallbacks> m_eventCallbacks;
  CComObjectStackEx<CDebugInputCallbacks> m_inputCallbacks;
  CComObjectStackEx<CDebugOutputCallbacks> m_outputCallbacks;

  void SetCallbackOwner(CDebugClient *client)
  {
    m_eventCallbacks.SetOwner(client);
    m_inputCallbacks.SetOwner(client);
    m_outputCallbacks.SetOwner(client);
  }
public:
  CDebugClient() : __inherited(CDebugEngine::CreateInstance<CDebugClient::DefaultInterface>())
  {
    if(*this) SetCallbackOwner(this);
  }
  CDebugClient(IUnknown *client) : __inherited(client)
  {
    if(*this) SetCallbackOwner(this);
  }
  CDebugClient(const CDebugClient& client) : __inherited(client.m_intf)
  {
    if(*this) SetCallbackOwner(this);
  }
  ~CDebugClient(void)
  {
    if(*this) SetCallbackOwner(NULL);
  }

  static void Export(void);

  static const CDebugClient GetCurrent(void) 
  {
    return CDebugClient(s_current);
  }

  const CDebugControl GetControl(void) const
  {
    return CDebugControl(m_intf);
  }
  const CDebugRegisters GetRegisters(void) const
  {
    return CDebugRegisters(m_intf);
  }
  const CDebugSymbols GetSymbols(void) const
  {
    return CDebugSymbols(m_intf);
  }
  const CDebugDataSpaces GetDataSpaces(void) const
  {
    return CDebugDataSpaces(m_intf);
  }
  const CDebugSystemObjects GetSystemObjects(void) const
  {
    return CDebugSystemObjects(m_intf);
  }
  const CDebugAdvanced GetAdvanced(void) const
  {
    return CDebugAdvanced(m_intf);
  }

  ULONG GetExitCode(void) const;
  const std::string GetIdentity(void) const;
  void OutputIdentity(const std::string& format = "%s", OutputTarget target = TARGET_ALL_CLIENTS) const;

  void CreateProcess(const std::string& commandLine, const std::string& initialDirectory = "", 
    dict environmentVariables = dict(), CreateFlags createFlags = CREATE_DEFAULT, 
    AttachUserFlags attachFlags = ATTACH_DEFAULT, ULONG attachProcessId = 0, ULONG server = DEFAULT_PROCESS_SERVER) const;
  void AttachProcess(ULONG id, AttachUserFlags flags = ATTACH_DEFAULT, ULONG server = DEFAULT_PROCESS_SERVER) const;
  void DetachProcesses(void) const;
  void TerminateProcesses(void) const;
  void AbandonCurrentProcess(void) const;
  void DetachCurrentProcess(void) const;
  void TerminateCurrentProcess(void) const;

  void AttachKernel(AttachKernelFlag flags = ATTACH_LOCAL_KERNEL, const std::string& connectOptions = "") const;
  bool IsKernelDebuggerEnabled(void) const;

  const std::string GetKernelConnectionOptions(void) const;
  void SetKernelConnectionOptions(const std::string& options) const;

  bool DispatchCallbacks(ULONG timeout = INFINITE) const;
  void ExitDispatch(void) const;

  ULONG GetNumberOfEventCallbacks(DebugEvent event) const;
  ULONG GetNumberOfInputCallbacks(void) const;
  ULONG GetNumberOfOutputCallbacks(void) const;

  OutputMask GetOutputMask(void) const;
  void SetOutputMask(OutputMask mask) const;
  ULONG GetOutputWidth(void) const;
  void SetOutputWidth(ULONG width) const;

  const std::string GetQuitLockString(void) const;
  void SetQuitLockString(const std::string str) const;

  void OpenDumpFile(const std::string& filename) const;
  void AddDumpInformationFile(const std::string& filename) const;
  const dict GetDumpFiles(void) const;
  void WriteUserDumpFile(const std::string& filename, UserQualifier qualifier = USER_SMALL_DUMP, DumpFormat format = FORMAT_DEFAULT, const std::string& comment = "") const;
  void WriteKernelDumpFile(const std::string& filename, KernelQualifier qualifier = KERNEL_SMALL_DUMP, DumpFormat format = FORMAT_DEFAULT, const std::string& comment = "") const;
};