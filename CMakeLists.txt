cmake_minimum_required(VERSION 3.19)
project(HuffmanZip LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(HuffmanZip
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    ziputil.h ziputil.cpp
    huffmannode.h huffmannode.cpp
    huffmantree.h huffmantree.cpp
        FileWriterUtil.cpp
        FileWriterUtil.h
        FileReaderUtil.cpp
        FileReaderUtil.h
        BitStreamWriter.cpp
        BitStreamWriter.h
        BitStreamReader.cpp
        BitStreamReader.h
)

target_link_libraries(HuffmanZip
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS HuffmanZip
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET HuffmanZip
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
