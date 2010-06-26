#include "StdAfx.h"
#include "DebugSymbols.h"

void CDebugSymbols::Export(void)
{
  enum_<CDebugSymbols::SymbolOption>("SymbolOption")
    .value("CASE_INSENSITIVE",          CDebugSymbols::OPT_CASE_INSENSITIVE)
    .value("UNDNAME",                   CDebugSymbols::OPT_UNDNAME)
    .value("DEFERRED_LOADS",            CDebugSymbols::OPT_DEFERRED_LOADS)
    .value("NO_CPP",                    CDebugSymbols::OPT_NO_CPP)
    .value("LOAD_LINES",                CDebugSymbols::OPT_LOAD_LINES)
    .value("OMAP_FIND_NEAREST",         CDebugSymbols::OPT_OMAP_FIND_NEAREST)
    .value("LOAD_ANYTHING",             CDebugSymbols::OPT_LOAD_ANYTHING)
    .value("IGNORE_CVREC",              CDebugSymbols::OPT_IGNORE_CVREC)
    .value("NO_UNQUALIFIED_LOADS",      CDebugSymbols::OPT_NO_UNQUALIFIED_LOADS)
    .value("FAIL_CRITICAL_ERRORS",      CDebugSymbols::OPT_FAIL_CRITICAL_ERRORS)
    .value("EXACT_SYMBOLS",             CDebugSymbols::OPT_EXACT_SYMBOLS)
    .value("ALLOW_ABSOLUTE_SYMBOLS",    CDebugSymbols::OPT_ALLOW_ABSOLUTE_SYMBOLS)
    .value("IGNORE_NT_SYMPATH",         CDebugSymbols::OPT_IGNORE_NT_SYMPATH)
    .value("INCLUDE_32BIT_MODULES",     CDebugSymbols::OPT_INCLUDE_32BIT_MODULES)
    .value("PUBLICS_ONLY",              CDebugSymbols::OPT_PUBLICS_ONLY)
    .value("NO_PUBLICS",                CDebugSymbols::OPT_NO_PUBLICS)
    .value("AUTO_PUBLICS",              CDebugSymbols::OPT_AUTO_PUBLICS)
    .value("NO_IMAGE_SEARCH",           CDebugSymbols::OPT_NO_IMAGE_SEARCH)
    .value("SECURE",                    CDebugSymbols::OPT_SECURE)
    .value("NO_PROMPTS",                CDebugSymbols::OPT_NO_PROMPTS)
    .value("OVERWRITE",                 CDebugSymbols::OPT_OVERWRITE)
    .value("IGNORE_IMAGEDIR",           CDebugSymbols::OPT_IGNORE_IMAGEDIR)
    .value("FLAT_DIRECTORY",            CDebugSymbols::OPT_FLAT_DIRECTORY)
    .value("FAVOR_COMPRESSED",          CDebugSymbols::OPT_FAVOR_COMPRESSED)
    .value("ALLOW_ZERO_ADDRESS",        CDebugSymbols::OPT_ALLOW_ZERO_ADDRESS)
    .value("DISABLE_SYMSRV_AUTODETECT", CDebugSymbols::OPT_DISABLE_SYMSRV_AUTODETECT)
    .value("DEBUG",                     CDebugSymbols::OPT_DEBUG)
    ;

  enum_<CDebugSymbols::TypeOption>("TypeOption")
    .value("DEFAULT",             CDebugSymbols::OPT_TYPE_DEFAULT)
    .value("UNICODE_DISPLAY",     CDebugSymbols::OPT_UNICODE_DISPLAY)
    .value("LONGSTATUS_DISPLAY",  CDebugSymbols::OPT_LONGSTATUS_DISPLAY)
    .value("FORCERADIX_OUTPUT",   CDebugSymbols::OPT_FORCERADIX_OUTPUT)
    .value("MATCH_MAXSIZE",       CDebugSymbols::OPT_MATCH_MAXSIZE)
    ;

  scope DebugSymbols = class_<CDebugSymbols>("DebugSymbols", no_init)
    .add_property("SymbolOptions", &CDebugSymbols::GetSymbolOptions, &CDebugSymbols::SetSymbolOptions,
      "the engine's global symbol options.")
    .def("AddSymbolOptions", &CDebugSymbols::AddSymbolOptions,
      "turns on some of the engine's global symbol options.")
    .def("RemoveSymbolOptions", &CDebugSymbols::RemoveSymbolOptions,
      "turns off some of the engine's global symbol options.")

    .add_property("TypeOptions", &CDebugSymbols::GetTypeOptions, &CDebugSymbols::SetTypeOptions,
      "the type formatting options for output generated by the engine.")
    .def("AddTypeOptions", &CDebugSymbols::AddTypeOptions,
      "turns on some type formatting options for output generated by the engine.")
    .def("RemoveTypeOptions", &CDebugSymbols::RemoveTypeOptions,
      "turns off some type formatting options for output generated by the engine.")

    .add_property("ImagePath", &CDebugSymbols::GetImagePath, &CDebugSymbols::SetImagePath,
      "The executable image path is used by the engine when searching for executable images.")
    .def("AppendImagePath", &CDebugSymbols::AppendImagePath, 
      "append directories to the executable image path.")

    .add_property("SymbolPath", &CDebugSymbols::GetSymbolPath, &CDebugSymbols::SetSymbolPath,
      "The symbol path is used by the engine when searching for debug symbol.")
    .def("AppendSymbolPath", &CDebugSymbols::AppendSymbolPath, 
      "append directories to the symbol path.")

    .add_property("SourcePath", &CDebugSymbols::GetSourcePath, &CDebugSymbols::SetSourcePath,
      "The source path is used by the engine when searching for source files.")
    .def("AppendSourcePath", &CDebugSymbols::AppendSourcePath, 
      "append directories to the source path.")

    .add_property("LoadedModules", &CDebugSymbols::GetLoadedModules, 
      "loaded modules in the current process's module list")
    .add_property("UnloadedModules", &CDebugSymbols::GetUnloadedModules,
      "unloaded modules in the current process's module list.")

    .def("GetModule", &CDebugSymbols::GetModuleByName, ("name", arg("start")=0), 
      "search through the target's modules for one with the specified name.")
    .def("GetModule", &CDebugSymbols::GetModuleByOffset, ("offset", arg("start")=0), 
      "searches through the target's modules for one whose memory allocation includes the specified location.")

    .def("GetType", &CDebugSymbols::GetTypeByName, 
      "return the type ID and module of the specified symbol.")
    .def("GetType", &CDebugSymbols::GetTypeByOffset, 
      "the type of the symbol closest to the specified memory location.")

    .def("GetOffsetByName", &CDebugSymbols::GetOffsetByName, 
      "the location of a symbol identified by name.")
    .def("GetNameByOffset", &CDebugSymbols::GetNameByOffset, ("offset", arg("delta")=0), 
      "the name and displacement of the symbol at the specified location in the target's virtual address space.")

    .def("GetSymbolOffsets", &CDebugSymbols::GetSymbolOffsets,
      "search for symbols whose names match a given pattern.")

    .def("GetSymbols", &CDebugSymbols::GetSymbolsByName, 
      "return the symbols whose names match a given pattern.")
    .def("GetSymbols", &CDebugSymbols::GetSymbolsByOffset,
      "returns the symbols which are located at a specified address.")
    ;

  class_<CDebugSymbols::CModule>("Module", no_init)
    .add_property("Base", &CDebugSymbols::CModule::GetBase)

    .add_property("ImageName", &CDebugSymbols::CModule::GetImageName, "The name of the executable file, including the extension.")
    .add_property("ModuleName", &CDebugSymbols::CModule::GetModuleName, "Just the file name without the extension.")
    .add_property("LoadedImageName", &CDebugSymbols::CModule::GetLoadedImageName, "The loaded image name.")
    .add_property("SymbolFileName", &CDebugSymbols::CModule::GetSymbolFileName, "The path and name of the symbol file.")
    .add_property("MappedImageName", &CDebugSymbols::CModule::GetMappedImageName, "The mapped image name.")
    
    .def("GetType", &CDebugSymbols::CModule::GetTypeByName, 
      "look up the specified type and return its type ID.")
    .def("Reload", &CDebugSymbols::CModule::Reload, 
      "delete the engine's symbol information for the specified module and reload these symbols as needed.")

    .def("__repr__", &CDebugSymbols::CModule::Repr)
    ;

  class_<CDebugSymbols::CType>("Type", no_init)
    .add_property("Module", &CDebugSymbols::CType::GetModule, "the module containing the symbol")
    .add_property("Id", &CDebugSymbols::CType::GetId, "the type ID")

    .add_property("Name", &CDebugSymbols::CType::GetName, "name of the type symbol")
    .add_property("Size", &CDebugSymbols::CType::GetSize, "the number of bytes of memory an instance of the type.")

    .add_property("Fields", &CDebugSymbols::CType::GetFields, 
      "the type of fields and its offset within a type")

    .def("ReadPhysical", &CDebugSymbols::CType::ReadPhysical, 
      "reads the value of a variable from the target computer's physical memory.")
    .def("WritePhysical", &CDebugSymbols::CType::WritePhysical,
      "writes the value of a variable in the target computer's physical memory.")
    .def("OutputPhysical", &CDebugSymbols::CType::OutputPhysical,
      ("offset", arg("options")=OPT_TYPE_DEFAULT, arg("target")=TARGET_ALL_CLIENTS),
      "formats the contents of a variable in the target computer's physical memory, and then sends this to the output callbacks.")
    .def("ReadVirtual", &CDebugSymbols::CType::ReadVirtual,
      "reads the value of a variable in the target's virtual memory.")
    .def("WriteVirtual", &CDebugSymbols::CType::WriteVirtual,
      "writes data to the target's virtual address space. The number of bytes written is the size of the specified type.")
    .def("OutputVirtual", &CDebugSymbols::CType::OutputVirtual,
      ("offset", arg("options")=OPT_TYPE_DEFAULT, arg("target")=TARGET_ALL_CLIENTS),
      "formats the contents of a variable in the target's virtual memory, and then sends this to the output callbacks.")
    
    .def("__repr__", &CDebugSymbols::CType::Repr)
    ;

  class_<CDebugSymbols::CSymbol>("Symbol", no_init)
    .add_property("Name", &CDebugSymbols::CSymbol::GetName)
    .add_property("ModuleBase", &CDebugSymbols::CSymbol::GetModuleBase,
      "The base address of the module in the target's virtual address space.")
    .add_property("Module", &CDebugSymbols::CSymbol::GetModule)
    .add_property("Id", &CDebugSymbols::CSymbol::GetId,
      "The symbol ID of the symbol.")
    .add_property("Offset", &CDebugSymbols::CSymbol::GetOffset, 
      "The location of the symbol in the target's virtual address space.")
    .add_property("Size", &CDebugSymbols::CSymbol::GetSize,
      "The size, in bytes, of the symbol's value.")
    .add_property("TypeId", &CDebugSymbols::CSymbol::GetTypeId,
      "The type ID of the symbol.")
    .add_property("Token", &CDebugSymbols::CSymbol::GetToken,
      "The managed token of the symbol.")
    .add_property("Tag", &CDebugSymbols::CSymbol::GetTag,
      "The symbol tag for the type of the symbol.")
    .add_property("Arg32", &CDebugSymbols::CSymbol::GetArg32,
      "The interpretation of Arg32 depends on the type of the symbol.")
    .add_property("Arg64", &CDebugSymbols::CSymbol::GetArg64,
      "The interpretation of Arg64 depends on the type of the symbol.")
    .def("__repr__", &CDebugSymbols::CSymbol::Repr)
    ;
}

void CDebugSymbols::CModule::Init(void)
{
  char szImageName[MAX_PATH], szModuleName[MAX_PATH], szLoadedImageName[MAX_PATH];
  ULONG nImageName = _countof(szImageName), nModuleName = _countof(szModuleName), nLoadedImageName = _countof(szLoadedImageName);

  Check(m_intf->GetModuleNames(DEBUG_ANY_ID, m_base, szImageName, nImageName, &nImageName,
    szModuleName, nModuleName, &nModuleName, szLoadedImageName, nLoadedImageName, &nLoadedImageName));

  m_imageName = std::string(szImageName, nImageName-1);
  m_moduleName = std::string(szModuleName, nModuleName-1);
  m_LoadedImageName = std::string(szLoadedImageName, nLoadedImageName-1);
}

const std::string CDebugSymbols::CModule::GetModuleNameString(ULONG which) const
{
  CComQIPtr<IDebugSymbols2> intf(m_intf);
  char szName[MAX_PATH];
  ULONG nName = _countof(szName);

  Check(intf->GetModuleNameString(which, DEBUG_ANY_ID, m_base, szName, nName, &nName));

  return std::string(szName, nName-1);
}

const CDebugSymbols::CType CDebugSymbols::CModule::GetTypeByName(const std::string& name) const
{
  ULONG id;

  Check(m_intf->GetTypeId(m_base, name.c_str(), &id));

  return CType(m_intf, m_base, id);
}

const std::string CDebugSymbols::CType::GetName(void) const
{
  char szName[MAX_NAME_LENGTH];
  ULONG size = _countof(szName);

  Check(m_intf->GetTypeName(m_module, m_id, szName, size, &size));

  return std::string(szName, size-1);
}
ULONG CDebugSymbols::CType::GetSize(void) const
{
  ULONG size = 0;

  Check(m_intf->GetTypeSize(m_module, m_id, &size));

  return size;
}
const dict CDebugSymbols::CType::GetFields(void) const
{
  CComQIPtr<IDebugSymbols3> intf(m_intf);

  char szName[MAX_NAME_LENGTH];
  ULONG uName, uType, uOffset;

  dict fields;

  for (int i=0; ; i++)
  {
    HRESULT hr = intf->GetFieldName(m_module, m_id, i, szName, _countof(szName), &uName);

    if (S_OK == hr)
    {
      Check(intf->GetFieldTypeAndOffset(m_module, m_id, szName, &uType, &uOffset));

      fields[std::string(szName, uName-1)] = make_tuple(CType(intf, m_module, uType), uOffset);
    }
    else if (E_INVALIDARG == hr) // All Fields done
    {
      break;
    }
    else
    {
      Check(hr);
    }
  }

  return fields;
}

const object CDebugSymbols::CType::ReadPhysical(ULONG64 offset) const
{
  ULONG size = GetSize();
  object buffer(handle<>(::PyBuffer_New(size)));

  LPVOID data = NULL;
  Py_ssize_t len = 0;

  if (0 != ::PyObject_AsWriteBuffer(buffer.ptr(), &data, &len))
    throw_error_already_set();

  Check(m_intf->ReadTypedDataPhysical(offset, m_module, m_id, data, len, &size));

  return buffer;
}
ULONG CDebugSymbols::CType::WritePhysical(ULONG64 offset, const object buffer) const
{
  ULONG size = 0;
  LPCVOID data = NULL;
  Py_ssize_t len = 0;

  if (0 != ::PyObject_AsReadBuffer(buffer.ptr(), &data, &len))
    throw_error_already_set();

  Check(m_intf->WriteTypedDataPhysical(offset, m_module, m_id, (LPVOID) data, len, &size));

  return size;
}
const object CDebugSymbols::CType::ReadVirtual(ULONG64 offset) const
{
  ULONG size = GetSize();
  object buffer(handle<>(::PyBuffer_New(size)));

  LPVOID data = NULL;
  Py_ssize_t len = 0;

  if (0 != ::PyObject_AsWriteBuffer(buffer.ptr(), &data, &len))
    throw_error_already_set();

  Check(m_intf->ReadTypedDataVirtual(offset, m_module, m_id, data, len, &size));

  return buffer;
}
ULONG CDebugSymbols::CType::WriteVirtual(ULONG64 offset, const object buffer) const
{
  ULONG size = 0;
  LPCVOID data = NULL;
  Py_ssize_t len = 0;

  if (0 != ::PyObject_AsReadBuffer(buffer.ptr(), &data, &len))
    throw_error_already_set();

  Check(m_intf->WriteTypedDataVirtual(offset, m_module, m_id, (LPVOID) data, len, &size));

  return size;
}

void CDebugSymbols::CType::OutputPhysical(ULONG64 offset, CDebugSymbols::TypeOption options, OutputTarget target)
{
  Check(m_intf->OutputTypedDataPhysical((ULONG) target, offset, m_module, m_id, (ULONG) options));
}
void CDebugSymbols::CType::OutputVirtual(ULONG64 offset, CDebugSymbols::TypeOption options, OutputTarget target)
{
  Check(m_intf->OutputTypedDataVirtual((ULONG) target, offset, m_module, m_id, (ULONG) options));
}

const std::string CDebugSymbols::CSymbol::GetName(void) const
{
  char szName[MAX_NAME_LENGTH];
  ULONG size = _countof(szName);

  DEBUG_MODULE_AND_ID id = { m_entry.ModuleBase, m_entry.Id };

  Check(m_intf->GetSymbolEntryString(&id, 0, szName, size, &size));

  return std::string(szName, size-1);
}

const list CDebugSymbols::GetSymbolOptions(void) const
{
  ULONG options = 0;

  Check(m_intf->GetSymbolOptions(&options));

  return FlagsToList(static_cast<SymbolOption>(options), OPT_CASE_INSENSITIVE, OPT_DEBUG);
}
void CDebugSymbols::SetSymbolOptions(const list options) const
{
  Check(m_intf->SetSymbolOptions(FlagsFromList<SymbolOption>(options)));
}
void CDebugSymbols::AddSymbolOptions(SymbolOption option) const
{
  Check(m_intf->AddSymbolOptions((ULONG) option));
}
void CDebugSymbols::RemoveSymbolOptions(SymbolOption option) const
{
  Check(m_intf->RemoveSymbolOptions((ULONG) option));
}

const list CDebugSymbols::GetTypeOptions(void) const
{
  CComQIPtr<IDebugSymbols2> intf(m_intf);
  ULONG options = 0;

  Check(intf->GetTypeOptions(&options));

  return FlagsToList(static_cast<TypeOption>(options), OPT_UNICODE_DISPLAY, OPT_MATCH_MAXSIZE);
}
void CDebugSymbols::SetTypeOptions(const list options) const
{
  CComQIPtr<IDebugSymbols2> intf(m_intf);

  Check(intf->SetTypeOptions(FlagsFromList<TypeOption>(options)));
}
void CDebugSymbols::AddTypeOptions(TypeOption option) const
{
  CComQIPtr<IDebugSymbols2> intf(m_intf);

  Check(intf->AddTypeOptions((ULONG) option));
}
void CDebugSymbols::RemoveTypeOptions(TypeOption option) const
{
  CComQIPtr<IDebugSymbols2> intf(m_intf);

  Check(intf->RemoveTypeOptions((ULONG) option));
}

const std::string CDebugSymbols::GetImagePath(void) const
{
  char szPath[MAX_PATH_LENGTH];
  ULONG ulSize = _countof(szPath);

  Check(m_intf->GetImagePath(szPath, ulSize, &ulSize));

  return std::string(szPath, ulSize-1);
}

void CDebugSymbols::SetImagePath(const std::string& path) const
{
  Check(m_intf->SetImagePath(path.c_str()));
}

void CDebugSymbols::AppendImagePath(const std::string& path) const
{
  Check(m_intf->AppendImagePath(path.c_str()));
}

const std::string CDebugSymbols::GetSymbolPath(void) const
{
  char szPath[MAX_PATH_LENGTH];
  ULONG ulSize = _countof(szPath);

  Check(m_intf->GetSymbolPath(szPath, ulSize, &ulSize));

  return std::string(szPath, ulSize-1);
}

void CDebugSymbols::SetSymbolPath(const std::string& path) const
{
  Check(m_intf->SetSymbolPath(path.c_str()));
}

void CDebugSymbols::AppendSymbolPath(const std::string& path) const
{
  Check(m_intf->AppendSymbolPath(path.c_str()));
}

const std::string CDebugSymbols::GetSourcePath(void) const
{
  char szPath[MAX_PATH_LENGTH];
  ULONG ulSize = _countof(szPath);

  Check(m_intf->GetSourcePath(szPath, ulSize, &ulSize));

  return std::string(szPath, ulSize-1);
}

void CDebugSymbols::SetSourcePath(const std::string& path) const
{
  Check(m_intf->SetSourcePath(path.c_str()));
}

void CDebugSymbols::AppendSourcePath(const std::string& path) const
{
  Check(m_intf->AppendSourcePath(path.c_str()));
}

const dict CDebugSymbols::GetLoadedModules(void) const
{
  ULONG loaded = 0, unloaded = 0;

  Check(m_intf->GetNumberModules(&loaded, &unloaded));

  dict modules;

  for (ULONG i=0; i<loaded; i++)
  {
    CModule module(m_intf, i);

    modules[module.GetModuleName()] = module;
  }

  return modules;
}
const dict CDebugSymbols::GetUnloadedModules(void) const
{
  ULONG loaded = 0, unloaded = 0;

  Check(m_intf->GetNumberModules(&loaded, &unloaded));

  dict modules;

  for (ULONG i=loaded; i<loaded+unloaded; i++)
  {
    CModule module(m_intf, i);

    modules[module.GetModuleName()] = module;
  }

  return modules;
}

const CDebugSymbols::CModule CDebugSymbols::GetModuleByName(const std::string& name, ULONG start) const
{
  ULONG idx = 0;

  Check(m_intf->GetModuleByModuleName(name.c_str(), start, &idx, NULL));

  return CModule(m_intf, idx);
}
const CDebugSymbols::CModule CDebugSymbols::GetModuleByOffset(ULONG64 offset, ULONG start) const
{
  ULONG idx = 0;

  Check(m_intf->GetModuleByOffset(offset, start, &idx, NULL));

  return CModule(m_intf, idx);
}

const CDebugSymbols::CType CDebugSymbols::GetTypeByName(const std::string& name) const
{
  ULONG64 module;
  ULONG id;

  Check(m_intf->GetSymbolTypeId(name.c_str(), &id, &module));

  return CType(m_intf, module, id);
}
const CDebugSymbols::CType CDebugSymbols::GetTypeByOffset(ULONG64 offset) const
{
  ULONG64 module;
  ULONG id;

  Check(m_intf->GetOffsetTypeId(offset, &id, &module));

  return CType(m_intf, module, id);
}

ULONG64 CDebugSymbols::GetOffsetByName(const std::string& name) const
{
  ULONG64 offset; 

  HRESULT hr = m_intf->GetOffsetByName(name.c_str(), &offset);

  Check(hr);

  if (S_FALSE == hr)  
    RaiseException("the symbol name was not unique and multiple symbols with that name were found.", PyExc_LookupError);  

  return offset;
}

const tuple CDebugSymbols::GetNameByOffset(ULONG64 offset, LONG delta) const
{
  char szName[MAX_NAME_LENGTH];
  ULONG size = _countof(szName);
  ULONG64 displacement = 0;

  if (delta)
  {
    Check(m_intf->GetNearNameByOffset(offset, delta, szName, size, &size, &displacement));
  }
  else
  {
    Check(m_intf->GetNameByOffset(offset, szName, size, &size, &displacement));
  }

  return make_tuple(std::string(szName, size-1), displacement);
}

const dict CDebugSymbols::GetSymbolOffsets(const std::string& pattern) const
{
  SymbolMatcher matcher(m_intf, pattern);

  dict symbols;
  
  std::string name;
  ULONG64 offset;

  while (matcher.Next(name, offset))
    symbols[name] = offset;

  return symbols;
}

const dict CDebugSymbols::GetSymbolsByName(const std::string& pattern) const
{
  CComQIPtr<IDebugSymbols3> intf(m_intf);
  ULONG count = 0;

  Check(intf->GetSymbolEntriesByName(const_cast<PSTR>(pattern.c_str()), 0, NULL, 0, &count));

  if (count == 0) return dict();

  std::vector<DEBUG_MODULE_AND_ID> ids(count);  

  Check(intf->GetSymbolEntriesByName(const_cast<PSTR>(pattern.c_str()), 0, &ids[0], ids.size(), &count));  

  dict symbols;

  for (size_t i=0; i<ids.size(); i++)
  {
    CSymbol symbol(intf, ids[i]);

    symbols[symbol.GetName()] = make_tuple(symbol, 0);
  }

  return symbols;
}
const dict CDebugSymbols::GetSymbolsByOffset(ULONG64 offset) const
{
  CComQIPtr<IDebugSymbols3> intf(m_intf);

  ULONG count = 1;

  std::vector<DEBUG_MODULE_AND_ID> ids(count);  
  std::vector<ULONG64> displacements(count);

  HRESULT hr = intf->GetSymbolEntriesByOffset(offset, 0, &ids[0], &displacements[0], ids.size(), &count);  

  Check(hr);

  dict symbols;

  if (count == 0) return symbols;

  if (S_OK != hr)
  {
    ids.resize(count);  
    displacements.resize(count);

    Check(intf->GetSymbolEntriesByOffset(offset, 0, &ids[0], &displacements[0], ids.size(), &count));  
  }

  for (size_t i=0; i<ids.size(); i++)
  {
    CSymbol symbol(intf, ids[i]);

    symbols[symbol.GetName()] = make_tuple(symbol, displacements[i]);
  }

  return symbols;
}