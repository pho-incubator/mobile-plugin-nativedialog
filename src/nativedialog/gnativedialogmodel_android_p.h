#ifndef NATIVEDIALOGMODEL_ANDROID_P_H
#define NATIVEDIALOGMODEL_ANDROID_P_H

#include "gnativedialogmodel_p.h"
#include "gnativedialogmodel.h"
#include <QtCore/private/qjnihelpers_p.h>
#include <QtCore/private/qjni_p.h>
#include <QDebug>

class NativeDialogModelAndroidPrivate : public NativeDialogModelPrivate
{
    Q_OBJECT
public:
    NativeDialogModelAndroidPrivate(NativeDialogModel *q);

    void init();

    void alert(const QString &id, const QString &title, const QString &message,
                           const QString &cancelButtonTitle = tr("OK"),
                           const QVariantList &otherButtons = QVariantList(),
                           NativeDialogModel::DialogStyle style = NativeDialogModel::Default);

    void actionSheet(const QString &id, const QString &title, const QString &cancelButtonTitle = tr("Cancel"),
                                 const QString &destructiveButtonTitle = QString(),
                                 const QVariantList &otherButtons = QVariantList());

    void openImagePicker(NativeDialogModel::PickerType type);

private:
    quintptr m_id;
    QJNIObjectPrivate m_dialogController;
};

#endif // NATIVEDIALOGMODEL_DEFAULT_P_H
