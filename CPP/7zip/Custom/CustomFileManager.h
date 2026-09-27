// CustomFileManager.h - header-only helpers for the 7zFM AutoExtract feature.

#ifndef ZIP7_INC_CUSTOM_FILE_MANAGER_H
#define ZIP7_INC_CUSTOM_FILE_MANAGER_H

#include "CustomIDs.h"

#include "../../Common/MyString.h"
#include "../UI/Common/CompressCall.h"

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

// Extract every selected archive into a folder named after the first archive
// (same directory, base name), then open that folder and terminate 7zFM.
// No copy/move dialog is shown, and upstream CApp::OnCopy is never touched.
inline void RunAutoExtract(const UStringVector &arcPaths, UInt32 writeZone)
{
  if (arcPaths.IsEmpty())
    return;

  // destination folder: "<archive path without extension>/"
  UString destFolder = arcPaths[0];
  const int dotPos = FindExtDot(destFolder);
  if (dotPos >= 0)
    destFolder.DeleteFrom(dotPos);
  destFolder.Add_PathSepar();

  ::ExtractArchives(arcPaths, destFolder,
      false, // showDialog
      false, // elimDup
      writeZone);

  OpenFolderAndExit(destFolder);
}

} // namespace Z7Custom

#endif
