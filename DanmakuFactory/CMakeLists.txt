cmake_minimum_required(VERSION 3.0)

project(DanmakuFactory C)

add_executable (DanmakuFactory JsonFile.c main.c XmlFile.c
                AssFile/AssFile.c AssFile/AssStringProcessing.c Config/Config.c
                List/DanmakuFactoryList.c String/DanmakuFactoryString.c TemplateFile/TemplateFile.c)

install(TARGETS DanmakuFactory
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
