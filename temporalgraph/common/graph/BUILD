load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "graph",
    hdrs = [
        "AdjacencyList.h",
        "AdjacencyMatrix.h",
        "Edge.h",
        "Graph.h",
        "Utils.h",
        "Vertex.h",
    ],
)

cc_library(
    name = "evelog",
    hdrs = [
        "EveLog.h",
        "EventList.h",
        "GraphInterface.h",
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
        "EdgeList.h",
        "EdgeLog.h",
        "GraphInterface.h",
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
        "CAS.h",
        "GraphInterface.h",
        "GraphUtils.h",
    ],
    deps = [
        "//lib:bitvector",
        "//lib:wavelet_tree",
        "@com_github_google_glog//:glog",
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
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
