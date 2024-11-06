
cmake_minimum_required(VERSION 3.29)
project(GameEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# تعيين مسارات Vulkan و GLFW
set(VULKAN_SDK "/Users/mac/VulkanSDK/macOS/")
set(GLFW_SDK "/Users/mac/glfw/")

# إضافة الملفات التنفيذية

# تعريف الملفات المصدر والرأس
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/VulkanCore/include/*.h")
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/VulkanCore/Source/*.cpp")

# إضافة الهدف التنفيذي وتضمين الملفات
add_executable(GameEngine main.cpp ${SRC_FILES} ${HEADER_FILES} ${SOURCE_FILES}
        VulkanCore/include/VulkanApp.h
        VulkanCore/include/VulkanApp.h)

# تضمين مسارات الـ include بعد إنشاء الهدف
target_include_directories(GameEngine PRIVATE
        "${VULKAN_SDK}/include"
        "${GLFW_SDK}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/VulkanCore/include/"
)

# ربط مكتبات Vulkan و GLFW بشكل مباشر
target_link_libraries(GameEngine
        "${VULKAN_SDK}/lib/libvulkan.dylib"
        "${VULKAN_SDK}/lib/libMoltenVK.dylib"
        "${GLFW_SDK}/lib/libglfw.3.dylib"

)
# إعداد الأيقونة لتطبيق macOS (اختياري)

# إعداد الأيقونة لتطبيق macOS (اختياري)
set(APP_ICON_MACOSX "../../GameEngine/Resources/logo.icns")
if (EXISTS "${APP_ICON_MACOSX}")
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "../../GameEngine/Resources")
    set_target_properties(GameEngine PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_ICON_FILE "logo.icns"
            MACOSX_BUNDLE_INFO_PLIST "../../GameEngine/Resources/Info.plist"
            RESOURCE "${APP_ICON_MACOSX}"
    )
endif ()

# نسخ مجلد Resources إلى داخل حزمة التطبيق بعد البناء
add_custom_command(
        TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Resources" "${CMAKE_BINARY_DIR}/GameEngine.app/Contents/Resources"
        COMMENT "Copying Resources to the application bundle"
)


# إعداد بيئة البناء بناءً على نوع البناء (Debug أو Release)
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE is not set. Defaulting to Release mode.")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# تشغيل ملف سكربت بعد بناء المشروع لتجميع الـ shaders
add_custom_command(
        TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E env bash "${CMAKE_SOURCE_DIR}/shaders/compile.sh"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Executing shader compile script after build..."
)

# نسخ محتويات مجلد shaders إلى مجلد التنفيذ بعد البناء
add_custom_command(
        TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/GameEngine.app/Contents/shaders"
        COMMENT "Copying shaders to the build directory..."
)

# تضمين CPack لإنشاء الحزم (اختياري)
include(CPack)
