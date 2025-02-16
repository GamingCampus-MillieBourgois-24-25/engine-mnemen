//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:23:11
//

#include "Launcher.hpp"

int main()
{
    ApplicationSpecs specs;
    specs.Width = 1280;
    specs.Height = 720;
    specs.WindowTitle = "Mnemen Launcher";
    specs.CopyToBackBuffer = true;

    Launcher* launcher = new Launcher(specs);
    launcher->Run();
    String projectDir = launcher->SelectedProject;
    delete launcher;

    // Launch editor
    if (projectDir.empty())
        return 0;
    
    String workDir = projectDir;
    size_t pos = workDir.find_last_of('/');
    if (pos != std::string::npos) {
        workDir = workDir.substr(0, pos);
    }
    const char* exePath = "Editor.exe";
    char commandLine[1024];
    snprintf(commandLine, sizeof(commandLine), "\"%s\" %s", exePath, projectDir.c_str());
    STARTUPINFOA si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;
    if (FAILED(CreateProcessA(
        exePath,             // Application name (executable path)
        commandLine,         // Command line (exePath + arguments)
        NULL,                // Process handle not inheritable
        NULL,                // Thread handle not inheritable
        FALSE,               // Set handle inheritance to FALSE
        0,
        NULL,                // Use parent's environment block
        workDir.c_str(),     // Set working directory
        &si,                 // Pointer to STARTUPINFO
        &pi                  // Pointer to PROCESS_INFORMATION
    ))) {
        LOG_ERROR("Failed to launch editor!");
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
