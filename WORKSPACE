load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

##
## Toolchain
##
BAZEL_TOOLCHAIN_TAG = "0.8.2"

BAZEL_TOOLCHAIN_SHA = "0fc3a2b0c9c929920f4bed8f2b446a8274cad41f5ee823fd3faa0d7641f20db0"

http_archive(
    name = "com_grail_bazel_toolchain",
    canonical_id = BAZEL_TOOLCHAIN_TAG,
    sha256 = BAZEL_TOOLCHAIN_SHA,
    strip_prefix = "bazel-toolchain-{tag}".format(tag = BAZEL_TOOLCHAIN_TAG),
    url = "https://github.com/grailbio/bazel-toolchain/archive/refs/tags/{tag}.tar.gz".format(tag = BAZEL_TOOLCHAIN_TAG),
)

load("@com_grail_bazel_toolchain//toolchain:deps.bzl", "bazel_toolchain_dependencies")

bazel_toolchain_dependencies()

load("@com_grail_bazel_toolchain//toolchain:rules.bzl", "llvm_toolchain")

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

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
# Run: `bazel run @hedron_compile_commands//:refresh_all` to generate compile_commands.json
http_archive(
    name = "hedron_compile_commands",

    # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'hedron_compile_commands' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
    sha256 = "3cd0e49f0f4a6d406c1d74b53b7616f5e24f5fd319eafc1bf8eee6e14124d115",

    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    strip_prefix = "bazel-compile-commands-extractor-3dddf205a1f5cde20faf2444c1757abe0564ff4c",
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/3dddf205a1f5cde20faf2444c1757abe0564ff4c.tar.gz",
)

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")

hedron_compile_commands_setup()
