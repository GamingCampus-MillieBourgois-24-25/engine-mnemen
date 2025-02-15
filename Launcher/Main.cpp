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

    Launcher launcher(specs);
    launcher.Run();
}
