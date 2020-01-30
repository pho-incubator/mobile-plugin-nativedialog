CONFIG += testcase parallel_test
TARGET = tst_gnativedialog
osx:CONFIG -= app_bundle

QT += nativedialog nativedialog-private testlib
SOURCES += \
    tst_gnativedialog.cpp
