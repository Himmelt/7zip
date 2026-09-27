// CustomFileManager.h - header-only helpers for the 7zFM AutoExtract feature.

#ifndef ZIP7_INC_CUSTOM_FILE_MANAGER_H
#define ZIP7_INC_CUSTOM_FILE_MANAGER_H

#include "CustomIDs.h"

#include "../../Common/MyString.h"
#include "../../Windows/FileDir.h"
#include "../../Windows/FileIO.h"
#include "../../Windows/FileName.h"

#include <windows.h>
#include <shellapi.h>
#include <stdlib.h>

namespace Z7Custom {

// extension dot position; -1 when the name has no real extension
inline int FindExtDot(const UString &s)
{
  const int dotPos = s.ReverseFind_Dot();
  return (dotPos > s.ReverseFind_PathSepar() + 1) ? dotPos : -1;
}

// Open the destination folder in Explorer and terminate 7zFM.
inline void OpenFolderAndExit(const UString &path)
{
  ShellExecuteW(NULL, L"open", path, NULL, NULL, SW_SHOW);
  exit(EXIT_SUCCESS);
}

// base name of a file path (dir part stripped), extension removed
inline UString GetAutoExtractArcName(const UString &path)
{
  UString s = path;
  const int sepPos = s.ReverseFind_PathSepar();
  if (sepPos >= 0)
    s.DeleteFrontal((unsigned)sepPos + 1);
  const int dotPos = FindExtDot(s);
  if (dotPos >= 0)
    s.DeleteFrom(dotPos);
  return s;
}

struct CAutoExtractDest
{
  UString destFolder;  // extraction target dir for CPanel::CopyTo
  UString openFolder;  // folder to open in Explorer after extraction
};

// The copy/extract engine does not create the destination directory itself
// (upstream CApp::OnCopy does it explicitly before CopyTo), so make sure the
// target exists and ends with a path separator before extraction.
inline bool PrepareAutoExtractDestDir(const UString &destFolder, UString &destDir)
{
  destDir = destFolder;
  NWindows::NFile::NName::NormalizeDirPathPrefix(destDir);
  return NWindows::NFile::NDir::CreateComplexDir(us2fs(destDir));
}

// Wrapping rule: the extracted content must land inside exactly one folder,
// named after the simplest (most specific) wrapper already available:
//   rule 1: the current dir has exactly one item and it is a folder ->
//           that folder itself is the wrapper: extract it as-is into baseDir
//   rule 2: browsing a subdirectory -> wrap the items into a folder
//           named after the browsed folder
//   rule 3: browsing the archive root (no own name) -> wrap the items into
//           a folder named after the archive (base name without extension)
// Returns false when no usable name exists.
inline bool ComputeAutoExtractDest(
    const UString &baseDir,      // disk dir that will hold the result
    const UString &curName,      // current browsed folder name, empty at root
    const UString &arcName,      // archive base name without extension
    bool singleFolder,           // exactly one item and it is a folder
    const UString &singleName,   // that folder's name
    CAutoExtractDest &dest)
{
  UString base = baseDir;
  while (base.Len() != 0)
  {
    const wchar_t c = base.Back();
    if (c == '\\' || c == '/')
      base.DeleteBack();
    else
      break;
  }
  if (base.IsEmpty())
    return false;

  if (singleFolder)
  {
    if (singleName.IsEmpty())
      return false;
    dest.destFolder = base;    // CopyTo places the single folder here
    dest.openFolder = base;
    dest.openFolder.Add_PathSepar();
    dest.openFolder += singleName;
    return true;
  }

  const UString &wrap = curName.IsEmpty() ? arcName : curName;
  if (wrap.IsEmpty())
    return false;
  dest.destFolder = base;
  dest.destFolder.Add_PathSepar();
  dest.destFolder += wrap;
  dest.destFolder.Add_PathSepar();
  dest.openFolder = dest.destFolder;
  return true;
}

} // namespace Z7Custom

#endif
