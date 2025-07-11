add_library(thermostat_vendor INTERFACE)
target_include_directories(thermostat_vendor INTERFACE ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(tests EXCLUDE_FROM_ALL)
