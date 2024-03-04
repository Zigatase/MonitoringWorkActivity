#include "utils.h"


bool Utils::AddToStartup()
{
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);

    const wchar_t* programPath = path;

    // 
    HKEY hKey;
    LONG result = RegCreateKeyExW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_WRITE,
        NULL,
        &hKey,
        NULL);

    if (result != ERROR_SUCCESS) {
        return false;
    }

    DWORD dwValueSize = (wcslen(programPath) + 1) * sizeof(wchar_t);
    result = RegSetValueExW(
        hKey,
        NULL,
        0,
        REG_SZ,
        (LPBYTE)programPath,
        dwValueSize);

    RegCloseKey(hKey);

    return (result == ERROR_SUCCESS);
}


bool Utils::RemoveFromStartup()
{
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);

    const wchar_t* programPath = path;

    //
    HKEY hKey;
    LONG result = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0,
        KEY_WRITE,
        &hKey);

    if (result != ERROR_SUCCESS) {
        return false;
    }

    result = RegDeleteValueW(hKey, NULL);

    RegCloseKey(hKey);

    return (result == ERROR_SUCCESS);
}