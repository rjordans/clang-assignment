set(LLVM_LINK_COMPONENTS support)

add_clang_executable(log-event-finder
  main.cpp
  logEventFinder.cpp
  )

add_library(log-event-finder-lib
  logEventFinder.cpp
  )
  
target_link_libraries(log-event-finder
  PRIVATE
  clangTooling
  clangBasic
  clangASTMatchers
  clangAST
  clangFrontend
  clangSerialization
  )