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

cc_test(
    name = "test",
    srcs = [
        "tests/EveLogTest.cpp",
    ],
    deps = [
        ":evelog",
        "@com_github_google_glog//:glog",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
