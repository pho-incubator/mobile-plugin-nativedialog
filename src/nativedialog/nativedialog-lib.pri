ANDROID_BUNDLED_JAR_DEPENDENCIES = jar/GroupsIncAndroidNativeDialog-bundled.jar
ANDROID_JAR_DEPENDENCIES = jar/GroupsIncAndroidNativeDialog.jar

INCLUDEPATH += $$PWD

PUBLIC_HEADERS += \
    gnativedialog_global.h \
    gnativedialogmodel.h

PRIVATE_HEADERS += \
    gnativedialogmodel_p.h

SOURCES += \
    gnativedialogmodel.cpp

android {
    QT += androidextras

    PRIVATE_HEADERS += \
        gnativedialogmodel_android_p.h

    SOURCES += \
        gnativedialogmodel_android_p.cpp

} else:ios {

    QT += gui-private

    PRIVATE_HEADERS += \
        gnativedialogmodel_ios_p.h

    OBJECTIVE_SOURCES += \
        gnativedialogmodel_ios_p.mm

    LIBS += -framework MobileCoreServices -framework AssetsLibrary

} else {

    PRIVATE_HEADERS += \
        gnativedialogmodel_default_p.h

    SOURCES += \
        gnativedialogmodel_default_p.cpp
}

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS
