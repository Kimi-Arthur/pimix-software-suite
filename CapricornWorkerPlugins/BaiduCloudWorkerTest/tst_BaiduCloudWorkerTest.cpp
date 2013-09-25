#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "BaiduCloudWorker.h"

class BaiduCloudWorkerTest : public QObject
{
    Q_OBJECT

public:
    BaiduCloudWorkerTest();


private Q_SLOTS:
    void uploadFileDirectTest();
    void uploadFileByBlockTest();
    void cleanup();
};

BaiduCloudWorkerTest::BaiduCloudWorkerTest()
{
}

void BaiduCloudWorkerTest::uploadFileDirectTest()
{
    BaiduCloudWorker w;
    QCOMPARE(w.uploadFile("test/test.asdfg", "U:/main.cpp"), CapricornWorker::Success);
}

void BaiduCloudWorkerTest::uploadFileByBlockTest()
{
    BaiduCloudWorker w;
    QCOMPARE(w.uploadFile("test/VTS_01_0.vob", "U:/VTS_01_0.vob"), CapricornWorker::Success);
}

void BaiduCloudWorkerTest::cleanup()
{
    BaiduCloudWorker w;
    w.removePath("test");
}


QTEST_MAIN(BaiduCloudWorkerTest)

#include "tst_BaiduCloudWorkerTest.moc"
