// SPDX-FileCopyrightText: 2022-2025 Paul Colby <git@colby.id.au>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "testsetnamecommand.h"
#include "outputstreamcapture.h"
#include "testdata.h"
#include "../stringliterals_p.h"

#include "setnamecommand.h"

#include <iostream>
#include <sstream>

Q_DECLARE_METATYPE(AbstractCommand::OutputFormat)

DOKIT_USE_STRINGLITERALS

class MockDeviceCommand : public DeviceCommand
{
public:
    explicit MockDeviceCommand(QObject * const parent = nullptr) : DeviceCommand(parent)
    {

    }

    AbstractPokitService * getService() override
    {
        return nullptr;
    }
};

void TestSetNameCommand::requiredOptions()
{
    SetNameCommand command(this);
    MockDeviceCommand mock;
    QCommandLineParser parser;
    const QStringList expected = mock.requiredOptions(parser) +
        QStringList{ u"new-name"_s };
    QCOMPARE(command.requiredOptions(parser), expected);
}

void TestSetNameCommand::supportedOptions()
{
    SetNameCommand command(this);
    MockDeviceCommand mock;
    QCommandLineParser parser;
    const QStringList expected = command.requiredOptions(parser) + mock.supportedOptions(parser);
    QCOMPARE(command.supportedOptions(parser), expected);
}

void TestSetNameCommand::processOptions_data()
{
    QTest::addColumn<QStringList>("arguments");
    QTest::addColumn<QString>("expected");
    QTest::addColumn<QStringList>("errors");

    QTest::addRow("missing-new-name")
        << QStringList{}
        << QString()
        << QStringList{ u"Missing required option: new-name"_s };

    QTest::addRow("empty-new-name")
        << QStringList{ u"--new-name"_s, u""_s }
        << u""_s
        << QStringList{ u"New name cannot be empty."_s };

    QTest::addRow("valid-new-name")
        << QStringList{ u"--new-name"_s, u"valid"_s }
        << u"valid"_s
        << QStringList{};

    QTest::addRow("11-char-new-name")
        << QStringList{ u"--new-name"_s, QString(11, 'A'_L1) }
        << u"AAAAAAAAAAA"_s
        << QStringList{};

    QTest::addRow("12-char-new-name")
        << QStringList{ u"--new-name"_s, QString(12, 'A'_L1) }
        << u"AAAAAAAAAAAA"_s
        << QStringList{ u"New name cannot exceed 11 characters."_s };
}

void TestSetNameCommand::processOptions()
{
    QFETCH(QStringList, arguments);
    QFETCH(QString, expected);
    QFETCH(QStringList, errors);

    arguments.prepend(u"dokit"_s); // The first argument is always the app name.

    QCommandLineParser parser;
    parser.addOption({u"new-name"_s, u"description"_s, u"name"_s});
    parser.process(arguments);

    SetNameCommand command(this);
    QCOMPARE(command.processOptions(parser), errors);
    QCOMPARE(command.newName, expected);
}

void TestSetNameCommand::getService()
{
    // Unable to safely invoke SetNameCommand::getService() without a valid Bluetooth device.
}

void TestSetNameCommand::serviceDetailsDiscovered()
{
    // Unable to safely invoke SetNameCommand::serviceDetailsDiscovered() without a valid service.
}

void TestSetNameCommand::deviceNameWritten_data()
{
    QTest::addColumn<AbstractCommand::OutputFormat>("format");
    QTest::newRow("1.csv") << AbstractCommand::OutputFormat::Csv;
    QTest::newRow("1.json") << AbstractCommand::OutputFormat::Json;
    QTest::newRow("1.txt") << AbstractCommand::OutputFormat::Text;
}

void TestSetNameCommand::deviceNameWritten()
{
    QFETCH(AbstractCommand::OutputFormat, format);
    LOADTESTDATA(expected);

    OutputStreamCapture capture(&std::cout);
    SetNameCommand command;
    command.format = format;
    command.deviceNameWritten();
    QCOMPARE(QByteArray::fromStdString(capture.data()), expected);
}

void TestSetNameCommand::tr()
{
    // Exercise the inline tr() function (added by the Q_OBJECT macro) to avoid false negatives in
    // test coverage.  There is no need to actually test tr() here, since its part of the Qt API.
    SetNameCommand command;
    QVERIFY(!command.tr("ignored").isEmpty());
}

QTEST_MAIN(TestSetNameCommand)
