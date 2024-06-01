#!/bin/bash


# 设置项目根目录
PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "PROJECT_ROOT:${PROJECT_ROOT}"

# 设置构建目录
BUILD_DIR="$PROJECT_ROOT/build"

if [ -d "$BUILD_DIR" ]; then
    rm -rf $BUILD_DIR
fi

# 创建构建目录
mkdir -p "$BUILD_DIR"

# 进入构建目录
cd "$BUILD_DIR"

# 运行 CMake 配置命令并指定 Xcode 生成器
cmake "$PROJECT_ROOT" -G Xcode