#include <gtest/gtest.h>
#include "logEventFinder.hpp"

#include <filesystem>
#include <iostream>

TEST(LOG_FINDER, test1) {
	auto cwd = std::filesystem::current_path();
	auto cwd_s = cwd.generic_string();

	auto executable = cwd_s + "\\..\\..\\log-event-finder\\Debug\\log-event-finder.exe";
	auto cpp = cwd_s + "\\..\\..\\..\\applications\\AFakeLoggingFramework\\AFakeLoggingFramework.cpp";

	const char* argv[4] = {
		(executable).c_str(),
		(cpp).c_str(),
		"--extra-arg-before=-IC:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.19041.0\\ucrt",
		"--" };

	auto obtained = logfinder::tool_call(4, argv);
	EXPECT_EQ(0, obtained);
}

int main(int argc, char* argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
