if(DEFINED _TOOLCHAIN_ANDROID_EXAMPLE)
  return()
else()
  set(_TOOLCHAIN_ANDROID_EXAMPLE 1)
endif()

set(_TOOLCHAIN_C_STANDARD 11)
set(_TOOLCHAIN_CXX_STANDARD 11)

set(ANDROID_NDK_VERSION "r21e")
set(CMAKE_SYSTEM_VERSION "8")
set(CMAKE_ANDROID_ARCH_ABI "armeabi-v7a")
set(CMAKE_ANDROID_ARM_NEON FALSE)
set(CMAKE_ANDROID_ARM_MODE TRUE) # 32-bit ARM
set(CMAKE_ANDROID_STL_TYPE "c++_static") # LLVM libc++ static

set(ANDROID_NDK_r21e C:/Users/Kiky/AppData/Local/Android/Sdk/ndk/21.4.7075529)

include("${CMAKE_CURRENT_LIST_DIR}/_utilities/_init.cmake")

_init("Android NDK")

include("${CMAKE_CURRENT_LIST_DIR}/_utilities/_common.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/_flags/_common.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/_os/_android.cmake")
