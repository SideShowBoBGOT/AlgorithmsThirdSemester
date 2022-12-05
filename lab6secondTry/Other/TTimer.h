//
// Created by choleraplague on 05.12.22.
//

#ifndef UNTITLED1_TTIMER_H
#define UNTITLED1_TTIMER_H

#include <chrono>

class TTimer {
    public:
    virtual void Start();
    virtual void Stop();
    virtual bool IsRunning();
    virtual double ElapsedMilliseconds();

    protected:
    std::chrono::time_point<std::chrono::system_clock> m_xStartTime;
    std::chrono::time_point<std::chrono::system_clock> m_xEndTime;
    bool                                               m_bRunning = false;
};


#endif //UNTITLED1_TTIMER_H
