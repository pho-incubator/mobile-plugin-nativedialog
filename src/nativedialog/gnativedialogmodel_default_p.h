#ifndef NATIVEDIALOGMODEL_DEFAULT_P_H
#define NATIVEDIALOGMODEL_DEFAULT_P_H

#include "gnativedialogmodel_p.h"
#include "gnativedialogmodel.h"

class NativeDialogModelDefaultPrivate : public NativeDialogModelPrivate
{
    Q_OBJECT
public:
    NativeDialogModelDefaultPrivate(NativeDialogModel *q);

    void init();

    void alert(const QString &id, const QString &title, const QString &message,
                           const QString &cancelButtonTitle = tr("OK"),
                           const QVariantList &otherButtons = QVariantList(),
                           NativeDialogModel::DialogStyle style = NativeDialogModel::Default);

    void actionSheet(const QString &id, const QString &title, const QString &cancelButtonTitle = tr("Cancel"),
                                 const QString &destructiveButtonTitle = QString(),
                                 const QVariantList &otherButtons = QVariantList());

    void openImagePicker(NativeDialogModel::PickerType type);
};

#endif // MAILMODEL_DEFAULT_P_H
