// SPDX-FileCopyrightText: 2022-2025 Paul Colby <git@colby.id.au>
// SPDX-License-Identifier: LGPL-3.0-or-later

/*!
 * \file
 * Declares the DeviceInfoService class.
 */

#ifndef QTPOKIT_INFOSERVICE_H
#define QTPOKIT_INFOSERVICE_H

#include "abstractpokitservice.h"

#include <QBluetoothAddress>
#include <QBluetoothUuid>
#include <QVersionNumber>

QTPOKIT_BEGIN_NAMESPACE

class DeviceInfoServicePrivate;

class QTPOKIT_EXPORT DeviceInfoService : public AbstractPokitService
{
    Q_OBJECT

public:
    /// UUID of the "Device Info" service.
    static inline const QBluetoothUuid serviceUuid
        { QBluetoothUuid::ServiceClassUuid::DeviceInformation }; // 0x180a

    /// Characteristics available via the `Device Info` service.
    struct QTPOKIT_EXPORT CharacteristicUuids {
        /// UUID of the `Device Info` service's `Manufacturer Name String` characterstic.
        static inline const QBluetoothUuid manufacturerName
            { QBluetoothUuid::CharacteristicType::ManufacturerNameString }; // 0x2a29

        /// UUID of the `Device Info` service's `Model Number String` characterstic.
        static inline const QBluetoothUuid modelNumber
            { QBluetoothUuid::CharacteristicType::ModelNumberString }; // 0x2a24

        /// UUID of the `Device Info` service's `Firmware Revision String` characterstic.
        static inline const QBluetoothUuid firmwareRevision
            { QBluetoothUuid::CharacteristicType::FirmwareRevisionString }; // 0x2a26

        /// UUID of the `Device Info` service's `Hardware Revision String` characterstic.
        static inline const QBluetoothUuid hardwareRevision
            { QBluetoothUuid::CharacteristicType::HardwareRevisionString }; // 0x2a27

        /// UUID of the `Device Info` service's `Software Revision String` characterstic.
        static inline const QBluetoothUuid softwareRevision
            { QBluetoothUuid::CharacteristicType::SoftwareRevisionString }; // 0x2a28

        /// UUID of the `Device Info` service's `Serial Number String` characterstic.
        static inline const QBluetoothUuid serialNumber
            { QBluetoothUuid::CharacteristicType::SerialNumberString }; // 0x2a25
    };

    DeviceInfoService(QLowEnergyController * const pokitDevice, QObject * parent = nullptr);
    ~DeviceInfoService() = default;

    bool readCharacteristics() override;
    bool readFirmwareRevisionCharacteristic();
    bool readHardwareRevisionCharacteristic();
    bool readManufacturerCharacteristics();
    bool readModelNumberCharacteristic();
    bool readSoftwareRevisionCharacteristic();
    bool readSerialNumberCharacteristic();

    // All Device Info characteristics are read-only, single values.
    QString manufacturer() const;
    QString modelNumber() const;
    QString hardwareRevision() const;
    QString firmwareRevision() const;
    QString softwareRevision() const;
    QString serialNumber() const;

Q_SIGNALS:
    void manufacturerRead(const QString &name);
    void modelNumberRead(const QString &model);
    void hardwareRevisionRead(const QString &revision);
    void firmwareRevisionRead(const QString &revision);
    void softwareRevisionRead(const QString &revision);
    void serialNumberRead(const QString &serialNumber);

protected:
    /// \cond internal
    DeviceInfoService(DeviceInfoServicePrivate * const d, QObject * const parent);
    /// \endcond

private:
    Q_DECLARE_PRIVATE(DeviceInfoService)
    Q_DISABLE_COPY(DeviceInfoService)
    QTPOKIT_BEFRIEND_TEST(DeviceInfoService)
};

QTPOKIT_END_NAMESPACE

#endif // QTPOKIT_INFOSERVICE_H
