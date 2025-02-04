//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-04 01:39:46
//

#pragma once

#define TO_SECONDS(Value) Value / 1000.0f

#include <Windows.h>

/// @brief A class for measuring elapsed time.
/// 
/// The `Timer` class provides functionality for measuring time intervals in milliseconds. 
/// It allows for getting the elapsed time and restarting the timer.
class Timer
{
public:
    /// @brief Constructs a Timer object.
    /// 
    /// Initializes the timer, setting up necessary resources to track elapsed time.
    Timer();

    /// @brief Gets the elapsed time.
    /// 
    /// This method returns the time elapsed since the timer was started or last restarted, 
    /// in milliseconds.
    /// 
    /// @return The elapsed time in milliseconds.
    float GetElapsed();

    /// @brief Restarts the timer.
    /// 
    /// This method resets the timer, starting a new measurement from the current point in time.
    void Restart();

private:
    /// @brief The frequency of the high-resolution timer (ticks per second).
    LARGE_INTEGER mFrequency;

    /// @brief The timestamp of when the timer was started.
    LARGE_INTEGER mStart;
};