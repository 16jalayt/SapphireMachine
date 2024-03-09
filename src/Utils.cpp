#include "Utils.h"
#include <Engine/Config.h>
#include <loguru.hpp>

void Utils::initLog(int argc, char** argv)
{
	Engine::Config::initLog(argc, argv);

#ifdef _DEBUG
	loguru::g_stderr_verbosity = loguru::Verbosity_MAX;
#else
	loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
#endif

	//Force to max because switch hard to debug
#ifdef __SWITCH__
	loguru::g_stderr_verbosity = loguru::Verbosity_MAX;
#endif

	if (Engine::Config::lograw)
	{
		loguru::g_preamble = false;
	}
	else
	{
		loguru::g_preamble_uptime = false;
		loguru::g_preamble_date = false;
		loguru::g_preamble_thread = true;
		loguru::g_preamble_verbose = false;
	}

	//init important for crash logging
	loguru::init(argc, argv);
	//Init sets to main thread by default
	loguru::set_thread_name("Main Thread");

	if (Engine::Config::logfile)
		loguru::add_file("game.log", loguru::Truncate, loguru::Verbosity_INFO);

	//Logging tests/examples
	//LOG_F(INFO, "I'm hungry for some %.3f!", 3.14159);
	//LOG_S(INFO) << "Some float: " << 3.14;
	//LOG_S(ERROR) << "My vec3: " << 3.14;
	//LOG_S(ERROR) << loguru::stacktrace(1).c_str();
	//tracetest();

	//LOG_SCOPE_FUNCTION(INFO);
	//VLOG_SCOPE_F(1, "Iteration %d", 5);
}