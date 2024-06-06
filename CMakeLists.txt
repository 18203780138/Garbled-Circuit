    cmake_minimum_required(VERSION 3.10)
    project(MyCryptoProject)

    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a+crypto") # 为 ARMv8 架构启用支持

    # 查找所需的包
    find_package(OpenSSL REQUIRED)

    # 包含目录
    include_directories(/usr/local/include) # 根据你的实际安装路径可能需要修改
    include_directories(${PROJECT_SOURCE_DIR}) # 添加项目根目录为包含目录，便于包含自定义头文件

    # 链接目录
    link_directories(/usr/local/lib) # 根据你的实际安装路径可能需要修改

    # 添加可执行文件
    add_executable(test glnp_eval.h glnp_gen.h GLNP.cpp)

    # 链接库
    target_link_libraries(test OpenSSL::SSL OpenSSL::Crypto)
    target_link_libraries(test emp-tool) # 确保你的系统中已安装 EMP-toolkit

