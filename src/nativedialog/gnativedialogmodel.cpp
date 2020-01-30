#include "gnativedialogmodel.h"
#include "gnativedialogmodel_p.h"
#include <QDebug>

const char *NativeDialogModel::NAME = "NativeDialogModel";

NativeDialogModel::NativeDialogModel(MvcFacade *parent)
    : MvcModel(parent)
    , d_ptr(NativeDialogModelPrivate::create(this))
{
}

NativeDialogModel::~NativeDialogModel()
{
    delete d_ptr;
}

void NativeDialogModel::init()
{
    Q_D(NativeDialogModel);
    d->init();

    qRegisterMetaType<NativeDialogModel::ImageOrientation>("ImageOrientation");
    qDebug() << "NativeDialogModel initialized.";
}

void NativeDialogModel::alert(const QString &id, const QString &title, const QString &message,
                              const QString &cancelButtonTitle, const QVariantList &otherButtons,
                              NativeDialogModel::DialogStyle style)
{
    Q_D(NativeDialogModel);
    d->alert(id, title, message, cancelButtonTitle, otherButtons, style);
}

void NativeDialogModel::actionSheet(const QString &id, const QString &title, const QString &cancelButtonTitle,
                                    const QString &destructiveButtonTitle, const QVariantList &otherButtons)
{
    Q_D(NativeDialogModel);
    d->actionSheet(id, title, cancelButtonTitle, destructiveButtonTitle, otherButtons);
}

void NativeDialogModel::openImagePicker(NativeDialogModel::PickerType type)
{
    Q_D(NativeDialogModel);
    d->openImagePicker(type);
}

const char *NativeDialogModel::name()
{
    return NAME;
}

void NativeDialogModel::apply(const QVariantMap &config)
{
    Q_UNUSED(config);
}

