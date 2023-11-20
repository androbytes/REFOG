#include "REFOG/Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace REFOG {
	std::shared_ptr<spdlog::logger> Logger::m_REFOGLogger;

	void Logger::Init() {
		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		sinks[0]->set_pattern("%^[%T] %n: %v%$");

		m_REFOGLogger = std::make_shared<spdlog::logger>("REFOG", sinks.begin(), sinks.end());
		m_REFOGLogger->set_level(spdlog::level::trace);
		m_REFOGLogger->flush_on(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger> Logger::GetLogger() {
		return m_REFOGLogger;
	}
}