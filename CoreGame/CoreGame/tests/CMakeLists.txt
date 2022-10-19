add_executable(test_Timer test_Timer.cpp)
target_include_directories(test_Timer PRIVATE ../include)
add_test(TestTimer test_Timer)
