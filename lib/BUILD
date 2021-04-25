package(default_visibility = ["//visibility:public"])

cc_library(
    name = "array",
    hdrs = [
        "Array.h",
        "FixedSizeArray.h",
    ],
    deps = [
        ":utils",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "utils",
    hdrs = [
        "Array.h",
        "BitmaskUtility.h",
        "FixedSizeArray.h",
        "GamaUtility.h",
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

cc_test(
    name = "utils_test",
    srcs = [
        "tests/UtilitiesTest.cpp",
    ],
    deps = [
        ":utils",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
