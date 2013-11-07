#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "PChecksum.h"
#include <zlib.h>

using namespace Pt::Core;

class PtCoreTest : public QObject
{
    Q_OBJECT

public:
    PtCoreTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCrc();
    void testCrc_data();
};

PtCoreTest::PtCoreTest()
{
}

void PtCoreTest::initTestCase()
{
}

void PtCoreTest::cleanupTestCase()
{
}

void PtCoreTest::testCrc()
{
    QFETCH(QByteArray, raw);
    QFETCH(QByteArray, hashed);
    PChecksum c(PChecksum::Algorithm::Crc32);
    c.addData(raw);
    QCOMPARE(c.result(), hashed);
}

void PtCoreTest::testCrc_data()
{
    QTest::addColumn<QByteArray>("raw");
    QTest::addColumn<QByteArray>("hashed");
    QTest::newRow("empty") << QByteArray() << QByteArray::fromHex("00000000");
    QTest::newRow("one 0") << QByteArray(1, '\0') << QByteArray::fromHex("d202ef8d");
    QTest::newRow("one 1") << QByteArray(1, '\1') << QByteArray::fromHex("a505df1b");
    QTest::newRow("a") << QByteArray("a") << QByteArray::fromHex("e8b7be43");
    QTest::newRow("normal") << QByteArray("abcd") << QByteArray::fromHex("ed82cd11");
}

QTEST_MAIN(PtCoreTest)

#include "tst_PtCoreTest.moc"
