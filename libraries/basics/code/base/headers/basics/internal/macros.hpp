/*
 * MACROS
 * Copyright © 2011+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1407150000
 */

// http://sourceforge.net/p/predef/wiki/Home
// https://gcc.gnu.org/onlinedocs/gcc-4.1.2/cpp/Predefined-Macros.html
// http://msdn.microsoft.com/en-us/library/b0084kay.aspx
// http://clang.llvm.org/docs/LanguageExtensions.html
// http://wiki.apache.org/stdcxx/C++0xCompilerSupport
// http://nadeausoftware.com/articles/2012/10/c_c_tip_how_detect_compiler_name_and_version_using_compiler_predefined_macros
// http://www.boost.org/doc/libs/1_51_0/boost/config/compiler/gcc.hpp
// http://www.boost.org/doc/libs/1_59_0/boost/config/compiler/clang.hpp
// http://www.boost.org/doc/libs/1_59_0/boost/config/compiler/visualc.hpp

#ifndef BASICS_MACROS_HEADER
#define BASICS_MACROS_HEADER

   /* --------------------------------------------------------------------------------------------- +
                              Determine the compiler maker and its version
    + --------------------------------------------------------------------------------------------- */

    #if  defined(__clang__)                         // Clang must be put before GCC because it defines various GCC macros

        #define BASICS_CLANG_COMPILER
        #define BASICS_COMPILER_VERSION (__clang_major__ * 100 + __clang_minor__)

        #if BASICS_COMPILER_VERSION < 303                                           // CUIDADO CON EL NÚMERO DE VERSIÓN DEL CLANG USADO EN MAC OS

            #error "basics++: this library requires Clang 3.3 or newer."

        #endif

    #elif defined(__GNUC__)

        #define BASICS_GNU_COMPILER
        #define BASICS_COMPILER_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

        #if BASICS_COMPILER_VERSION < 408

            #error "basics++: this library requires GCC 4.8 or newer."

        #endif

    #elif defined(_MSC_VER)

        #define BASICS_MICROSOFT_COMPILER
        #define BASICS_COMPILER_VERSION (_MSC_VER)

        #if BASICS_COMPILER_VERSION < 1900

            #error "basics++: this library requires Visual C++ 2015 or newer."

        #endif

    #else

        #error "basics++: unsupported compiler."

    #endif

   /* --------------------------------------------------------------------------------------------- +
                                   Determine the compiler features
    + --------------------------------------------------------------------------------------------- */

    #if defined(BASICS_GNU_COMPILER)

        #ifdef __EXCEPTIONS
            #define BASICS_EXCEPTIONS_ENABLED
        #endif

        #ifdef __GXX_RTTI
            #define BASICS_RTTI_ENABLED
        #endif

    #elif defined(BASICS_MICROSOFT_COMPILER)

        #ifdef _CPPUNWIND
            #define BASICS_EXCEPTIONS_ENABLED
        #endif

        #ifdef _CPPRTTI
            #define BASICS_RTTI_ENABLED
        #endif

    #elif defined(BASICS_CLANG_COMPILER)

        #if __has_feature(cxx_exceptions)
            #define BASICS_EXCEPTIONS_ENABLED
        #endif

        #if __has_feature(cxx_rtti)
            #define BASICS_RTTI_ENABLED
        #endif

    #endif

   /* --------------------------------------------------------------------------------------------- +
                  Apply patches to get standard features not provided by some compilers
    + --------------------------------------------------------------------------------------------- */

    // http://wiki.apache.org/stdcxx/C++0xCompilerSupport
    // https://msdn.microsoft.com/en-us/library/hh567368.aspx
    // https://gcc.gnu.org/projects/cxx0x.html
    // http://clang.llvm.org/cxx_status.html

    #if defined(BASICS_MICROSOFT_COMPILER)

        #include <ciso646>

    #endif

   /* --------------------------------------------------------------------------------------------- +
                                    Detect the target operating system
    + --------------------------------------------------------------------------------------------- */

    // https://sourceforge.net/p/predef/wiki/OperatingSystems/

    #if defined(_WIN32) || defined(_WIN64)

        #define BASICS_WINDOWS_OS

    #elif defined(__ANDROID__) || defined(__ANDROID_NDK__)

        #define BASICS_ANDROID_OS

    #elif defined(__APPLE__) && defined(TARGET_OS_IPHONE)

        #define BASICS_IOS

    #elif defined(__APPLE__)

        #define BASICS_MAC_OS

    #elif defined(__linux__) || defined(__gnu_linux__)

        #define BASICS_LINUX_OS

    #else

        #error "basics++: unsupported operating system."

    #endif

   /* --------------------------------------------------------------------------------------------- +
                                     Detect the platform SDK version
    + --------------------------------------------------------------------------------------------- */

    #if   defined(BASICS_WINDOWS_OS)

        // http://blogs.msdn.com/b/oldnewthing/archive/2007/04/11/2079137.aspx

    #elif defined(BASICS_LINUX_OS)

    #elif defined(BASICS_ANDROID_OS)

        #include <android/api-level.h>

        #define BASICS_PLATFORM_API_VERSION __ANDROID_API__

    #elif defined(BASICS_IOS)

    #elif defined(BASICS_MAC_OS)

    #endif

   /* --------------------------------------------------------------------------------------------- +
                     Detect the target processor architecture and its basic features
    + --------------------------------------------------------------------------------------------- */

    #include <climits>
    #include <cstdint>

    #if    UINT_MAX == UINT32_MAX

        #define BASICS_INT_SIZE 4

    #elif  UINT_MAX == UINT64_MAX

        #define BASICS_INT_SIZE 8

    #endif

    #if defined(__amd64__) || defined(_M_AMD64) || defined(__amd64) || defined(__x86_64__)

        #define BASICS_AMD64_ARCHITECTURE
        #define BASICS_WORD_SIZE 8
        #define BASICS_LITTLE_ENDIAN
        #define BASICS_DEFAULT_MEMORY_ALIGNMENT 8

    #elif defined(__i386__) || defined(_M_IX86) || defined(__i386)

        #define BASICS_IA32_ARCHITECTURE
        #define BASICS_WORD_SIZE 4
        #define BASICS_LITTLE_ENDIAN
        #define BASICS_DEFAULT_MEMORY_ALIGNMENT 4

    #elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM)

        #define BASICS_ARM_ARCHITECTURE

        #if defined(__LP64__) || defined(_M_X64) || defined(__aarch64__) || defined(__ARM_ARCH_ISA_A64)

            #define BASICS_WORD_SIZE 8
            #define BASICS_DEFAULT_MEMORY_ALIGNMENT 8

        #else

            #define BASICS_WORD_SIZE 4
            #define BASICS_DEFAULT_MEMORY_ALIGNMENT 4

        #endif

        // Detect endianness...

    #elif defined(__mips__) || defined(__MIPS__) || defined(__mips)

        #define BASICS_MIPS_ARCHITECTURE

        #if defined(__LP64__)

            #define BASICS_WORD_SIZE 8
            #define BASICS_DEFAULT_MEMORY_ALIGNMENT 8

        #else

            #define BASICS_WORD_SIZE 4
            #define BASICS_DEFAULT_MEMORY_ALIGNMENT 4

        #endif

        // Detect endianness...

    #else

        #error "basics++: unsupported target processor architecture."

    #endif

   /* --------------------------------------------------------------------------------------------- +
                 Detect if the build must be optimized and define NDEBUG if appropriate
    + --------------------------------------------------------------------------------------------- */

    #if defined(__OPTIMIZE__) || ( defined(BASICS_MICROSOFT_COMPILER) && not defined(_DEBUG) )

        #define BASICS_OPTIMIZED_BUILD

        #if not defined(NDEBUG)

            #define NDEBUG

        #endif

    #endif

#endif
