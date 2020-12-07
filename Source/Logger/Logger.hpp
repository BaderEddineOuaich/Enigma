#pragma once
#ifndef ENIGMA_LOGGER_H
#define ENIGMA_LOGGER_H

#include <Core/Core.hpp>

//Logging Library
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/basic_file_sink.h>

NS_ENIGMA_BEGIN
class ENIGMA_API Logger
{
public:
	/*
	*	Initialize Enigma Logger
	*/
	static void Initialize();

public: /* Accessors */
	static const std::shared_ptr<spdlog::logger>& GetLogger() noexcept { return m_logger; }

private:
    static std::shared_ptr<spdlog::logger> m_logger;
};


/// Enigma Log Macros
#define ENIGMA_TRACE(...)				::Enigma::Logger::GetLogger()->trace(__VA_ARGS__)
#define ENIGMA_INFO(...)				::Enigma::Logger::GetLogger()->info(__VA_ARGS__)
#define ENIGMA_WARN(...)				::Enigma::Logger::GetLogger()->warn("{0} | {1}:{2} | {3}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)
#define ENIGMA_ERROR(...)				::Enigma::Logger::GetLogger()->error("{0} | {1}:{2} | {3}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)
#define ENIGMA_CRITICAL(...)			::Enigma::Logger::GetLogger()->critical("{0} | {1}:{2} | {3}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)

#define log_trace(...)				ENIGMA_TRACE(__VA_ARGS__)
#define log_info(...)				ENIGMA_INFO(__VA_ARGS__)
#define log_warn(...)				ENIGMA_WARN(__VA_ARGS__)
#define log_error(...)				ENIGMA_ERROR(__VA_ARGS__)
#define log_critical(...)			ENIGMA_CRITICAL(__VA_ARGS__)


#define LOG(...)					ENIGMA_TRACE(__VA_ARGS__)
#define LOG_TRACE(...)				ENIGMA_TRACE(__VA_ARGS__)
#define LOG_INFO(...)				ENIGMA_INFO(__VA_ARGS__)
#define LOG_WARN(...)				ENIGMA_WARN(__VA_ARGS__)
#define LOG_ERROR(...)				ENIGMA_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...)			ENIGMA_CRITICAL(__VA_ARGS__)

NS_ENIGMA_END

#endif // !ENIGMA_LOGGER_H
