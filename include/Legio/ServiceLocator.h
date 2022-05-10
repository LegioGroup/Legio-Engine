#pragma once
#include <memory>
#include <any>
#include <Legio/application/Events/Event.h>
#include <Legio/platform/Log.h>
#include <Legio/platform/Window.h>
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Input.h>


#define LG_PROVIDE_FN(Class, sourcePtr, targetPtr)             \
static inline void Provide(Class* sourcePtr)                   \
{                                                              \
    if(targetPtr != nullptr)                                   \
    {                                                          \
        return;                                                \
    }                                                          \
    targetPtr = std::unique_ptr<Class>(sourcePtr);             \
}                                                              \
static inline Class* Get##Class##() { return targetPtr.get(); }\

namespace LG
{
    class ServiceLocator
    {
    public:

        LG_PROVIDE_FN(Log, log, m_log);
        LG_PROVIDE_FN(Window, window, m_window);
        LG_PROVIDE_FN(Input, input, m_input);
        LG_PROVIDE_FN(Renderer, renderer, m_renderer);

        static inline void ShutdownServices()
        {
            m_renderer.reset();
            m_input.reset();
            m_window.reset();
            m_log.reset();
        }

    private:

        static inline std::unique_ptr<Log> m_log = nullptr;
        static inline std::unique_ptr<Window> m_window = nullptr;
        static inline std::unique_ptr<Input> m_input = nullptr;
        static inline std::unique_ptr<Renderer> m_renderer = nullptr;
    };

} //namespace LG
