// CustomIDs.h - central place for identifiers added by our custom patch.
// Keeping them here avoids touching upstream resource/ID headers, so conflicts
// when syncing with new 7-Zip releases are trivial to resolve.

#ifndef ZIP7_INC_CUSTOM_IDS_H
#define ZIP7_INC_CUSTOM_IDS_H

// "Append date and time to filename" checkbox in the Compress dialog.
// 4020 is unused in CompressDialogRes.h (4019, then 4040).
#define Z7_CUSTOM_IDX_DATETIME          4020

// Explorer context-menu internal command id. It is intentionally outside
// CZipContextMenu::enum_CommandInternalID (whose members end around 25).
#define Z7_CUSTOM_CMD_ZIP_WITH_DATE     100

// String-table id for the 7zFM AutoExtract toolbar button (7200 series).
#define Z7_CUSTOM_IDS_AUTOEXTRACT       7207

// 7zFM toolbar command id. Upstream toolbar ids are 1070..1073; 1200 keeps
// distance from both the toolbar range and the property-name string ids.
#define Z7_CUSTOM_TOOLBAR_AUTO_EXTRACT  1200

#endif
