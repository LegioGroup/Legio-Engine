#pragma once
#include <memory>
#include <Legio/platform/Window.h>
namespace LG
{
    class ServiceLocator
    {
    public:
        static inline const std::unique_ptr<Window>& GetWindow() { return m_window; };
        static inline void Proivide(Window* window)
        {
            if(m_window != nullptr)
            {
                return;
            }
            m_window = std::unique_ptr<Window>(window);
        }
    
    private:
        static inline std::unique_ptr<Window> m_window = nullptr;
    };

} //namespace LG
