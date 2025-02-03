//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2025-02-03 12:23:11
//

#include "Editor.hpp"

int main()
{
    ApplicationSpecs specs;
    specs.Width = 1280;
    specs.Height = 720;
    specs.WindowTitle = "MNEMEN EDITOR";
    specs.GameName = "MnmEditor";

    Editor editor(specs);
    editor.Run();
}
