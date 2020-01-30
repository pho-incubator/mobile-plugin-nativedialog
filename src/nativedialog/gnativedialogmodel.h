#ifndef NATIVEDIALOGMODEL_H
#define NATIVEDIALOGMODEL_H

#include <QObject>
#include <GroupsIncMvc/gmvcmodel.h>
#include "gnativedialog_global.h"
#include <QVariant>
#include <qqml.h>

class NativeDialogModelPrivate;

class Q_NATIVEDIALOG_EXPORT NativeDialogModel : public MvcModel
{
    Q_OBJECT
    Q_ENUMS(DialogStyle)
    Q_ENUMS(PickerType)
    Q_ENUMS(ImageOrientation)
public:
    enum DialogStyle {
        Default,
        Login,
        PlainTextInput,
        SecureTextInput
    };

    enum PickerType {
        PhotoLibrary = 0,
        PhotoAlbum,
        Camera
    };

    enum ImageOrientation {
        Up = 0,
        Down,
        Left,
        Right,
        UpMirrored,
        DownMirrored,
        LeftMirrored,
        RightMirrored
    };

    explicit NativeDialogModel(MvcFacade *parent = 0);
    ~NativeDialogModel();

    Q_INVOKABLE void alert(const QString &id,
                           const QString &title, const QString &message,
                           const QString &cancelButtonTitle = tr("OK"),
                           const QVariantList &otherButtons = QVariantList(),
                           DialogStyle style = Default);

    Q_INVOKABLE void actionSheet(const QString &id,
                                 const QString &title, const QString &cancelButtonTitle = tr("Cancel"),
                                 const QString &destructiveButtonTitle = QString(),
                                 const QVariantList &otherButtons = QVariantList());

    Q_INVOKABLE void openImagePicker(PickerType type = PhotoLibrary);

    static const char *NAME;

    void init();
    const char *name();
    void apply(const QVariantMap &config);

Q_SIGNALS:
    void buttonClicked(const QString &id, int buttonIndex, const QString &buttonLabel);
    void buttonClickedWithText(const QString &id, int buttonIndex, const QString &buttonLabel, const QString &inputText);
    void buttonClickedWithLogin(const QString &id, int buttonIndex, const QString &buttonLabel, const QString &username, const QString &password);

    void canceled(const QString &id);
    void beforeClose(const QString &id, int buttonIndex, const QString &buttonLabel);
    void closed(const QString &id, int buttonIndex, const QString &buttonLabel);

    void imageSelectStarted();
    void imageSelected(const QString &imagePath, ImageOrientation orientation);
    void imageSelectCancel();
    void imageError(const QString &error);

private:
    Q_DECLARE_PRIVATE(NativeDialogModel)
    NativeDialogModelPrivate *d_ptr;

};

QML_DECLARE_TYPE(NativeDialogModel)
Q_DECLARE_METATYPE(NativeDialogModel::DialogStyle)
Q_DECLARE_METATYPE(NativeDialogModel::PickerType)
Q_DECLARE_METATYPE(NativeDialogModel::ImageOrientation)

#endif // NATIVEDIALOGMODEL_H
