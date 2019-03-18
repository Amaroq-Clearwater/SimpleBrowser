#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#ifndef _INC_WINDOWS
    #include <windows.h>
#endif
#include <string>
#include <vector>

struct SETTINGS
{
    INT m_x, m_y, m_cx, m_cy;
    BOOL m_bMaximized;
    std::wstring m_homepage;
    std::vector<std::wstring> m_url_list;
    BOOL m_secure;
    BOOL m_dont_r_click;
    BOOL m_local_file_access;
    BOOL m_dont_popup;
    DWORD m_emulation;
    std::vector<std::wstring> m_black_list;

    BOOL load();
    BOOL save();
    void reset();
};
extern SETTINGS g_settings;

void ShowSettingsDlg(HINSTANCE hInst, HWND hwnd, const std::wstring& strCurPage);

#endif  // ndef SETTINGS_HPP_
