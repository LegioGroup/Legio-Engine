#include <Legio/platform/Clock.h>

namespace LG
{
    Clock::Clock(float fixedTick)
    : m_fixedTick(fixedTick)
    , m_maxElapsedTime(1.f/10.f)
    , m_elapsedTime(0.0f)
    , m_totalTime(0.0f)
    , m_timeBetweenUpdates(0.0f)
    {
        QueryPerformanceFrequency(&m_frequency);
        QueryPerformanceCounter(&m_previousTime);
        QueryPerformanceCounter(&m_currentTime);
    }

    void Clock::InitUpdatesToProcess()
    {
        QueryPerformanceCounter(&m_currentTime);
        LARGE_INTEGER elapsed;
        elapsed.QuadPart = m_currentTime.QuadPart - m_previousTime.QuadPart;
        m_previousTime = m_currentTime;

        // Add time elapsed from previous process to remaining time.
        m_timeBetweenUpdates = static_cast<float>(elapsed.QuadPart) / static_cast<float>(m_frequency.QuadPart);
        m_elapsedTime += m_timeBetweenUpdates;

        //fix number of iterations
        if(m_elapsedTime > m_maxElapsedTime)
        {
            m_elapsedTime = m_maxElapsedTime;
        }
    }

    bool Clock::ProcessUpdates()
    {
        if(m_elapsedTime >= m_fixedTick)
        {
            m_totalTime += m_fixedTick;
            m_elapsedTime -= m_fixedTick;
            return true;
        }
        return false;
    }

} // namespace LG
