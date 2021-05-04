package(default_visibility = ["//visibility:public"])

cc_library(
    name = "array",
    hdrs = [
        "Array.h",
        "FixedSizeArray.h",
        "VariableSizeArray.h",
        "VariableSizeDenseArray.h",
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
        "FixedSizeArray.h",
        "utils/BitmaskUtility.h",
        "utils/DensePointersUtility.h",
        "utils/GamaUtility.h",
        "utils/Utils.h",
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
        ":array",
        ":utils",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
