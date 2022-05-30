#pragma once
#include <Legio/ServiceLocator.h>

#define LG_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#ifndef NDEBUG
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
    #define LG_CORE_CRITICAL(...)
    #define LG_CORE_ERROR(...)
    #define LG_CORE_WARN(...)
    #define LG_CORE_INFO(...)
    #define LG_CORE_TRACE(...)

    #define LG_APP_CRITICAL(...)
    #define LG_APP_ERROR(...)
    #define LG_APP_WARN(...)
    #define LG_APP_INFO(...)
    #define LG_APP_TRACE(...)
#endif
