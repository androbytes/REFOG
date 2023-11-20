#pragma once
#include "spdlog/logger.h"
#include <memory>

namespace REFOG {
	class Logger {
	private:
		static std::shared_ptr<spdlog::logger> m_REFOGLogger;
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger> GetLogger();
	};
}

#define REFOG_TRACE(...) REFOG::Logger::GetLogger()->trace(__VA_ARGS__)
#define REFOG_INFO(...) REFOG::Logger::GetLogger()->info(__VA_ARGS__)
#define REFOG_WARN(...) REFOG::Logger::GetLogger()->warn(__VA_ARGS__)
#define REFOG_ERROR(...) REFOG::Logger::GetLogger()->error(__VA_ARGS__)
#define REFOG_CRITICAL(...) REFOG::Logger::GetLogger()->critical(__VA_ARGS__)