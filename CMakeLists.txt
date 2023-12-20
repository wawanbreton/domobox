cmake_minimum_required(VERSION 3.6)

project(domobox-v2 LANGUAGES CXX)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("####### Simulator mode")
    add_compile_definitions(ENV_SIMULATOR)
else()
    message("####### Physical mode")
    set(CMAKE_PREFIX_PATH "/usr/local/Qt-5.15.1/lib/cmake/Qt5")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Xml Widgets Sql Multimedia SerialPort REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Xml Widgets Sql Multimedia SerialPort REQUIRED)

if(ANDROID)
    add_library(domobox-v2 SHARED
      main.cpp
      constants.h
      domostyle.cpp
      domostyle.h
      powermanager.cpp
      powermanager.h
      resource/audioaccessor.cpp
      resource/audiostate.h
      resource/configurationmanager.cpp
      resource/dailyhotperiods.cpp
      resource/daymode.h
      resource/fireaccessor.cpp
      resource/firemode.h
      resource/forecastdata.cpp
      resource/gpiocontroller.cpp
      resource/playlist.cpp
      resource/hotperiod.cpp
      resource/temperaturethresholds.cpp
      resource/resourceaccessor.cpp
      resource/shutter.h
      resource/shutteraccessor.h
      resource/shutteraccessor.cpp
      resource/shuttercommand.h
      resource/song.h
      resource/temperatureaccessor.cpp
      resource/temperatureconfiguration.cpp
      resource/timeaccessor.cpp
      resource/weatheraccessor.cpp
      resource/wifiaccessor.cpp
      resource/wifistate.h
      qml.qrc
      assets.qrc
    )
else()
    add_executable(domobox-v2
      main.cpp
      constants.h
      domostyle.cpp
      domostyle.h
      powermanager.cpp
      powermanager.h
      resource/audioaccessor.cpp
      resource/audiostate.h
      resource/configurationmanager.cpp
      resource/dailyhotperiods.cpp
      resource/daymode.h
      resource/fireaccessor.cpp
      resource/firemode.h
      resource/forecastdata.cpp
      resource/gpiocontroller.cpp
      resource/playlist.cpp
      resource/hotperiod.cpp
      resource/temperaturethresholds.cpp
      resource/resourceaccessor.cpp
      resource/shutter.h
      resource/shutteraccessor.h
      resource/shutteraccessor.cpp
      resource/shuttercommand.h
      resource/song.h
      resource/temperatureaccessor.cpp
      resource/temperatureconfiguration.cpp
      resource/timeaccessor.cpp
      resource/weatheraccessor.cpp
      resource/wifiaccessor.cpp
      resource/wifistate.h
      qml.qrc
      assets.qrc
    )
endif()

target_compile_definitions(domobox-v2
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(domobox-v2
  PRIVATE Qt${QT_VERSION_MAJOR}::Core
          Qt${QT_VERSION_MAJOR}::Quick
          Qt${QT_VERSION_MAJOR}::Xml
          Qt${QT_VERSION_MAJOR}::Widgets
          Qt${QT_VERSION_MAJOR}::Sql
          Qt${QT_VERSION_MAJOR}::Multimedia
          Qt${QT_VERSION_MAJOR}::SerialPort
          sensors
          asound)
