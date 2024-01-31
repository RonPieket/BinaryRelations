#include <QCoreApplication>
#include <QTest>

#include "TestOneToOne.h"
#include "TestOneToMany.h"

int main(int argc, char *argv[])
{
    int status = 0;
    status |= QTest::qExec(new TestOneToOne, argc, argv);
    status |= QTest::qExec(new TestOneToMany, argc, argv);

    return status;
}

#include "TestOneToOne.moc"
#include "TestOneToMany.moc"
