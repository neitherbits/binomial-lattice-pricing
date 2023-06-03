# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.231.6/containers/cpp/.devcontainer/base.Dockerfile

# [Choice] Debian / Ubuntu version (use Debian 11, Ubuntu 18.04/21.04 on local arm64/Apple Silicon): debian-11, debian-10, ubuntu-21.04, ubuntu-20.04, ubuntu-18.04
ARG VARIANT="ubuntu-22.04"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}

# Install needed packages. Use a separate RUN statement to add your own dependencies.
RUN echo 'VERSION="dev"' > /usr/local/etc/vscode-dev-containers/meta.env
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install build-essential cmake cppcheck valgrind clang clangd lldb llvm gdb openjdk-11-jdk lcov \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

ARG USERNAME=vscode

#
# Install bazelisk
#
RUN if [[ `dpkg --print-architecture` == "arm64" ]]; then \
    wget https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-arm64 -O /usr/local/bin/bazel \
    && chmod +x /usr/local/bin/bazel; \
    else \
    wget https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-amd64 -O /usr/local/bin/bazel \
    && chmod +x /usr/local/bin/bazel; \
    fi

#
# Install buildifier
#
RUN if [[ `dpkg --print-architecture` == "arm64" ]]; then \
    wget https://github.com/bazelbuild/buildtools/releases/latest/download/buildifier-linux-arm64 -O /usr/local/bin/buildifier \
    && chmod +x /usr/local/bin/buildifier; \
    else \
    wget https://github.com/bazelbuild/buildtools/releases/latest/download/buildifier-linux-amd64 -O /usr/local/bin/buildifier \
    && chmod +x /usr/local/bin/buildifier; \
    fi

#
# Set up command history volume
# See https://code.visualstudio.com/remote/advancedcontainers/persist-bash-history
#
RUN mkdir /commandhistory \
    && chown -R $USERNAME /commandhistory
