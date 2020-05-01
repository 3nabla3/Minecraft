#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Hazel {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#ifndef HZ_DIST

// Core log macros
#define HZ_CORE_TRACE(...) ::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...)  ::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...)  ::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...) ::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_FATAL(...) HZ_CORE_ERROR(__VA_ARGS__); __debugbreak();

// Client log macros
#define HZ_TRACE(...)      ::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...)       ::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...)       ::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...)      ::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define HZ_FATAL(...)	   HZ_ERROR(__VA_ARGS__); __debugbreak();

#else
#define HZ_CORE_TRACE(...) 
#define HZ_CORE_INFO(...)  
#define HZ_CORE_WARN(...)  
#define HZ_CORE_ERROR(...) 
#define HZ_CORE_FATAL(...) 

// Client log macros
#define HZ_TRACE(...)      
#define HZ_INFO(...)       
#define HZ_WARN(...)       
#define HZ_ERROR(...)      
#define HZ_FATAL(...)      
#endif