#pragma once

#include <QTest>

#include "BinaryRelations/OneToMany.h"

using namespace BinaryRelations;

class TestOneToMany : public QObject
{
    Q_OBJECT

private:
    enum ObjectId
    {
        kObjectIdNull = 0,
        kJill,
        kZeke,
        kSean,
        kXena,

        kChevy,
        kToyota,
    };

    enum ObjectType
    {
        kObjectTypeNull = 0,
        kVehicle,
        kPerson,
    };

    OneToMany<ObjectId, ObjectId> m_VehicleToPassengers;
    OneToMany<ObjectType, ObjectId> m_TypeToObjects;

private slots:
    void init()
    {
        m_VehicleToPassengers.clear();
        m_VehicleToPassengers.insert(kChevy, kJill);
        m_VehicleToPassengers.insert(kChevy, kZeke);
        m_VehicleToPassengers.insert(kToyota, kSean);

        m_TypeToObjects.clear();
        m_TypeToObjects.insert(kPerson, kJill);
        m_TypeToObjects.insert(kPerson, kZeke);
        m_TypeToObjects.insert(kPerson, kSean);
        m_TypeToObjects.insert(kPerson, kXena);
        m_TypeToObjects.insert(kVehicle, kChevy);
        m_TypeToObjects.insert(kVehicle, kToyota);
    }

    void Insert()
    {
        QVERIFY(m_VehicleToPassengers.contains(kChevy, kJill));
        QVERIFY(m_VehicleToPassengers.contains(kChevy, kZeke));
        QVERIFY(m_VehicleToPassengers.contains(kToyota, kSean));

        QVERIFY(m_TypeToObjects.contains(kPerson, kJill));
        QVERIFY(m_TypeToObjects.contains(kPerson, kZeke));
        QVERIFY(m_TypeToObjects.contains(kPerson, kSean));
        QVERIFY(m_TypeToObjects.contains(kPerson, kXena));
        QVERIFY(m_TypeToObjects.contains(kVehicle, kChevy));
        QVERIFY(m_TypeToObjects.contains(kVehicle, kToyota));
    }

    void InsertOverwrite()
    {
        m_VehicleToPassengers.insert(kChevy, kSean);

        QVERIFY(!m_VehicleToPassengers.contains(kToyota, kSean));
        QVERIFY( m_VehicleToPassengers.contains(kChevy, kSean));
    }

    void Remove()
    {
        m_VehicleToPassengers.remove(kToyota, kSean);

        QVERIFY(!m_VehicleToPassengers.contains(kToyota, kSean));
        QVERIFY(m_VehicleToPassengers.contains(kChevy, kJill));
        QVERIFY(m_VehicleToPassengers.contains(kChevy, kZeke));
    }

    void RemoveLeft()
    {
        m_VehicleToPassengers.removeLeft(kChevy);

        QVERIFY(m_VehicleToPassengers.contains(kToyota, kSean));
        QVERIFY(!m_VehicleToPassengers.contains(kChevy, kJill));
        QVERIFY(!m_VehicleToPassengers.contains(kChevy, kZeke));
    }

    void RemoveRight()
    {
        m_VehicleToPassengers.removeRight(kJill);

        QVERIFY(m_VehicleToPassengers.contains(kToyota, kSean));
        QVERIFY(!m_VehicleToPassengers.contains(kChevy, kJill));
        QVERIFY(m_VehicleToPassengers.contains(kChevy, kZeke));
    }

    void Clear()
    {
        m_VehicleToPassengers.clear();
    }

    void Merge()
    {
        OneToMany<ObjectId, ObjectId> other;
        other.insert(kToyota, kZeke);
        other.insert(kToyota, kXena);
        m_VehicleToPassengers.merge(other);

        QVERIFY(m_VehicleToPassengers.contains(kChevy, kJill));
        QVERIFY(!m_VehicleToPassengers.contains(kChevy, kZeke));
        QVERIFY(m_VehicleToPassengers.contains(kToyota, kZeke));
        QVERIFY(m_VehicleToPassengers.contains(kToyota, kSean));
        QVERIFY(m_VehicleToPassengers.contains(kToyota, kXena));
    }
};
