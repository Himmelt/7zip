// CustomContextMenu.h - helpers for the Explorer "compress with datetime"
// context-menu command (header-only inline).

#ifndef ZIP7_INC_CUSTOM_CONTEXT_MENU_H
#define ZIP7_INC_CUSTOM_CONTEXT_MENU_H

#include "../UI/Common/CompressCall.h"

#include "CustomDateTime.h"
#include "CustomIDs.h"

namespace Z7Custom {

// Compress the given files into the archive described by (folder, arcName,
// arcType) without showing the compress dialog.
inline HRESULT CompressZipWithDatetime(
    const UString &folder,
    const UString &arcName,
    const UString &arcType,
    const UStringVector &fileNames)
{
  return CompressFiles(folder, arcName, arcType, false,
      fileNames, false, false, false);
}

} // namespace Z7Custom

#endif
