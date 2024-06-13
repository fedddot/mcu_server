FetchContent_Declare(
    task_engine
    GIT_REPOSITORY  https://github.com/fedddot/task_engine.git
    GIT_TAG         main
)
FetchContent_GetProperties(task_engine)
if(NOT task_engine_POPULATED)
	FetchContent_Populate(task_engine)
	add_subdirectory(${task_engine_SOURCE_DIR} ${task_engine_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()