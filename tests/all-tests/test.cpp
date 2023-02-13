#include <gtest/gtest.h>
#include "logEventFinder.hpp"
#include <filesystem>

#include <filesystem>
#include <iostream>

TEST(LOG_FINDER, test1) {
	auto cwd = std::filesystem::current_path();
	auto cwd_s = cwd.generic_string();

	auto executable = cwd / ".."/".."/"logEventFinder"/"log-event-finder";
	auto cpp = cwd /".."/".."/".."/"applications"/"AFakeLoggingFramework"/"AFakeLoggingFramework.cpp";

	const char* argv[6] = {
		(executable).c_str(),
		(cpp).c_str(),
		"--extra-arg-before=-I/usr/include/",
		"--extra-arg-before=-I/usr/include/c++/11",
		"--extra-arg-before=-I/usr/include/x86_64-linux-gnu/c++/11",
		"--" };

	auto obtained = logfinder::tool_call(6, argv);
	EXPECT_EQ(0, obtained);
}

int main(int argc, char* argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
