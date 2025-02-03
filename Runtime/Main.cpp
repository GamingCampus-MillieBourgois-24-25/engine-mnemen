//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2025-02-03 12:23:11
//

#include "Runtime.hpp"

int main()
{
    ApplicationSpecs specs;
    specs.Width = 1280;
    specs.Height = 720;
    specs.WindowTitle = "Game Demo";
    specs.GameName = "DEMO";

    Runtime runtime(specs);
    runtime.Run();
}
