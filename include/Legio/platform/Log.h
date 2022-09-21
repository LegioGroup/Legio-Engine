#pragma once

#include <memory>
#include <spdlog/spdlog.h>
namespace LG
{
    class Log
    {
    public:
        ~Log();
        void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; };
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; };

    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;

    };
} //namespace LG

#ifdef LG_DEBUG
    #define LG_CORE_CRITICAL(...)   ::LG::Log::GetCoreLogger()->critical(__VA_ARGS__)
    #define LG_CORE_ERROR(...)      ::LG::Log::GetCoreLogger()->error(__VA_ARGS__)
    #define LG_CORE_WARN(...)       ::LG::Log::GetCoreLogger()->warn(__VA_ARGS__)
    #define LG_CORE_INFO(...)       ::LG::Log::GetCoreLogger()->info(__VA_ARGS__)
    #define LG_CORE_TRACE(...)      ::LG::Log::GetCoreLogger()->trace(__VA_ARGS__)

    #define LG_APP_CRITICAL(...)   ::LG::Log::GetClientLogger()->critical(__VA_ARGS__)
    #define LG_APP_ERROR(...)      ::LG::Log::GetClientLogger()->error(__VA_ARGS__)
    #define LG_APP_WARN(...)       ::LG::Log::GetClientLogger()->warn(__VA_ARGS__)
    #define LG_APP_INFO(...)       ::LG::Log::GetClientLogger()->info(__VA_ARGS__)
    #define LG_APP_TRACE(...)      ::LG::Log::GetClientLogger()->trace(__VA_ARGS__)
#else
    #define LG_CORE_CRITICAL(...) do{}while(false)
    #define LG_CORE_ERROR(...) do{}while(false)
    #define LG_CORE_WARN(...) do{}while(false)
    #define LG_CORE_INFO(...) do{}while(false)
    #define LG_CORE_TRACE(...) do{}while(false)

    #define LG_APP_CRITICAL(...) do{}while(false)
    #define LG_APP_ERROR(...) do{}while(false)
    #define LG_APP_WARN(...) do{}while(false)
    #define LG_APP_INFO(...) do{}while(false)
    #define LG_APP_TRACE(...) do{}while(false)
#endif