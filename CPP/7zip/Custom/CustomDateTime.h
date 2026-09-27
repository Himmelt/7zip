// CustomDateTime.h - header-only helpers for the "append date/time" feature.
// Used by the Compress dialog (GUI) and the Explorer context menu.

#ifndef ZIP7_INC_CUSTOM_DATE_TIME_H
#define ZIP7_INC_CUSTOM_DATE_TIME_H

#include "CustomIDs.h"

#include "../../Common/MyString.h"
#include "../../Windows/Control/ComboBox.h"

#include <ctime>

namespace Z7Custom {

// current local time formatted as "_YYYYMMDDHHMMSS"
inline UString GetTimestamp()
{
  const std::time_t t = std::time(nullptr);
  std::tm lt;
  localtime_s(&lt, &t);
  char buffer[16] = {};
  std::strftime(buffer, sizeof(buffer), "_%Y%m%d%H%M%S", &lt);
  return UString(buffer);
}

// extension dot position; -1 when the name has no real extension
inline int FindExtDot(const UString &s)
{
  const int dotPos = s.ReverseFind_Dot();
  return (dotPos > s.ReverseFind_PathSepar() + 1) ? dotPos : -1;
}

// true if the 15 chars before dotPos are "_YYYYMMDDHHMMSS"
inline bool IsTimestampAt(const UString &s, int dotPos)
{
  if (dotPos < 15)
    return false;
  const UString c = s.Mid(dotPos - 15, 15);
  if (c[0] != '_')
    return false;
  for (int i = 1; i < 15; i++)
    if (c[i] < L'0' || c[i] > L'9')
      return false;
  return true;
}

// Add/remove the timestamp in the archive-name edit box according to the
// Z7_CUSTOM_IDX_DATETIME checkbox state.
inline void ToggleArchiveDatetime(HWND hDlg, NWindows::NControl::CComboBox &pathCombo)
{
  UString s;
  pathCombo.GetText(s);
  int dotPos = FindExtDot(s);

  // always strip a previous timestamp first
  if (IsTimestampAt(s, dotPos))
  {
    s = s.Left(dotPos - 15) + s.Mid(dotPos, s.Len() - dotPos);
    dotPos = FindExtDot(s);
  }

  const HWND checkBox = GetDlgItem(hDlg, Z7_CUSTOM_IDX_DATETIME);
  if (checkBox && SendMessage(checkBox, BM_GETCHECK, 0, 0) == BST_CHECKED)
  {
    const UString ts = GetTimestamp();
    if (dotPos >= 0)
      s = s.Left(dotPos) + ts + s.Mid(dotPos, s.Len() - dotPos);
    else
      s += ts;
  }

  pathCombo.SetText(s);
}

// e.g. MakeTimestampedName("name", L".zip") -> "name_YYYYMMDDHHMMSS.zip"
inline UString MakeTimestampedName(const UString &base, const wchar_t *ext)
{
  UString result = base + GetTimestamp();
  result += ext;
  return result;
}

} // namespace Z7Custom

#endif
