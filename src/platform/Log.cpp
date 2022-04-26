#include <Legio/platform/Log.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace LG
{
    std::shared_ptr<spdlog::logger> Log::s_coreLogger;
    std::shared_ptr<spdlog::logger> Log::s_clientLogger;

    Log::~Log()
    {
        LG_CORE_INFO("Shutting Down Logger");
    }

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        s_coreLogger = spdlog::stdout_color_mt("LEGIO");
        s_coreLogger->set_level(spdlog::level::trace);

        s_clientLogger = spdlog::stdout_color_mt("APP");
        s_clientLogger->set_level(spdlog::level::trace);

    }

} //namespace LG