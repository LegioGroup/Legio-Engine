#pragma once
#include <memory>
#include <any>
#include <Legio/application/Events/Event.h>
#include <Legio/platform/Log.h>
#include <Legio/platform/Window.h>
#include <Legio/rendering/Renderer.h>
#include <Legio/platform/Input.h>
#include <Legio/editor/Editor.h>


#define LG_PROVIDE_FN(Class)                                                         \
static inline void Provide(Class* ptr)                                               \
{                                                                                    \
    if(s_##Class## != nullptr)                                                       \
    {                                                                                \
        return;                                                                      \
    }                                                                                \
    s_##Class## = std::unique_ptr<Class>(ptr);                                       \
}                                                                                    \
static inline Class* Get##Class##() { return s_##Class##.get(); }                    \
                                                                                     \
private: static inline std::unique_ptr<Class> s_##Class## = nullptr;  public:        \


namespace LG
{
    class ServiceLocator
    {
    public:

        LG_PROVIDE_FN(Log);
        LG_PROVIDE_FN(Window);
        LG_PROVIDE_FN(Input);
        LG_PROVIDE_FN(Renderer);
        LG_PROVIDE_FN(Editor);

        static inline void ShutdownServices()
        {
            s_Renderer->Shutdown();
            s_Input.reset();
            s_Window.reset();
            s_Log.reset();
            s_Editor.reset();
        }
    };

} //namespace LG
