cmake_minimum_required(VERSION 3.29)
project(GameEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# تعيين مسارات Vulkan و GLFW
set(VULKAN_SDK "/Users/Abdulkafi/VulkanSDK/macOS/")
set(GLFW_SDK "/Users/Abdulkafi/glfw/")

# تعريف متغير يحمل قيمة true إذا كانت NDEBUG مُعرفة
set(NDEBUG_ENABLED OFF)  # إعداد المتغير افتراضيًا إلى OFF
if(DEFINED NDEBUG)
    set(NDEBUG_ENABLED TRUE)
endif()

# تعريف الملفات المصدر والرأس
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/VulkanCore/include/*.h")
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/VulkanCore/Source/*.cpp")

# إضافة الهدف التنفيذي وتضمين الملفات
add_executable(GameEngine main.cpp ${SOURCE_FILES} ${HEADER_FILES})

# تضمين مسارات الـ include
target_include_directories(GameEngine PRIVATE
        "${VULKAN_SDK}/include"
        "${GLFW_SDK}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/VulkanCore/include/"
)

# ربط مكتبات Vulkan و GLFW
target_link_libraries(GameEngine
        "${VULKAN_SDK}/lib/libvulkan.dylib"
        "${VULKAN_SDK}/lib/libMoltenVK.dylib"
        "${GLFW_SDK}/lib/libglfw.3.dylib"
)

# إعداد الأيقونة لتطبيق macOS (اختياري)
set(APP_ICON_MACOSX "${CMAKE_SOURCE_DIR}/Resources/logo.icns")
if (EXISTS "${APP_ICON_MACOSX}")
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "${CMAKE_SOURCE_DIR}/Resources")
    set_target_properties(GameEngine PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_ICON_FILE "logo.icns"
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Resources/Info.plist"
            RESOURCE "${APP_ICON_MACOSX}"
    )
endif()

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
