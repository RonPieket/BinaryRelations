#pragma once

#include <QTest>

#include "BinaryRelations/OneToOne.h"

using namespace BinaryRelations;

class TestOneToOne : public QObject
{
    Q_OBJECT

private:
    enum ObjectId
    {
        kNone = 0,
        kJill,
        kZeke,
        kSean,
        kXena,
    };

    OneToOne<ObjectId, QString> m_IdToUniqueName;

private slots:
    void init()
    {
        m_IdToUniqueName.insert(kJill, "JILL");
        m_IdToUniqueName.insert(kZeke, "ZEKE");
        m_IdToUniqueName.insert(kSean, "SEAN");
        m_IdToUniqueName.insert(kXena, "XENA");
    }

    void Insert()
    {
        QVERIFY(m_IdToUniqueName.contains(kJill, "JILL"));
        QVERIFY(m_IdToUniqueName.contains(kZeke, "ZEKE"));
        QVERIFY(m_IdToUniqueName.contains(kSean, "SEAN"));
        QVERIFY(m_IdToUniqueName.contains(kXena, "XENA"));
    }

    void InsertOverwrite()
    {
        m_IdToUniqueName.insert(kZeke, "ALEX");

        QVERIFY(!m_IdToUniqueName.contains(kZeke, "ZEKE"));
        QVERIFY(m_IdToUniqueName.contains(kZeke, "ALEX"));
    }

    void Remove()
    {
        m_IdToUniqueName.remove(kJill, "JILL");

        QVERIFY(!m_IdToUniqueName.contains(kJill, "JILL"));
        QVERIFY( m_IdToUniqueName.contains(kZeke, "ZEKE"));
        QVERIFY( m_IdToUniqueName.contains(kSean, "SEAN"));
        QVERIFY( m_IdToUniqueName.contains(kXena, "XENA"));
    }

    void RemoveLeft()
    {
        m_IdToUniqueName.removeLeft(kJill);

        QVERIFY(!m_IdToUniqueName.contains(kJill, "JILL"));
        QVERIFY( m_IdToUniqueName.contains(kZeke, "ZEKE"));
        QVERIFY( m_IdToUniqueName.contains(kSean, "SEAN"));
        QVERIFY( m_IdToUniqueName.contains(kXena, "XENA"));
    }

    void RemoveRight()
    {
        m_IdToUniqueName.removeRight("JILL");

        QVERIFY(!m_IdToUniqueName.contains(kJill, "JILL"));
        QVERIFY( m_IdToUniqueName.contains(kZeke, "ZEKE"));
        QVERIFY( m_IdToUniqueName.contains(kSean, "SEAN"));
        QVERIFY( m_IdToUniqueName.contains(kXena, "XENA"));
    }

    void Clear()
    {
        m_IdToUniqueName.clear();
    }
};
