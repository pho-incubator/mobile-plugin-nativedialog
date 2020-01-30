#ifndef NATIVEDIALOGMODEL_IOS_P_H
#define NATIVEDIALOGMODEL_IOS_P_H

#include "gnativedialogmodel_p.h"

class NativeDialogModelIosPrivate : public NativeDialogModelPrivate
{
    Q_OBJECT
public:
    explicit NativeDialogModelIosPrivate(NativeDialogModel *q);
    virtual ~NativeDialogModelIosPrivate();

    void init();

    void alert(const QString &uid, const QString &title, const QString &message,
                           const QString &cancelButtonTitle = tr("OK"),
                           const QVariantList &otherButtons = QVariantList(),
                           NativeDialogModel::DialogStyle style = NativeDialogModel::Default);

    void actionSheet(const QString &uid, const QString &title, const QString &cancelButtonTitle = tr("Cancel"),
                                 const QString &destructiveButtonTitle = QString(),
                                 const QVariantList &otherButtons = QVariantList());

    void openImagePicker(NativeDialogModel::PickerType type);

private:
    void *m_delegate;
    void *m_imagePickerDelegate;
};

#endif // NATIVEDIALOGMODEL_IOS_P_H
