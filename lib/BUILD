cc_library(
    name = "array",
    hdrs = [
        "Array.h",
        "FixedSizeArray.h",
    ],
    deps = [
        "@com_github_google_glog//:glog",
    ],
)

cc_test(
    name = "array_test",
    srcs = [
        "tests/ArrayTest.cpp",
    ],
    deps = [
        ":array",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
