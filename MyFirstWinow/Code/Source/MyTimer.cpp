#include "../Include/MyTimer.h"

using namespace std::chrono;

MyTimer::MyTimer()
{
    m_startTime = steady_clock::now();
}

float MyTimer::Mark()
{
    const auto m_endTime = m_startTime;
    m_startTime = steady_clock::now();
    const duration<float> timeSpan = m_endTime - m_startTime;
    return timeSpan.count();
}

float MyTimer::Peek() const
{
    return duration<float>(steady_clock::now() - m_startTime).count();
}
