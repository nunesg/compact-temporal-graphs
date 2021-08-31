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
        "AbstractGraph.h",
        "EveLog.h",
        "EventList.h",
        "GraphUtils.h",
    ],
    deps = [
        "//lib:array",
        "//lib:utils",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "edgelog",
    hdrs = [
        "AbstractGraph.h",
        "EdgeList.h",
        "EdgeLog.h",
        "GraphUtils.h",
    ],
    deps = [
        "//lib:array",
        "//lib:utils",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "cas",
    hdrs = [
        "AbstractGraph.h",
        "CAS.h",
        "GraphUtils.h",
    ],
    deps = [
        "//lib:bitvector",
        "//lib:wavelet_tree",
        "//temporalgraph/common",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "parser",
    hdrs = [
        "GraphParser.h",
        "GraphUtils.h",
    ],
    deps = [
        ":cas",
        ":edgelog",
        ":evelog",
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
        "//temporalgraph/common",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "tests/CASTest.cpp",
        "tests/EdgeLogTest.cpp",
        "tests/EveLogTest.cpp",
    ],
    deps = [
        ":cas",
        ":edgelog",
        ":evelog",
        "//temporalgraph/common",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
