cmake_minimum_required(VERSION 3.10)
project(CEF3SimpleSample VERSION 1.0)

set(APP_NAME "CEF3SimpleSample")
set(APP_FOLDER_NAME "CEF3SimpleSample")

if(APPLE)
    ### Build main executable
    find_program(IBTOOL ibtool HINTS "/usr/bin" "${OSX_DEVELOPER_ROOT}/usr/bin")
    find_library(COCOA_LIBRARY Cocoa)
    file(GLOB SRC_FILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/*.mm"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform/*.cpp"
    )
    add_executable(MainExecutable MACOSX_BUNDLE ${SRC_FILES})
    target_compile_features(MainExecutable PRIVATE cxx_range_for)
    target_link_libraries(MainExecutable
        ${COCOA_LIBRARY}
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/lib/cef_sandbox.a"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/lib/libcef_dll_wrapper.a"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/lib/Chromium Embedded Framework.framework"
    )
    target_include_directories(MainExecutable PUBLIC
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/include"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform"
    )
    set(EXECUTABLE_NAME ${APP_NAME})
    set(PRODUCT_NAME ${APP_NAME})
    set_target_properties(MainExecutable PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/CEF3SimpleSample-Info.plist"
        OUTPUT_NAME "${APP_NAME}"
    )
    add_custom_command (TARGET MainExecutable POST_BUILD
        COMMAND ${IBTOOL} --errors --warnings --notices
                --output-format human-readable-text --compile
                "${PROJECT_BINARY_DIR}/${APP_NAME}.app/Contents/Resources/Base.lproj/MainMenu.nib"
                "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/Resources/Base.lproj/MainMenu.xib"
    )
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/lib/Chromium Embedded Framework.framework"
                "${PROJECT_BINARY_DIR}/${APP_NAME}.app/Contents/Frameworks/Chromium Embedded Framework.framework"
        VERBATIM
    )
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform/html"
                "${PROJECT_BINARY_DIR}/${APP_NAME}.app/Contents/Resources/html"
        VERBATIM
    )
    ### Build helpers
    file(GLOB SRC_FILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/AppDelegate.mm"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/Helper Process Main.cpp"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform/*.cpp"
    )
    set(HELPER_NAME "HelperApp")
    add_executable(${HELPER_NAME} MACOSX_BUNDLE ${SRC_FILES})
    target_compile_features(${HELPER_NAME} PRIVATE cxx_range_for)
    target_link_libraries(${HELPER_NAME}
        ${COCOA_LIBRARY}
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/lib/cef_sandbox.a"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/lib/libcef_dll_wrapper.a"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/lib/Chromium Embedded Framework.framework"
    )
    target_include_directories(${HELPER_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/include"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform"
    )
    set_target_properties(${HELPER_NAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/CEF3SimpleSampleHelper-Info.plist"
        OUTPUT_NAME "${APP_NAME} Helper"
    )
    add_custom_command(
        TARGET ${HELPER_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/lib/Chromium Embedded Framework.framework"
                "${PROJECT_BINARY_DIR}/${APP_NAME} Helper.app/Contents/Frameworks/Chromium Embedded Framework.framework"
        VERBATIM
    )
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_BINARY_DIR}/${APP_NAME} Helper.app"
                "${PROJECT_BINARY_DIR}/${APP_NAME}.app/Contents/Frameworks/${APP_NAME} Helper.app"
        VERBATIM
    )
    add_dependencies(MainExecutable ${HELPER_NAME})
    set(HELPER_SUFFIXES "GPU" "Plugin" "Renderer")
    foreach(suffix ${HELPER_SUFFIXES})
        file(GLOB SRC_FILES CONFIGURE_DEPENDS
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/Helper Process Main.cpp"
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform/*.cpp"
        )
        set(HELPER_NAME "${suffix}HelperApp")
        add_executable(${HELPER_NAME} MACOSX_BUNDLE ${SRC_FILES})
        target_compile_features(${HELPER_NAME} PRIVATE cxx_range_for)
        target_link_libraries(${HELPER_NAME}
            ${COCOA_LIBRARY}
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/lib/cef_sandbox.a"
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/lib/libcef_dll_wrapper.a"
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/lib/Chromium Embedded Framework.framework"
        )
        target_include_directories(${HELPER_NAME} PUBLIC
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac"
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/include"
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform"
        )
        set_target_properties(${HELPER_NAME} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/CEF3SimpleSampleHelper-Info.plist"
            OUTPUT_NAME "${APP_NAME} Helper (${suffix})"
        )
        add_custom_command(
            TARGET ${HELPER_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Mac/lib/Chromium Embedded Framework.framework"
                    "${PROJECT_BINARY_DIR}/${APP_NAME} Helper (${suffix}).app/Contents/Frameworks/Chromium Embedded Framework.framework"
            VERBATIM
        )
        add_custom_command(
            TARGET MainExecutable
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${PROJECT_BINARY_DIR}/${APP_NAME} Helper (${suffix}).app"
                    "${PROJECT_BINARY_DIR}/${APP_NAME}.app/Contents/Frameworks/${APP_NAME} Helper (${suffix}).app"
            VERBATIM
        )
        add_dependencies(MainExecutable ${HELPER_NAME})
    endforeach()
elseif(WIN32)
    file(GLOB SRC_FILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Win/main.cpp"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform/*.cpp"
    )
    add_executable(MainExecutable WIN32 ${SRC_FILES})
    target_compile_features(MainExecutable PRIVATE cxx_range_for)
    target_compile_options(MainExecutable PUBLIC "/EHsc" "/MT")
    target_link_libraries(MainExecutable
        ${GTK3_LIBRARIES}
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Win/lib/libcef_dll_wrapper.lib"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Win/lib/libcef.lib"
    )
    target_include_directories(MainExecutable PUBLIC
        ${GTK3_INCLUDE_DIRS}
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Win"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Win/include"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform"
    )
    set(EXECUTABLE_NAME ${APP_NAME})
    set(PRODUCT_NAME ${APP_NAME})
    set_target_properties(MainExecutable PROPERTIES OUTPUT_NAME "${APP_NAME}")
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform/html"
                "${PROJECT_BINARY_DIR}/bin/html"
        VERBATIM
    )
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Win/Resources"
                "${PROJECT_BINARY_DIR}/bin"
        VERBATIM
    )
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Win/lib"
                "${PROJECT_BINARY_DIR}/bin"
        VERBATIM
    )
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                "${PROJECT_BINARY_DIR}/Release/${APP_NAME}.exe"
                "${PROJECT_BINARY_DIR}/bin/${APP_NAME}.exe"
        VERBATIM
    )
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    file(GLOB SRC_FILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Linux/main.cpp"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform/*.cpp"
    )
    add_executable(MainExecutable ${SRC_FILES})
    target_compile_options(MainExecutable PUBLIC ${GTK3_CFLAGS_OTHER})
    target_compile_features(MainExecutable PRIVATE cxx_range_for)
    target_link_directories(MainExecutable PUBLIC ${GTK3_LIBRARY_DIRS})
    target_link_libraries(MainExecutable
        ${GTK3_LIBRARIES}
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Linux/lib/libcef_dll_wrapper.a"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Linux/lib/libcef.so"
    )
    target_include_directories(MainExecutable PUBLIC
        ${GTK3_INCLUDE_DIRS}
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Linux"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Linux/include"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform"
    )
    set(EXECUTABLE_NAME ${APP_NAME})
    set(PRODUCT_NAME ${APP_NAME})
    set_target_properties(MainExecutable PROPERTIES OUTPUT_NAME "${APP_NAME}")
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CrossPlatform/html"
                "${PROJECT_BINARY_DIR}/bin/html"
        VERBATIM
    )
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/Linux/lib"
                "${PROJECT_BINARY_DIR}/bin"
        VERBATIM
    )
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                "${PROJECT_BINARY_DIR}/${APP_NAME}"
                "${PROJECT_BINARY_DIR}/bin/${APP_NAME}"
        VERBATIM
    )
endif()
