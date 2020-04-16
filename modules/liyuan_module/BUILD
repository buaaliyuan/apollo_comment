load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "libcommon_component_example_1.so",
    linkopts = ["-shared"],
    linkstatic = False,
    deps = [":common_component_example_lib", ":hippo_example_lib"],
)

cc_library(
    name = "common_component_example_lib",
    srcs = ["common_component_example_1.cc"],
    hdrs = ["common_component_example_1.h"],
    deps = [
        "//cyber",
        "//cyber/examples/proto:examples_cc_proto",
    ],
)

cc_binary(
    name = "libhippo_example.so",
    linkopts = ["-shared"],
    linkstatic = False,
    deps = [":hippo_example_lib"],
)

cc_library(
    name = "hippo_example_lib",
    srcs = ["hippo_example.cc"],
    hdrs = ["hippo_example.h"],
    deps = [
        "//cyber",
        "//modules/liyuan_module/proto:test_cc_proto"
    ],
)

cpplint()