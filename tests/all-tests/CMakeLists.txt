project(all_tests)

################################
# Unit Tests
################################
# Add test cpp file
add_executable(${PROJECT_NAME} test.cpp)

# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME} gtest 
  gtest_main 
  log-event-finder-lib
  clangTooling
  clangBasic
  clangASTMatchers
  clangAST
  clangFrontend
  clangSerialization)

set(INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/logEventFinder")
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
#add_test(${PROJECT_NAME} all_tests)
