#include "gnativedialogmodel_android_p.h"

#include <QtAndroidExtras>
#include "gnativedialogmodel.h"

#define fromQString(str) (str.isEmpty() ? NULL : QAndroidJniObject::fromString(str).object())

static const char androidNativeDialogControllerClass[] = "com/groupsinc/NativeDialog/NativeDialogController";

static inline QString toQString(JNIEnv *env, jstring value)
{
    return QString::fromUtf8(env->GetStringUTFChars(value, 0));
}

typedef QMap<quintptr, NativeDialogModelAndroidPrivate *> NativeDialogControllers;
Q_GLOBAL_STATIC(NativeDialogControllers, g_nativeDialogControllers)

#define DIALOG_D(id) \
    const NativeDialogControllers &dialogControllers = *g_nativeDialogControllers; \
    NativeDialogModelPrivate *d = dialogControllers[id]; \
    if (!d) \
        return;


NativeDialogModelPrivate *NativeDialogModelPrivate::create(NativeDialogModel *q)
{
    return new NativeDialogModelAndroidPrivate(q);
}

NativeDialogModelAndroidPrivate::NativeDialogModelAndroidPrivate(NativeDialogModel *q) :
    NativeDialogModelPrivate(q)
  , m_id(reinterpret_cast<quintptr>(this))
{
    m_dialogController = QJNIObjectPrivate(androidNativeDialogControllerClass,
                                           "(Landroid/app/Activity;J)V",
                                           QtAndroidPrivate::activity(),
                                           m_id);
    g_nativeDialogControllers->insert(m_id, this);
}

void NativeDialogModelAndroidPrivate::init()
{
}

void NativeDialogModelAndroidPrivate::alert(const QString &id, const QString &title, const QString &message,
                                            const QString &cancelButtonTitle, const QVariantList &otherButtons,
                                            NativeDialogModel::DialogStyle style)
{
    int i = 0;
    int len = otherButtons.count();

    QAndroidJniEnvironment env;
    jobjectArray jArray = (jobjectArray) env->NewObjectArray(len, env->FindClass("java/lang/String"), env->NewStringUTF(""));
    for (; i < len; i++)
        env->SetObjectArrayElement(jArray, i, env->NewStringUTF(otherButtons.at(i).toString().toUtf8()));

    m_dialogController.callMethod<void>(
                "alert", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;I)V",
                fromQString(id),
                fromQString(title),
                fromQString(message),
                fromQString(cancelButtonTitle),
                jArray,
                static_cast<int>(style));

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

void NativeDialogModelAndroidPrivate::actionSheet(const QString &id, const QString &title, const QString &cancelButtonTitle,
                                                  const QString &destructiveButtonTitle, const QVariantList &otherButtons)
{
    int i = 0;
    int len = otherButtons.count();

    QAndroidJniEnvironment env;
    jobjectArray jArray = (jobjectArray) env->NewObjectArray(len, env->FindClass("java/lang/String"), env->NewStringUTF(""));
    for (; i < len; i++)
        env->SetObjectArrayElement(jArray, i, env->NewStringUTF(otherButtons.at(i).toString().toUtf8()));

    m_dialogController.callMethod<void>(
                "actionSheet", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V",
                fromQString(id),
                fromQString(title),
                fromQString(cancelButtonTitle),
                fromQString(destructiveButtonTitle),
                jArray);

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

void NativeDialogModelAndroidPrivate::openImagePicker(NativeDialogModel::PickerType type)
{
    m_dialogController.callMethod<void>("openImagePicker", "(Z)V", type == NativeDialogModel::Camera ? true : false);
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

static inline void buttonClicked(JNIEnv *env, jclass, jlong id, jstring buttonId, jint buttonIndex, jstring buttonLabel)
{
    DIALOG_D(id);
    emit d->q()->buttonClicked(toQString(env, buttonId), buttonIndex, toQString(env, buttonLabel));
}

static inline void imageSelected(JNIEnv *env, jclass, jlong id, jstring imagePath)
{
    DIALOG_D(id);
    emit d->q()->imageSelected(toQString(env, imagePath), NativeDialogModel::Up);
}

static inline void imageSelectStarted(JNIEnv *, jclass, jlong id)
{
    DIALOG_D(id);
    emit d->q()->imageSelectStarted();
}

static inline void imageSelectCancel(JNIEnv *, jclass, jlong id)
{
    DIALOG_D(id);
    emit d->q()->imageSelectCancel();
}

static inline void imageError(JNIEnv *env, jclass, jlong id, jstring errorMessage)
{
    DIALOG_D(id);
    emit d->q()->imageError(toQString(env, errorMessage));
}

JNINativeMethod jniMethods[] = {
    { "buttonClicked", "(JLjava/lang/String;ILjava/lang/String;)V", (void *) buttonClicked }
};

JNINativeMethod jniActivityMethods[] = {
    { "imageSelected", "(JLjava/lang/String;)V", (void *) imageSelected },
    { "imageSelectStarted", "(J)V", (void *) imageSelectStarted },
    { "imageSelectCancel", "(J)V", (void *) imageSelectCancel },
    { "imageError", "(JLjava/lang/String;)V", (void *) imageError }
};

int JNICALL JNI_OnLoad(JavaVM *vm, void *)
{
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4) != JNI_OK)
        return JNI_FALSE;

    jclass nativeDialogControllerClass = env->FindClass("com/groupsinc/NativeDialog/NativeDialogController");
    if (!nativeDialogControllerClass) {
        qDebug() << "NativeDialogController class not found.";
        return JNI_ERR;
    }

    if (env->RegisterNatives(nativeDialogControllerClass, jniMethods, sizeof(jniMethods) / sizeof(jniMethods[0]))) {
        qDebug() << "NativeDialogController could not register native methods.";
        return JNI_ERR;
    }

    jclass imagePickerActivityClass = env->FindClass("com/groupsinc/NativeDialog/ImagePickerActivity");
    if (!imagePickerActivityClass) {
        qDebug() << "ImagePickerActivity class not found.";
        return JNI_ERR;
    }

    if (env->RegisterNatives(imagePickerActivityClass, jniActivityMethods, sizeof(jniActivityMethods) / sizeof(jniActivityMethods[0]))) {
        qDebug() << "ImagePickerActivity could not register native methods.";
        return JNI_ERR;
    }

    return JNI_VERSION_1_4;
}
