load(qt_build_paths)
CONFIG += java
DESTDIR = $$MODULE_BASE_OUTDIR/jar

JAVACLASSPATH += $$PWD/src

JAVASOURCES += \
    $$PWD/src/com/groupsinc/NativeDialog/NativeDialogController.java \
    $$PWD/src/com/groupsinc/NativeDialog/ImagePickerActivity.java

OTHER_FILES += $$PWD/AndroidManifest.xml

# install
target.path = $$[QT_INSTALL_PREFIX]/jar
INSTALLS += target
