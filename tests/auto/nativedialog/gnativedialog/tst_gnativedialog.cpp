#include <QtTest/QtTest>
#include <GroupsIncNativeDialog/private/gnativedialog_p.h>

class tst_GNativeDialog : public QObject
{
    Q_OBJECT
public:
    tst_GNativeDialog() {}

private slots:
    void initTestCase();

};

void tst_GNativeDialog::initTestCase()
{
}

QTEST_MAIN(tst_GNativeDialog)

#include "tst_gnativedialog.moc"
