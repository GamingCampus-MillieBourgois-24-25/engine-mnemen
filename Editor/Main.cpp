//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:23:11
//

#include "Editor.hpp"

int main(int argc, char *argv[])
{
    ApplicationSpecs specs;
    specs.Width = 1920;
    specs.Height = 1080;
    specs.WindowTitle = "Mnemen Editor";
    specs.ProjectPath = "TestGame.mpj";
    specs.CopyToBackBuffer = false;

    if (argc == 2) {
        specs.ProjectPath = argv[1];
    }

    Editor editor(specs);
    editor.Run();
}
