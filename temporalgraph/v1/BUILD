load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    deps = [
        "//lib:fixed_size_array",
        "//temporalgraph/common",
        "//temporalgraph/common/graph:parser",
    ],
)
