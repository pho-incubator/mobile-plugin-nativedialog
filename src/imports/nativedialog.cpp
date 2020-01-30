#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>
#include <GroupsIncNativeDialog/gnativedialogmodel.h>
#include <GroupsIncMvc/gmvcfacade.h>

QT_BEGIN_NAMESPACE

GMVC_DEFINE_MODEL(NativeDialogModel)

class GNativeDialogModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("GroupsIncNativeDialog"));

        // @uri GroupsIncNativeDialog
        qmlRegisterSingletonType<NativeDialogModel>(uri, 1, 0, "NativeDialogModel", GMVC_MODEL(NativeDialogModel));
    }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(uri);
        Q_UNUSED(engine);
    }
};

QT_END_NAMESPACE

#include "nativedialog.moc"



