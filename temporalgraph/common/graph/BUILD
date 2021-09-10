load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "graph",
    hdrs = [
        "AbstractGraph.h",
        "GraphUtils.h",
    ],
)

cc_library(
    name = "evelog",
    hdrs = [
        "EveLog.h",
        "EventList.h",
    ],
    deps = [
        ":graph",
        "//lib:array",
        "//lib:utils",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "edgelog",
    hdrs = [
        "EdgeList.h",
        "EdgeLog.h",
    ],
    deps = [
        ":graph",
        "//lib:array",
        "//lib:utils",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "cas",
    hdrs = [
        "CAS.h",
    ],
    deps = [
        ":graph",
        "//lib:bitvector",
        "//lib:wavelet_tree",
        "//temporalgraph/common",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "adjlist",
    hdrs = [
        "AdjacencyList.h",
    ],
    deps = [
        ":graph",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "parser",
    hdrs = [
        "GraphParser.h",
    ],
    deps = [
        ":cas",
        ":edgelog",
        ":evelog",
        ":graph",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "test_utils",
    hdrs = [
        "tests/TestUtils.h",
    ],
    deps = [
        ":graph",
        "@com_github_google_glog//:glog",
    ],
)

cc_test(
    name = "cas_test",
    srcs = [
        "tests/CASTest.cpp",
    ],
    deps = [
        ":cas",
        ":test_utils",
        "//temporalgraph/common",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "evelog_test",
    srcs = [
        "tests/EveLogTest.cpp",
    ],
    deps = [
        ":evelog",
        ":graph",
        ":parser",
        ":test_utils",
        "//temporalgraph/common",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "edgelog_test",
    srcs = [
        "tests/EdgeLogTest.cpp",
    ],
    deps = [
        ":edgelog",
        ":graph",
        ":parser",
        ":test_utils",
        "//temporalgraph/common",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "adjlist_test",
    srcs = [
        "tests/AdjacencyListTest.cpp",
    ],
    deps = [
        ":adjlist",
        ":graph",
        ":test_utils",
        "//temporalgraph/common",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "tests/AdjacencyListTest.cpp",
        "tests/CASTest.cpp",
        "tests/EdgeLogTest.cpp",
        "tests/EveLogTest.cpp",
    ],
    deps = [
        ":adjlist_test",
        ":cas",
        ":edgelog",
        ":evelog",
        ":parser",
        "//temporalgraph/common",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
