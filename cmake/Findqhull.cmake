
#TODO:这是我写的一个打算兼容大部分简单cmake find*.cmake  文件实现
# unset(findObject CACHE)
# set(findObject qhull)

find_path(libqhull_INCLUDE_DIR NAMES libqhull/qhull_a.h)
mark_as_advanced(libqhull_INCLUDE_DIR)

# find_path(libqhull_r_INCLUDE_DIR NAMES libqhull_r/qhull_ra.h)
# mark_as_advanced(libqhull_r_INCLUDE_DIR)

# find_path(libqhullcpp_INCLUDE_DIR NAMES libqhullcpp/Qhull.h)
# mark_as_advanced(libqhullcpp_INCLUDE_DIR)

message("SB---:${CMAKE_PREFIX_PATH}/lib")
#遍历所有子目录
#查找所有头文件
# file(GLOB_RECURSE list_include_globbed "${libqhull_INCLUDE_DIR}/libqhull/*.h" "${libqhull_INCLUDE_DIR}/libqhull/*.hpp")
# message("include:\n+++++++++++++++++++++++++++++++++++++++:start")
# foreach(file ${list_include_globbed})
#     message(STATUS "     - ${file}")
# endforeach()
# message("+++++++++++++++++++++++++++++++++++++++:end")


# unset(LIBS CACHE)           # 清空变量LIB_DIR
# #find_library(LIBS NAMES qhull libqhull)
# #查找所有库
# #遍历所有子目录
# set(libs_dir ${CMAKE_PREFIX_PATH}/lib)
# file(GLOB_RECURSE list_libs_globbed "${libs_dir}/*.lib" "${libs_dir}/*.a")
# message("lib:\n+++++++++++++++++++++++++++++++++++++++:start:${libs_dir}")
# foreach(file ${list_libs_globbed})
#     message(STATUS "     - ${file}")
# endforeach()
# message("+++++++++++++++++++++++++++++++++++++++:end")

if(NOT libqhull_LIBRARY)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # 使用微软编译器
        # find_library(libqhull_LIBRARY_RELEASE qhull PATHS .)
        # find_library(libqhull_LIBRARY_DEBUG qhull PATHS .)
        # 查找库(从最新/最相关的条目到最少的条目排序)。
        find_library(libqhull_r_LIBRARY_RELEASE NAMES
            qhull_r
        )
        find_library(libqhullcpp_LIBRARY_RELEASE NAMES
            qhullcpp
        )
        find_library(libqhull_static_LIBRARY_RELEASE NAMES
            qhullstatic
        )
        find_library(libqhull_static_r_LIBRARY_RELEASE NAMES
            qhullstatic_r
        )

        find_library(libqhull_r_LIBRARY_DEBUG NAMES
            qhull_rd
        )
        find_library(libqhullcpp_LIBRARY_DEBUG NAMES
            qhullcpp_d
        )
        find_library(libqhull_static_LIBRARY_DEBUG NAMES
            qhullstatic_d
        )
        find_library(libqhull_static_r_LIBRARY_DEBUG NAMES
            qhullstatic_rd
        )

    else()
        # 使用 Mingw 编译器
        # 查找库(从最新/最相关的条目到最少的条目排序)。
        find_library(libqhull_LIBRARY_RELEASE NAMES
            qhull.a
            libqhull.a
            qhull.dll.a
            libqhull.dll.a
        )


        find_library(libqhull_LIBRARY_DEBUG NAMES
            qhulld.a
            libqhulld.a
            libqhull_d.a
            qhulld.dll.a
            libqhulld.dll.a
            libqhull_d.dll.a
        )
    endif()


    mark_as_advanced(libqhull_r_LIBRARY_RELEASE)
    mark_as_advanced(libqhullcpp_LIBRARY_RELEASE)
    mark_as_advanced(libqhull_static_LIBRARY_RELEASE)
    mark_as_advanced(libqhull_static_r_LIBRARY_RELEASE)

    mark_as_advanced(libqhull_r_LIBRARY_DEBUG)
    mark_as_advanced(libqhullcpp_LIBRARY_DEBUG)
    mark_as_advanced(libqhull_static_LIBRARY_DEBUG)
    mark_as_advanced(libqhull_static_r_LIBRARY_DEBUG)

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(libqhull_LIBRARY ${libqhullcpp_LIBRARY_DEBUG})
      set(libqhull_static_r_LIBRARY ${libqhull_static_r_LIBRARY_DEBUG})
  else ()
      set(libqhull_LIBRARY ${libqhullcpp_LIBRARY_RELEASE})
      set(libqhull_static_r_LIBRARY ${libqhull_static_r_LIBRARY_RELEASE})
  endif ()

    mark_as_advanced(libqhull_LIBRARY)
    mark_as_advanced(libqhull_static_r_LIBRARY)
endif()

# 为了下游可以继续使用
set(libqhull_FOUND FALSE)
set(qhull_FOUND FALSE)
if (libqhull_INCLUDE_DIR AND libqhull_LIBRARY)
    set(libqhull_FOUND True)
    set(qhull_FOUND True)
    #set(QHULL_USES_QT)
    add_definitions(-DQHULL_USES_QT)

endif()

if(libqhull_FOUND)
    include_directories(${libqhull_INCLUDE_DIR})
    # include_directories(${libqhull_r_INCLUDE_DIR})
    # include_directories(${libqhullcpp_INCLUDE_DIR})
   # target_link_libraries(${PROJECT_NAME} ${Add_LIBRARIES})
   message("--------------------------------------------:${PROJECT_NAME}")
   message("libqhull_INCLUDE_DIR:" ${libqhull_INCLUDE_DIR})
   message("PROJECT_SOURCE_DIR:" ${PROJECT_SOURCE_DIR})
   message("libqhull_LIBRARY:" ${libqhull_LIBRARY})
   message("libqhull_r_LIBRARY_DEBUG:" ${libqhull_r_LIBRARY_DEBUG})
   message("libqhullcpp_LIBRARY_DEBUG:" ${libqhullcpp_LIBRARY_DEBUG})
   message("libqhull_static_LIBRARY_DEBUG:" ${libqhull_static_LIBRARY_DEBUG})
   message("libqhull_static_r_LIBRARY_DEBUG:" ${libqhull_static_r_LIBRARY_DEBUG})
   message("++++++++++++++++++++++++++++++++++++++++++++++:")
endif(libqhull_FOUND)



#unset(INCLUDE_DIR CACHE)

