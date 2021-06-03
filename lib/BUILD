package(default_visibility = ["//visibility:public"])

cc_library(
    name = "fixed_size_array",
    hdrs = [
        "Array.h",
        "BitArray.h",
        "FixedSizeArray.h",
    ],
    deps = [
        ":bitmask_utils",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "variable_size_array",
    hdrs = [
        "VariableSizeArray.h",
        "VariableSizeDenseArray.h",
    ],
    deps = [
        ":fixed_size_array",
        ":utils",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "array",
    deps = [
        ":fixed_size_array",
        ":variable_size_array",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "bitmask_utils",
    hdrs = [
        "utils/BitmaskUtility.h",
    ],
    deps = [
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "bitvector",
    hdrs = [
        "BitVector.h",
        "ClarkSelect.h",
        "JacobsonRank.h",
    ],
    deps = [
        ":fixed_size_array",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "utils",
    hdrs = [
        "HuffmanTree.h",
        "utils/DeltaGapUtility.h",
        "utils/DensePointersUtility.h",
        "utils/GamaUtility.h",
        "utils/HuffmanUtility.h",
        "utils/Utils.h",
    ],
    deps = [
        ":bitmask_utils",
        ":fixed_size_array",
        ":heap",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "heap",
    hdrs = [
        "Heap.h",
    ],
    deps = [
        ":fixed_size_array",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "wavelet_tree",
    hdrs = [
        "WaveletTree.h",
    ],
    deps = [
        ":array",
        "@com_github_google_glog//:glog",
    ],
)

cc_test(
    name = "array_test",
    srcs = [
        "tests/ArrayTest.cpp",
    ],
    deps = [
        ":fixed_size_array",
        ":utils",
        ":variable_size_array",
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
        ":variable_size_array",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "heap_test",
    srcs = [
        "tests/HeapTest.cpp",
    ],
    deps = [
        ":heap",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "bitvector_test",
    srcs = [
        "tests/BitVectorTest.cpp",
    ],
    deps = [
        ":array",
        ":bitvector",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "wavelet_tree_test",
    srcs = [
        "tests/WaveletTreeTest.cpp",
    ],
    deps = [
        ":wavelet_tree",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
