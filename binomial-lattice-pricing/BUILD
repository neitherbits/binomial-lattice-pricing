load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load("//build:rules.bzl", "cc_fuzzer")

cc_library(
    name = "binomial_lattice_pricing",
    srcs = [
        "model.cc",
        "node.cc",
    ],
    hdrs = [
        "model.h",
        "node.h",
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

cc_fuzzer(
    name = "binomial_lattice_pricing_fuzzer",
    srcs = [
        "tests/sample_fuzzer.cc",
    ],
    corpus = "tests/sample_corpus",
    deps = [
        ":binomial_lattice_pricing",
    ],
)
