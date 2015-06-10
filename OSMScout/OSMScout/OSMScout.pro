TEMPLATE = app
load(ubuntu-click)

# specify the manifest file, this file is required for click
# packaging and for the IDE to create runconfigurations
UBUNTU_MANIFEST_FILE=manifest.json.in
#QT_CONFIG -= no-pkg-config
CONFIG += qt thread c++11

QT += core gui widgets qml quick svg positioning

#PKGCONFIG += libosmscout-map-qt

gcc:QMAKE_CXXFLAGS += -fopenmp

#QMAKE_CFLAGS = -O3 -march=armv7-a -mtune=cortex-a8
#QMAKE_CXXFLAGS = -O3 -march=armv7-a -mtune=cortex-a8

INCLUDEPATH = src
INCLUDEPATH += ../libosmscout/include/
INCLUDEPATH += ../libosmscout-map/include
INCLUDEPATH += ../libosmscout-map-qt/include

release: DESTDIR = release
debug:   DESTDIR = debug

OBJECTS_DIR = $$DESTDIR/
MOC_DIR = $$DESTDIR/
RCC_DIR = $$DESTDIR/
UI_DIR = $$DESTDIR/

SOURCES = src/OSMScout.cpp \
          src/Settings.cpp \
          src/Theme.cpp \
          src/DBThread.cpp \
          src/MapWidget.cpp \
          src/MainWindow.cpp \
          src/SearchLocationModel.cpp \
          src/RoutingModel.cpp \
          
HEADERS = src/Settings.h \
          src/Theme.h \
          src/DBThread.h \
          src/MapWidget.h \
          src/MainWindow.h \
          src/SearchLocationModel.h \
          src/RoutingModel.h \

QMLFILES += \
    qml/custom/MapButton.qml \
    qml/main.qml \
    qml/RoutingDialog.qml \
    qml/AboutDialog.qml \
    qml/DownloadMapDialog.qml \
    qml/SearchDialog.qml

CUSTOMQMLFILES += \
    qml/custom/LineEdit.qml \
    qml/custom/DialogActionButton.qml \
    qml/custom/LocationEdit.qml \
    qml/custom/LocationSearch.qml \
    qml/custom/ScrollIndicator.qml \
    qml/custom/MapDialog.qml \
    qml/custom/ListItemWithActions.qml

PICFILES += \
    pics/DeleteText.svg \
    pics/route.svg

DEFINES += "__UBUNTU__"

qml_files.path = /qml
qml_files.files = $${QMLFILES}
INSTALLS+=qml_files

custom_qml_files.path = /qml/custom
custom_qml_files.files = $${CUSTOMQMLFILES}
INSTALLS+=custom_qml_files

pic_files.path = /pics
pic_files.files = $${PICFILES}
INSTALLS+=pic_files


#map_files.path = /
#map_files.files = $${MAPFILES}
#INSTALLS+=map_files

RESOURCES += \
    res.qrc

OTHER_FILES += OSMScout.apparmor \
               OSMScout.desktop \
               OSMScout.png


#specify where the config files are installed to
config_files.path = /OSMScout
config_files.files += $${OTHER_FILES}
message($$config_files.files)
INSTALLS+=config_files


LIBS += ../libosmscout-map-qt/libosmscout-map-qt.a \
        ../libosmscout-map/libosmscout-map.a \
        ../libosmscout/libosmscout.a


#LIBS += ../libosmscout/src/.libs/libosmscout.so \
#                     ../libosmscout-map/src/.libs//libosmscoutmap.so \
#                     ../libosmscout/src/.libs/libosmscout.so.0 \
#                     ../libosmscout-map/src/.libs//libosmscoutmap.so.0
#                     ../libosmscout-map-qt/src/.libs/libosmscoutmapqt.so

#ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
target.path = $${UBUNTU_CLICK_BINARY_PATH}

INSTALLS+=target

#libfiles.files = $${LIBS}
#libfiles.path = $${UBUNTU_CLICK_BINARY_PATH}/..

#xINSTALLS+=libfiles