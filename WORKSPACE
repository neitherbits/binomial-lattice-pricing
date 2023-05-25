load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

##
## Toolchain
##
git_repository(
    name = "com_grailbio_bazel_toolchain",
    commit = "9e71d562023dc7994e747110ee1ca345ad6b4413",  # Latest as of 2022-05-01
    remote = "https://github.com/grailbio/bazel-toolchain.git",
)

load("@com_grailbio_bazel_toolchain//toolchain:deps.bzl", "bazel_toolchain_dependencies")

bazel_toolchain_dependencies()

load("@com_grailbio_bazel_toolchain//toolchain:rules.bzl", "llvm_toolchain")

llvm_toolchain(
    name = "llvm_toolchain",
    llvm_version = "14.0.0",
    stdlib = {
        "linux-x86_64": "builtin-libc++",
        "linux-aarch64": "builtin-libc++",
    },
)

load("@llvm_toolchain//:toolchains.bzl", "llvm_register_toolchains")

llvm_register_toolchains()

##
## absl, use at head since it is stable and follows the library's guidelines
##
git_repository(
    name = "com_google_absl",
    commit = "c3e9485bdc0556d2cb454ac106786170e3c95e8a",  # 2023-05-24
    remote = "https://github.com/abseil/abseil-cpp",
)

##
## gtest and dependencies, based on the contents of googletest/WORKSPACE
##
git_repository(
    name = "com_google_gtest",
    commit = "45804691223635953f311cf31a10c632553bbfc3",  # 2023-05-24
    remote = "https://github.com/google/googletest",
)

# Note this must use a commit from the `abseil` branch of the RE2 project.
# https://github.com/google/re2/tree/abseil
http_archive(
    name = "com_googlesource_code_re2",
    sha256 = "cb8b5312a65f2598954545a76e8bce913f35fbb3a21a5c88797a4448e9f9b9d9",
    strip_prefix = "re2-578843a516fd1da7084ae46209a75f3613b6065e",
    urls = ["https://github.com/google/re2/archive/578843a516fd1da7084ae46209a75f3613b6065e.zip"],  # 2023-05-24
)

git_repository(
    name = "bazel_skylib",
    remote = "https://github.com/bazelbuild/bazel-skylib",
    tag = "1.4.1",  # Latest as of 2023-05-24
)
