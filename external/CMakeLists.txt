# jsoncpp import

include(FetchContent)

FetchContent_Declare(
    googletest
    set(GTEST_CREATE_SHARED_LIBRARY 0)
    set(GTEST_LINKED_AS_SHARED_LIBRARY 0)
    URL $ENV{GOOGLE_TEST_ARCHIVE_PATH}
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    jsoncpp
    set(JSONCPP_WITH_TESTS OFF)
    set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF)
    set(JSONCPP_WITH_WARNING_AS_ERROR OFF)
    set(JSONCPP_WITH_STRICT_ISO OFF)
    set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF)
    set(JSONCPP_WITH_CMAKE_PACKAGE OFF)
    set(JSONCPP_WITH_EXAMPLE OFF)
    set(BUILD_SHARED_LIBS OFF)
    set(BUILD_OBJECT_LIBS OFF)
    set(BUILD_STATIC_LIBS ON)
    URL $ENV{JSONCPP_ARCHIVE_PATH}
)
FetchContent_MakeAvailable(jsoncpp)