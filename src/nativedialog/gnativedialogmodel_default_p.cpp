#include "gnativedialogmodel_default_p.h"

NativeDialogModelPrivate *NativeDialogModelPrivate::create(NativeDialogModel *q)
{
    return new NativeDialogModelDefaultPrivate(q);
}

NativeDialogModelDefaultPrivate::NativeDialogModelDefaultPrivate(NativeDialogModel *q) :
    NativeDialogModelPrivate(q)
{
}

void NativeDialogModelDefaultPrivate::init()
{
}

void NativeDialogModelDefaultPrivate::alert(const QString &id, const QString &title, const QString &message,
                                            const QString &cancelButtonTitle, const QVariantList &otherButtons,
                                            NativeDialogModel::DialogStyle style)
{
    Q_UNUSED(id);
    Q_UNUSED(title);
    Q_UNUSED(message);
    Q_UNUSED(cancelButtonTitle);
    Q_UNUSED(otherButtons);
    Q_UNUSED(style);
}

void NativeDialogModelDefaultPrivate::actionSheet(const QString &id, const QString &title, const QString &cancelButtonTitle,
                                                  const QString &destructiveButtonTitle, const QVariantList &otherButtons)
{
    Q_UNUSED(id);
    Q_UNUSED(title);
    Q_UNUSED(cancelButtonTitle);
    Q_UNUSED(destructiveButtonTitle)
    Q_UNUSED(otherButtons);
}

void NativeDialogModelDefaultPrivate::openImagePicker(NativeDialogModel::PickerType type)
{
    Q_UNUSED(type);
}
