load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "binomial_lattice_pricing",
    srcs = [
        "model.cc",
        "recomb_tree.cc",
    ],
    hdrs = [
        "Node.h",
        "model.h",
        "recomb_tree.h",
    ],
)

cc_test(
    name = "binomial_lattice_pricing_tests",
    srcs = [
        "tests/sample_tests.cc",
    ],
    deps = [
        ":binomial_lattice_pricing",
        "@com_google_gtest//:gtest_main",
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        ":binomial_lattice_pricing",
    ],
)
