// CustomApp.h - implementations of custom 7zFM methods (header-only inline).
// Included at the END of UI/FileManager/App.h, after all class definitions,
// so the method bodies live in our own file instead of upstream App.cpp.

#ifndef ZIP7_INC_CUSTOM_APP_H
#define ZIP7_INC_CUSTOM_APP_H

#include "CustomIDs.h"
#include "CustomFileManager.h"

#include "../../Windows/PropVariant.h"
#include "../PropID.h"

// CApp: entry used by the AutoExtract toolbar button.
inline void CApp::AutoExtract()
{
  GetFocusedPanel().AutoExtract();
}

// CPanel: one-click extract.
// - plain FS panel: fall back to the upstream extract flow for the selection;
// - browsing inside an archive: extract the currently browsed folder, wrapped
//   into the simplest folder (see ComputeAutoExtractDest), then open it and
//   exit 7zFM. Runs in-process via the upstream CPanel::CopyTo engine, so the
//   archive password and progress dialog come for free.
inline void CPanel::AutoExtract()
{
  if (_parentFolders.IsEmpty())
  {
    ExtractArchives();
    return;
  }

  CRecordVector<UInt32> indices;
  Get_ItemIndices_All(indices);
  if (indices.IsEmpty())
    return;

  const bool singleFolder = (indices.Size() == 1 && IsItem_Folder(indices[0]));

  // outermost archive entry: real file on disk
  const UString baseDir = fs2us(_parentFolders.Front().FolderPath);
  // innermost archive: real file path or virtual path (name source)
  const CFolderLink &link = _parentFolders.Back();
  const UString srcPath = link.IsVirtual ? link.VirtualPath : fs2us(link.FilePath);

  // archive-internal path of the currently browsed folder ("" at the root).
  // do NOT use _currentFolderPrefix here: it holds the whole virtual path
  // including the archive file name itself (e.g. "...\build\content.7z\"),
  // so its last segment would wrongly become the wrapper name.
  UString curName;
  {
    NWindows::NCOM::CPropVariant prop;
    if (_folder && _folder->GetFolderProperty(kpidPath, &prop) == S_OK)
      if (prop.vt == VT_BSTR)
        curName = (wchar_t *)prop.bstrVal;
  }

  Z7Custom::CAutoExtractDest dest;
  if (!Z7Custom::ComputeAutoExtractDest(baseDir,
        curName,
        Z7Custom::GetAutoExtractArcName(srcPath),
        singleFolder,
        singleFolder ? GetItemName(indices[0]) : UString(),
        dest))
    return;

  if (singleFolder)
  {
    CRecordVector<UInt32> one;
    one.Add(indices[0]);
    indices = one;
  }

  // the engine does not create the destination dir: do it explicitly,
  // exactly like upstream CApp::OnCopy does before CopyTo
  UString destDir;
  if (!Z7Custom::PrepareAutoExtractDestDir(dest.destFolder, destDir))
  {
    MessageBox_Error_2Lines_Message_HRESULT(destDir, GetLastError_noZero_HRESULT());
    return;
  }

  CCopyToOptions options;
  options.showErrorMessages = true;
  options.folder = destDir;

  UStringVector messages;
  const HRESULT res = CopyTo(options, indices, &messages);
  if (res == S_OK && messages.IsEmpty())
    Z7Custom::OpenFolderAndExit(dest.openFolder);
}

#endif
