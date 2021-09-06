load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    deps = [
        "//temporalgraph/common:experiments",
        "//temporalgraph/common/graph:evelog",
        "//temporalgraph/common/graph:parser",
        "@com_github_gflags_gflags//:gflags",
    ],
)
