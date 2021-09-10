load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "common",
    hdrs = [
        "TimeCounter.h",
        "Utils.h",
    ],
)

cc_library(
    name = "experiments",
    hdrs = [
        "CommandLineFlags.h",
        "RssMonitor.h",
        "TestRunner.h",
        "TestSummary.h",
    ],
    deps = [
        ":common",
        "//temporalgraph/common/graph",
        "@com_github_gflags_gflags//:gflags",
    ],
)
