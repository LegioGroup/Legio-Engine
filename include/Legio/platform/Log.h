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