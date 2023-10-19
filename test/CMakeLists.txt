
add_executable(test_simple_config test_simple_config.cpp)
target_include_directories(test_simple_config
        PRIVATE
        ${SIMPLE_CONFIG_INCLUDE}
)

target_link_libraries(test_simple_config PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_simple_config PRIVATE simple_config)
