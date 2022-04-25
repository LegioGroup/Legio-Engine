#pragma once
#include <memory>
#include <Legio/platform/Window.h>
#include <Legio/platform/Log.h>


#define LG_PROVIDE_FN(Class, sourcePtr, targetPtr)          \
static inline void Provide(Class* sourcePtr)                \
{                                                           \
    if(targetPtr != nullptr)                                \
    {                                                       \
        return;                                             \
    }                                                       \
    targetPtr = std::unique_ptr<Class>(sourcePtr);          \
}                                                           \

namespace LG
{
    class ServiceLocator
    {
    public:
        static inline const std::unique_ptr<Window>& GetWindow() { return m_window; };
        static inline const std::unique_ptr<Log>& GetLogger(){ return m_log;};
        
        LG_PROVIDE_FN(Window, window, m_window);
        LG_PROVIDE_FN(Log, log, m_log);

    private:
        static inline std::unique_ptr<Window> m_window = nullptr;
        static inline std::unique_ptr<Log> m_log = nullptr;
    };

} //namespace LG
