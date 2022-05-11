#pragma once
#include <string>
#include <Legio/platform/Clock.h>
#include <Legio/application/Events/Event.h>
namespace LG
{
    class Application
    {
    public:
        explicit Application(std::string appName = "Main App");
        virtual ~Application();
        void Run();
        void OnEvent(Event& event);
    private:
        bool OnWindowCloseEvent(Event& event);
        void InitServices();
        void ShutdownServices();
    protected:
        virtual void Update(float deltaTime){};

    protected:
        std::string m_appName;
        bool m_running = true;
        Clock m_clock {1.0f / 60.0f};
    };

} //namespace LG

extern LG::Application* CreateApplication();