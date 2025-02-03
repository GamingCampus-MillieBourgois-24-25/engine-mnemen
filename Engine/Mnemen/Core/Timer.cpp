//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-04 01:40:00
//

#include <Core/Timer.hpp>

Timer::Timer()
{
    QueryPerformanceFrequency(&mFrequency);
    QueryPerformanceCounter(&mStart);
}

float Timer::GetElapsed()
{
    LARGE_INTEGER end;

    QueryPerformanceCounter(&end);

    return (end.QuadPart - mStart.QuadPart) * 1000.0 / mFrequency.QuadPart;
}

void Timer::Restart()
{
    QueryPerformanceCounter(&mStart);
}