CXX_MODULE = nativedialog
TARGET  = declarative_nativedialog
TARGETPATH = GroupsIncNativeDialog
IMPORT_VERSION = 1.0

QT += qml quick mvc nativedialog nativedialog-private
SOURCES += \
    $$PWD/nativedialog.cpp

load(qml_plugin)

OTHER_FILES += qmldir
