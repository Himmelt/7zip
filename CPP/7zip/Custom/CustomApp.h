// CustomApp.h - implementations of custom 7zFM methods (header-only inline).
// Included at the END of UI/FileManager/App.h, after all class definitions,
// so the method bodies live in our own file instead of upstream App.cpp.

#ifndef ZIP7_INC_CUSTOM_APP_H
#define ZIP7_INC_CUSTOM_APP_H

#include "../UI/Common/ZipRegistry.h"

#include "CustomIDs.h"
#include "CustomFileManager.h"

// CPanelCallbackImp: route the one-click extract request to CApp.
inline void CPanelCallbackImp::OnAutoExtract()
{
  _app->AutoExtract(_index);
}

// CPanel: one-click extract - no dialog, extract into a folder named after
// the archive, then open that folder and exit 7zFM.
inline void CPanel::AutoExtract()
{
  if (!_parentFolders.IsEmpty())
  {
    _panelCallback->OnAutoExtract();
    return;
  }
  // archive opened directly: reuse the normal extract flow
  ExtractArchives();
}

// CApp: entry used by the toolbar button.
inline void CApp::AutoExtract()
{
  GetFocusedPanel().AutoExtract();
}

// CApp: collect the selected archives and run extraction without a dialog.
inline void CApp::AutoExtract(unsigned srcPanelIndex)
{
  CPanel &panel = Panels[srcPanelIndex];
  CRecordVector<UInt32> indices;
  panel.Get_ItemIndices_OperSmart(indices);
  if (indices.IsEmpty())
    return;

  UStringVector paths;
  panel.GetFilePaths(indices, paths);

  CContextMenuInfo ci;
  ci.Load();

  Z7Custom::RunAutoExtract(paths, ci.WriteZone);
}

#endif
