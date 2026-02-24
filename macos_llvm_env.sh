#!/usr/bin/env bash
# setup paths so you can build this project with llvm on macos
export LDFLAGS="/opt/homebrew/opt/llvm/lib/c++/libc++.dylib"
export CXXFLAGS="-I/opt/homebrew/opt/llvm/include/c++/v1 -stdlib=libc++"
export CPPFLAGS="-I/opt/homebrew/opt/llvm/include/c++/v1"

export SDKROOT=$(xcrun --show-sdk-path)
export CC=/opt/homebrew/opt/llvm/bin/clang
export CXX=/opt/homebrew/opt/llvm/bin/clang++
export CPPFLAGS="-I/opt/homebrew/opt/llvm/include/c++/v1"
export CXXFLAGS="-I/opt/homebrew/opt/llvm/include/c++/v1 -stdlib=libc++"
export LDFLAGS="/opt/homebrew/opt/llvm/lib/c++/libc++.dylib -Wl,-rpath,/opt/homebrew/opt/llvm/lib"
