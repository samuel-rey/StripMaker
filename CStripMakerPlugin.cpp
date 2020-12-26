#include "pch.h"
#include "CStripMakerPlugin.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "loadSettings.h"

std::shared_ptr<spdlog::logger> logger;

CStripMakerPlugIn::CStripMakerPlugIn(void) :CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, MY_PLUGIN_NAME, MY_PLUGIN_VERSION, MY_PLUGIN_DEVELOPER, MY_PLUGIN_COPYRIGHT)
{
	auto logger = spdlog::basic_logger_mt("StripMaker", "StripMaker/logs/log.txt");
	logger->info("StripMaker plugin starting up...");
	//loadSettings();

}