#ifndef NATIVEDIALOGMODEL_P_H
#define NATIVEDIALOGMODEL_P_H

#include <QObject>
#include "gnativedialogmodel.h"

class NativeDialogModelPrivate : public QObject
{
    Q_OBJECT
public:
    NativeDialogModelPrivate(NativeDialogModel *q)
        : QObject(0)
        , q_ptr(q) {}

    static NativeDialogModelPrivate *create(NativeDialogModel *q);

    virtual void init() = 0;

    virtual void alert(const QString &id, const QString &title, const QString &message,
                           const QString &cancelButtonTitle = tr("OK"),
                           const QVariantList &otherButtons = QVariantList(),
                           NativeDialogModel::DialogStyle style = NativeDialogModel::Default) = 0;

    virtual void actionSheet(const QString &id, const QString &title, const QString &cancelButtonTitle = tr("Cancel"),
                                 const QString &destructiveButtonTitle = QString(),
                                 const QVariantList &otherButtons = QVariantList()) = 0;

    virtual void openImagePicker(NativeDialogModel::PickerType type) = 0;

    NativeDialogModel *q() { return q_ptr; }

public:
    Q_DECLARE_PUBLIC(NativeDialogModel)
    NativeDialogModel *q_ptr;
};

#endif // NATIVEDIALOGMODEL_P_H
