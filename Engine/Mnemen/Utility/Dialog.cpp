//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-11 21:42:07
//

#include "Dialog.hpp"

#include <Core/UTF.hpp>

#include <windows.h>
#include <commdlg.h>
#include <shlwapi.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <shlobj.h>
#include <atlbase.h>

String BuildFilterString(const Vector<String>& extensions)
{
    String filter;
    for (const auto& ext : extensions) {
        filter += ext.substr(1) + " Files (*" + ext + ")\0*" + ext + "\0";
    }
    filter += "All Files (*.*)\0*.*\0";
    filter += '\0';
    return filter;
}

String Dialog::Open(const Vector<String>& extensions)
{
    String filter = BuildFilterString(extensions);
    char filePath[MAX_PATH] = {0};

    char initialDir[MAX_PATH] = {0};
    GetCurrentDirectoryA(MAX_PATH, initialDir);
    
    char assetsDir[MAX_PATH] = {0};
    GetCurrentDirectoryA(MAX_PATH, assetsDir);
    snprintf(assetsDir, MAX_PATH, "%s/Assets", assetsDir);

    OPENFILENAMEA ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = filter.c_str();
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = "Select a File";
    ofn.lpstrInitialDir = assetsDir;

    if (GetOpenFileNameA(&ofn)) {
        SetCurrentDirectoryA(initialDir);
        
        // Calculate relative path based on initialDir
        String result(filePath);
        size_t len = strlen(initialDir);
        if (result.find(initialDir) == 0) {
            result = result.substr(len + 1); // Remove the initialDir part
        }
        std::replace(result.begin(), result.end(), '\\', '/');
        return result;
    }
    SetCurrentDirectoryA(initialDir);
    return "";
}

String Dialog::Save(const Vector<String>& extensions)
{
    String filter = BuildFilterString(extensions);
    char filePath[MAX_PATH] = {0};

    char initialDir[MAX_PATH] = {0};
    GetCurrentDirectoryA(MAX_PATH, initialDir);
    
    char assetsDir[MAX_PATH] = {0};
    GetCurrentDirectoryA(MAX_PATH, assetsDir);
    snprintf(assetsDir, MAX_PATH, "%s/Assets", assetsDir);

    OPENFILENAMEA ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = filter.c_str();
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    ofn.lpstrTitle = "Save File As";
    ofn.lpstrInitialDir = assetsDir;

    if (GetSaveFileNameA(&ofn)) {
        SetCurrentDirectoryA(initialDir);
        
        // Calculate relative path based on initialDir
        String result(filePath);
        size_t len = strlen(initialDir);
        if (result.find(initialDir) == 0) {
            result = result.substr(len + 1); // Remove the initialDir part
        }
        std::replace(result.begin(), result.end(), '\\', '/');
        return result;
    }
    SetCurrentDirectoryA(initialDir);
    return "";
}

String Dialog::OpenFolder()
{
    // Create the FileOpenDialog object
    CComPtr<IFileOpenDialog> pFileOpenDialog;

    // Initialize COM library
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    // Create the file dialog
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileOpenDialog));
    if (FAILED(hr)) {
        std::cout << "Failed to create the FileOpenDialog instance!" << std::endl;
        CoUninitialize();
        return "";
    }

    // Set the dialog to pick a folder by using a custom filter (folder picker mode)
    DWORD dwOptions;
    pFileOpenDialog->GetOptions(&dwOptions);
    pFileOpenDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

    // Show the dialog
    hr = pFileOpenDialog->Show(NULL);
    if (FAILED(hr)) {
        std::cout << "Dialog failed or was canceled" << std::endl;
        CoUninitialize();
        return "";
    }

    // Retrieve the selected folder
    CComPtr<IShellItem> pItem;
    hr = pFileOpenDialog->GetResult(&pItem);
    if (FAILED(hr)) {
        std::cout << "Failed to retrieve the folder" << std::endl;
        CoUninitialize();
        return "";
    }

    // Get the folder path from the selected item
    PWSTR pszPath = NULL;
    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
    if (FAILED(hr)) {
        std::cout << "Failed to get the folder path" << std::endl;
        CoUninitialize();
        return "";
    }

    // Convert the path to a String
    WideString wstr(pszPath);
    String folderPath = UTF::WideToAscii(wstr);

    // Clean up and release COM resources
    CoTaskMemFree(pszPath);
    CoUninitialize();

    // Normalize backslashes to forward slashes
    std::replace(folderPath.begin(), folderPath.end(), '\\', '/');

    return folderPath;
}
