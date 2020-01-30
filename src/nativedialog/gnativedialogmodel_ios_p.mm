#include "gnativedialogmodel_ios_p.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <MobileCoreServices/UTCoreTypes.h>
#import <AssetsLibrary/AssetsLibrary.h>

#include <qpa/qplatformnativeinterface.h>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQuickWindow>
#include <GroupsIncMvc/gmvcfacade.h>

static NSString *toNSString(const QString &qString)
{
    QByteArray b = qString.toUtf8();
    NSData *d = b.toNSData();
    return qString.isEmpty() ? nil : [[NSString alloc] initWithData:d encoding:NSUTF8StringEncoding];
    // return qString.isEmpty() ? nil : [[NSString alloc] initWithFormat:@"%s", b.data()];
}

@interface NativeImagePickerControllerDelegate : UIViewController <UIImagePickerControllerDelegate, UINavigationControllerDelegate>
{
    NativeDialogModelIosPrivate *m_nativeDialogController;
    UIView *m_parentView;
}
@end

@implementation NativeImagePickerControllerDelegate

- (id)initWithNativeController:(NativeDialogModelIosPrivate *)nativeDialogController andUIView:(UIView *)parentView
{
    self = [super init];
    if (self) {
        m_nativeDialogController = nativeDialogController;
        m_parentView = parentView;
    }

    return self;
}

- (void)openImagePicker:(UIImagePickerControllerSourceType)sourceType
{
    UIImagePickerController *picker = [[[UIImagePickerController alloc] init] autorelease];
    picker.delegate = self;
    picker.allowsEditing = NO;
    picker.sourceType = sourceType;
    picker.mediaTypes = @[(NSString *) kUTTypeImage];

    [[[m_parentView window] rootViewController] presentViewController:picker animated:YES completion:nil];
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    UIImage *chosenImage = info[UIImagePickerControllerOriginalImage];
    NSURL *url = info[UIImagePickerControllerReferenceURL];
    NSLog(@"Media URL %@", url);

    [picker dismissViewControllerAnimated:YES completion:nil];
    emit m_nativeDialogController->q_ptr->imageSelectStarted();

    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 300 * NSEC_PER_MSEC), dispatch_get_main_queue(), ^{
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *filePath = [[paths objectAtIndex:0] stringByAppendingPathComponent:@"Image.jpg"];

        NSLog(@"Image library saved Path %@", filePath);

        // Save image.
        NSData *imageContext = UIImageJPEGRepresentation(chosenImage, .75);
        [imageContext writeToFile:filePath atomically:YES];

        NativeDialogModel::ImageOrientation orientation = NativeDialogModel::Up;
        switch (chosenImage.imageOrientation) {
            case UIImageOrientationUpMirrored:
                orientation = NativeDialogModel::UpMirrored;
                break;
            case UIImageOrientationDownMirrored:
                orientation = NativeDialogModel::DownMirrored;
                break;
            case UIImageOrientationLeftMirrored:
                orientation = NativeDialogModel::LeftMirrored;
                break;
            case UIImageOrientationRightMirrored:
                orientation = NativeDialogModel::RightMirrored;
                break;
            case UIImageOrientationUp:
                orientation = NativeDialogModel::Up;
                break;
            case UIImageOrientationDown:
                orientation = NativeDialogModel::Down;
                break;
            case UIImageOrientationLeft:
                orientation = NativeDialogModel::Left;
                break;
            case UIImageOrientationRight:
                orientation = NativeDialogModel::Right;
                break;
        }

        emit m_nativeDialogController->q_ptr->imageSelected(QString::fromNSString(filePath), orientation);
        if (url == nil) {
            ALAssetsLibrary *library = [[ALAssetsLibrary alloc] init];
            [library writeImageToSavedPhotosAlbum:chosenImage.CGImage orientation:(ALAssetOrientation)chosenImage.imageOrientation completionBlock:nil];
            [library release];

            NSLog(@"Photo saved into album.");
        }

        // emit m_nativeDialogController->q_ptr->imageSelected();
    });
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissViewControllerAnimated:YES completion:nil];
    emit m_nativeDialogController->q_ptr->imageSelectCancel();
}

@end

@interface NativeDialogControllerDelegate : NSObject <UIActionSheetDelegate, UIAlertViewDelegate>
{
    NativeDialogModelIosPrivate *m_nativeDialogController;
    UIView *m_parentView;
    NSString *m_id;
}
@end

@implementation NativeDialogControllerDelegate

- (id)initWithNativeController:(NativeDialogModelIosPrivate *)nativeDialogController andUIView:(UIView *)parentView
{
    self = [super init];
    if (self) {
        m_nativeDialogController = nativeDialogController;
        m_id = nil;
        m_parentView = parentView;
    }

    return self;
}

- (void)alert:(NSString *)uid title:(NSString *)title message:(NSString *)message cancelButtonTitle:(NSString *)cancelButtonTitle
      otherButtons:(NSArray *) otherButtons  style:(UIAlertViewStyle) style
{
    if (m_id != nil)
        [m_id release];

    m_id = uid;
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:title
                                                           message:message
                                                          delegate:self
                                                 cancelButtonTitle:cancelButtonTitle
                                                 otherButtonTitles:nil];
    if (otherButtons != nil)
        for (NSString *title in otherButtons)
            [alertView addButtonWithTitle:title];

    [alertView setAlertViewStyle:style];

    [alertView show];
    [alertView autorelease];
}

- (void)actionSheet:(NSString *)uid title:(NSString *)title cancelButtonTitle:(NSString *)cancelButtonTitle
      destructiveButtonTitle:(NSString *)destructiveButtonTitle
      otherButtons:(NSArray *) otherButtons
{
    if (m_id != nil)
        [m_id release];

    m_id = uid;
    UIActionSheet *actionSheet = [[UIActionSheet alloc] initWithTitle:title
                                                                 delegate:self
                                                        cancelButtonTitle:cancelButtonTitle
                                                   destructiveButtonTitle:destructiveButtonTitle
                                                        otherButtonTitles:nil];

    if (otherButtons != nil)
        for (NSString *title in otherButtons)
            [actionSheet addButtonWithTitle:title];

    [actionSheet showInView:m_parentView];
    [actionSheet autorelease];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (alertView.alertViewStyle == UIAlertViewStyleSecureTextInput
            || alertView.alertViewStyle == UIAlertViewStylePlainTextInput) {
        emit m_nativeDialogController->q_ptr->buttonClickedWithText(m_id != nil ? QString::fromNSString(m_id) : QString(), buttonIndex,
                                                                    QString::fromNSString([alertView buttonTitleAtIndex:buttonIndex]),
                                                                    QString::fromNSString([alertView textFieldAtIndex:0].text));
    } else if (alertView.alertViewStyle == UIAlertViewStyleLoginAndPasswordInput) {
        emit m_nativeDialogController->q_ptr->buttonClickedWithLogin(m_id != nil ? QString::fromNSString(m_id) : QString(), buttonIndex,
                                                                     QString::fromNSString([alertView buttonTitleAtIndex:buttonIndex]),
                                                                     QString::fromNSString([alertView textFieldAtIndex:0].text),
                                                                     QString::fromNSString([alertView textFieldAtIndex:1].text));
    } else {
        emit m_nativeDialogController->q_ptr->buttonClicked(m_id != nil ? QString::fromNSString(m_id) : QString(), buttonIndex,
                                                            QString::fromNSString([alertView buttonTitleAtIndex:buttonIndex]));
    }
}

- (void)alertViewCancel:(UIAlertView *)alertView
{
    Q_UNUSED(alertView);
    emit m_nativeDialogController->q_ptr->canceled(m_id != nil ? QString::fromNSString(m_id) : QString());
}

- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
    emit m_nativeDialogController->q_ptr->beforeClose(m_id != nil ? QString::fromNSString(m_id) : QString(), buttonIndex,
                                                      QString::fromNSString([alertView buttonTitleAtIndex:buttonIndex]));
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    emit m_nativeDialogController->q_ptr->closed(m_id != nil ? QString::fromNSString(m_id) : QString(), buttonIndex,
                                                        QString::fromNSString([alertView buttonTitleAtIndex:buttonIndex]));
}

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    emit m_nativeDialogController->q_ptr->buttonClicked(m_id != nil ? QString::fromNSString(m_id) : QString(), buttonIndex,
                                                        QString::fromNSString([actionSheet buttonTitleAtIndex:buttonIndex]));
}

- (void)actionSheetCancel:(UIActionSheet *)actionSheet
{
    Q_UNUSED(actionSheet);
    emit m_nativeDialogController->q_ptr->canceled(QString::fromNSString(m_id));
}

- (void)actionSheet:(UIActionSheet *)actionSheet willDismissWithButtonIndex:(NSInteger)buttonIndex
{
    emit m_nativeDialogController->q_ptr->beforeClose(m_id != nil ? QString::fromNSString(m_id) : QString(), buttonIndex,
                                                      QString::fromNSString([actionSheet buttonTitleAtIndex:buttonIndex]));
}

- (void)actionSheet:(UIActionSheet *)actionSheet didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    emit m_nativeDialogController->q_ptr->closed(m_id != nil ? QString::fromNSString(m_id) : QString(), buttonIndex,
                                                 QString::fromNSString([actionSheet buttonTitleAtIndex:buttonIndex]));
}

@end

NativeDialogModelPrivate *NativeDialogModelPrivate::create(NativeDialogModel *q)
{
    return new NativeDialogModelIosPrivate(q);
}

NativeDialogModelIosPrivate::NativeDialogModelIosPrivate(NativeDialogModel *q)
    : NativeDialogModelPrivate(q)
    , m_delegate(0)
    , m_imagePickerDelegate(0)
{
}

NativeDialogModelIosPrivate::~NativeDialogModelIosPrivate()
{
    NativeDialogControllerDelegate *delegate = id(m_delegate);
    [delegate release];
    m_delegate = 0;

    NativeImagePickerControllerDelegate *imageDelegate = id(m_imagePickerDelegate);
    [imageDelegate release];
    m_imagePickerDelegate = 0;
}

void NativeDialogModelIosPrivate::init()
{
    Q_Q(NativeDialogModel);
    MvcFacade *facade = qobject_cast<MvcFacade *>(q->parent());
    QObject *top = qobject_cast<QQmlApplicationEngine *>(facade->engine())->rootObjects().at(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(top);
    UIView *view = static_cast<UIView *>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("uiview", window));

    if (!m_delegate)
        m_delegate = [[NativeDialogControllerDelegate alloc] initWithNativeController:this andUIView:view];

    if (!m_imagePickerDelegate)
        m_imagePickerDelegate = [[NativeImagePickerControllerDelegate alloc] initWithNativeController:this andUIView:view];

}

void NativeDialogModelIosPrivate::alert(const QString &uid, const QString &title, const QString &message,
                                            const QString &cancelButtonTitle, const QVariantList &otherButtons,
                                            NativeDialogModel::DialogStyle style)
{
    NSMutableArray *buttons = nil;
    if (!otherButtons.isEmpty()) {
        buttons = [[NSMutableArray alloc] init];
        int i = 0;
        int len = otherButtons.length();
        for (; i < len; i++)
            if (otherButtons.at(i).type() == QVariant::String)
                [buttons addObject:toNSString(otherButtons.at(i).toString())];
    }

    UIAlertViewStyle nativeStyle;
    switch (style) {
        case NativeDialogModel::PlainTextInput:
            nativeStyle = UIAlertViewStylePlainTextInput;
            break;
        case NativeDialogModel::SecureTextInput:
            nativeStyle = UIAlertViewStyleSecureTextInput;
            break;
        case NativeDialogModel::Login:
            nativeStyle = UIAlertViewStyleLoginAndPasswordInput;
            break;
        default:
            nativeStyle = UIAlertViewStyleDefault;
    }

    NativeDialogControllerDelegate *delegate = id(m_delegate);
    [delegate alert:toNSString(uid)
                                    title:toNSString(title)
                                    message:toNSString(message)
                                    cancelButtonTitle:toNSString(cancelButtonTitle)
                                    otherButtons:buttons
                                    style:nativeStyle];
}

void NativeDialogModelIosPrivate::actionSheet(const QString &uid, const QString &title, const QString &cancelButtonTitle,
                                                  const QString &destructiveButtonTitle, const QVariantList &otherButtons)
{
    NSMutableArray *buttons = nil;
    if (!otherButtons.isEmpty()) {
        buttons = [[NSMutableArray alloc] init];
        int i = 0;
        int len = otherButtons.length();
        for (; i < len; i++)
            if (otherButtons.at(i).type() == QVariant::String)
                [buttons addObject:toNSString(otherButtons.at(i).toString())];
    }

    NativeDialogControllerDelegate *delegate = id(m_delegate);
    [delegate actionSheet:toNSString(uid)
                                            title:toNSString(title)
                                            cancelButtonTitle:toNSString(cancelButtonTitle)
                                            destructiveButtonTitle:toNSString(destructiveButtonTitle)
                                            otherButtons:buttons];
}

void NativeDialogModelIosPrivate::openImagePicker(NativeDialogModel::PickerType type)
{
    NativeImagePickerControllerDelegate *delegate = id(m_imagePickerDelegate);
    UIImagePickerControllerSourceType sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    if (type == NativeDialogModel::Camera)
        sourceType = UIImagePickerControllerSourceTypeCamera;
    else if (type == NativeDialogModel::PhotoAlbum)
        sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
    else if (type != NativeDialogModel::PhotoLibrary)
        qDebug() << "Invalid Picker Source Type" << type;

    if (![UIImagePickerController isSourceTypeAvailable:sourceType]) {
        qDebug() << "Source type is not available." << type;
        return;
    }

    [delegate openImagePicker:sourceType];
}
