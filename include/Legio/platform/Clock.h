#pragma once
#include <windows.h>

namespace LG
{
    class Clock
    {
    public:
        Clock(float fixedTick);

        void InitUpdatesToProcess();

        bool ProcessUpdates();

        inline void SetMaxElapsedTime(double elapsedTime) { m_maxElapsedTime = static_cast<float>(elapsedTime); }
        inline float GetFixedTick() const { return m_fixedTick; }
        inline float GetTotalTime() const { return m_totalTime; }
        inline float GetTimeBetwUpdates() const { return m_timeBetweenUpdates; }
        inline float GetElapsedTime() const { return m_elapsedTime; }

    private:
        const float m_fixedTick;
        float m_maxElapsedTime;
        LARGE_INTEGER m_frequency;
        LARGE_INTEGER m_previousTime;
        LARGE_INTEGER m_currentTime;

        float m_elapsedTime;
        float m_totalTime;
        float m_timeBetweenUpdates;
    };
} // namespace LG
