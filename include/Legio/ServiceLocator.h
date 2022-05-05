#pragma once
#include <memory>
#include <any>
#include <Legio/platform/Window.h>
#include <Legio/platform/Log.h>
#include <Legio/rendering/Renderer.h>


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
        static inline const std::unique_ptr<Log>& GetLogger(){ return m_log; };
        static inline const std::unique_ptr<Renderer>& GetRenderer(){ return m_renderer; };

        LG_PROVIDE_FN(Window, window, m_window);
        LG_PROVIDE_FN(Log, log, m_log);
        LG_PROVIDE_FN(Renderer, renderer, m_renderer);

        static inline void ShutdownServices()
        {
            m_window.reset();
            m_log.reset();
            m_renderer.reset();
        }

    private:
        static inline std::unique_ptr<Window> m_window = nullptr;
        static inline std::unique_ptr<Log> m_log = nullptr;
        static inline std::unique_ptr<Renderer> m_renderer = nullptr;
    };

} //namespace LG
