#pragma once
#include <string>
namespace LG
{
    class Application
    {
    public:
        explicit Application(std::string appName = "Main App");
        virtual ~Application();
        void Run();

    private:
        void InitServices();
        void ShutdownServices();

    protected:
        virtual void Update(float deltaTime){};

    protected:
        std::string m_appName;
        bool m_running = true;
    };

} //namespace LG

extern LG::Application* CreateApplication();