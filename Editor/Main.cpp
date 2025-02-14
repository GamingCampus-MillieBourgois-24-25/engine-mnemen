//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:23:11
//

#include "Editor.hpp"

int main()
{
    ApplicationSpecs specs;
    specs.Width = 1920;
    specs.Height = 1080;
    specs.WindowTitle = "MNEMEN | <D3D12>";
    specs.GameName = "MnmEditor";
    specs.StartScene = "";
    specs.CopyToBackBuffer = false;

    Editor editor(specs);
    editor.Run();
}
