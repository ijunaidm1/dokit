// SPDX-FileCopyrightText: 2022-2023 Paul Colby <git@colby.id.au>
// SPDX-License-Identifier: LGPL-3.0-or-later

/*!
 * \file
 * Defines the GenericAccessService and GenericAccessServicePrivate classes.
 */

#include <qtpokit/genericaccessservice.h>
#include "genericaccessservice_p.h"

#include <QtEndian>

/*!
 * \class GenericAccessService
 *
 * The GenericAccessService class accesses the `Generic Access` service of Pokit devices.
 *
 * \cond internal
 * \pokitApi Pokit API 1.00 (and 0.02) claims support for the `Generic Access` (`0x1800`) service,
 * however the neither the Pokit Meter, nor the Pokit Pro report any support for this service, but
 * both report support for an undocumented `Generic Attribute` (`0x1801`) service instead.
 * \endcond
 */

/*!
 * Constructs a new Pokit service with \a parent.
 */
GenericAccessService::GenericAccessService(QLowEnergyController * const controller, QObject * parent)
    : AbstractPokitService(new GenericAccessServicePrivate(controller, this), parent)
{

}

/*!
 * \cond internal
 * Constructs a new Pokit service with \a parent, and private implementation \a d.
 */
GenericAccessService::GenericAccessService(
    GenericAccessServicePrivate * const d, QObject * const parent)
    : AbstractPokitService(d, parent)
{

}
/// \endcond

/*!
 * Destroys this GenericAccessService object.
 */
GenericAccessService::~GenericAccessService()
{

}

bool GenericAccessService::readCharacteristics()
{
    const bool r1 = readDeviceNameCharacteristic();
    const bool r2 = readAppearanceCharacteristic();
    return (r1 && r2);
}

/*!
 * Read the `Generic Access` service's `Appearance` characteristic.
 *
 * Returns `true` if the read request is succesfully queued, `false` otherwise (ie if the
 * underlying controller it not yet connected to the Pokit device, or the device's services have
 * not yet been discovered).
 *
 * Emits appearanceRead() if/when the characteristic has been read successfully.
 */
bool GenericAccessService::readAppearanceCharacteristic()
{
    Q_D(GenericAccessService);
    return d->readCharacteristic(CharacteristicUuids::appearance);
}

/*!
 * Read the `Generic Access` service's `Device Name` characteristic.
 *
 * Returns `true` if the read request is succesfully queued, `false` otherwise (ie if the
 * underlying controller it not yet connected to the Pokit device, or the device's services have
 * not yet been discovered).
 *
 * Emits deviceNameRead() if/when the characteristic has been read successfully.
 */
bool GenericAccessService::readDeviceNameCharacteristic()
{
    Q_D(GenericAccessService);
    return d->readCharacteristic(CharacteristicUuids::deviceName);
}

/*!
 * Returns the most recent value of the `Generic Access` services's `Appearance` characteristic.
 *
 * The returned value, if any, is from the underlying Bluetooth stack's cache. If no such value is
 * currently available (ie the serviceDetailsDiscovered signal has not been emitted yet), `0xFFFF`
 * is returned.
 *
 * \note Pokit's Bluetooth API suggests the device's `Appearance` will always be 0 aka "Unknown
 * Appearance", so this is probably not a very useful characteristic if you already know you are
 * dealing with a Pokit device.
 */
quint16 GenericAccessService::appearance() const
{
    Q_D(const GenericAccessService);
    const QLowEnergyCharacteristic characteristic =
        d->getCharacteristic(CharacteristicUuids::appearance);
    return (characteristic.isValid()) ? GenericAccessServicePrivate::parseAppearance(characteristic.value())
        : std::numeric_limits<quint16>::max();
}

/*!
 * Returns the most recent value of the `Generic Access` services's `Device Name` characteristic.
 *
 * The returned value, if any, is from the underlying Bluetooth stack's cache. If no such value is
 * currently available (ie the serviceDetailsDiscovered signal has not been emitted yet), then a
 * null QString is returned.
 */
QString GenericAccessService::deviceName() const
{
    Q_D(const GenericAccessService);
    const QLowEnergyCharacteristic characteristic =
        d->getCharacteristic(CharacteristicUuids::deviceName);
    return (characteristic.isValid()) ? QString::fromUtf8(characteristic.value()) : QString();
}

/*!
 * Set's the Pokit device's name to \a name.
 *
 * Returns `true` if the write request was successfully queued, `false` otherwise.
 *
 * Emits deviceNameWritten() if/when the \a name has been set.
 */
bool GenericAccessService::setDeviceName(const QString &name)
{
    Q_D(const GenericAccessService);
    const QLowEnergyCharacteristic characteristic =
        d->getCharacteristic(CharacteristicUuids::deviceName);
    if (!characteristic.isValid()) {
        return false;
    }

    const QByteArray value = name.toUtf8();
    if (value.length() > 11) {
        qCWarning(d->lc).noquote() << tr(R"(Device name "%1" is too long (%2 > 11 bytes): 0x%3)")
            .arg(name).arg(value.length()).arg(QLatin1String(value.toHex()));
        return false;
    }

    d->service->writeCharacteristic(characteristic, value);
    return (d->service->error() != QLowEnergyService::ServiceError::CharacteristicWriteError);
}


/*!
 * \fn GenericAccessService::appearanceRead
 *
 * This signal is emitted when the `Appearance` characteristic has been read successfully.
 *
 * \see readAppearanceCharacteristic
 * \see appearance
 */

/*!
 * \fn GenericAccessService::deviceNameRead
 *
 * This signal is emitted when the `Device Name` characteristic has been read successfully.
 *
 * \see readDeviceName
 */

/*!
 * \fn GenericAccessService::deviceNameWritten
 *
 * This signal is emitted when the `Device Name` characteristic has been written successfully.
 *
 * \see setDeviceName
 */

/*!
 * \cond internal
 * \class GenericAccessServicePrivate
 *
 * The GenericAccessServicePrivate class provides private implementation for GenericAccessService.
 */

/*!
 * \internal
 * Constructs a new GenericAccessServicePrivate object with public implementation \a q.
 */
GenericAccessServicePrivate::GenericAccessServicePrivate(
    QLowEnergyController * controller, GenericAccessService * const q)
    : AbstractPokitServicePrivate(GenericAccessService::serviceUuid, controller, q)
{

}

/*!
 * Parses the `Appearance` \a value. Returns `0xFFFF` if not valid.
 */
quint16 GenericAccessServicePrivate::parseAppearance(const QByteArray &value)
{
    if (!checkSize(QLatin1String("Appearance"), value, 2, 2)) {
        return std::numeric_limits<quint16>::max();
    }
    const quint16 appearance = qFromLittleEndian<quint16>(value.constData());
    qCDebug(lc).noquote() << tr("Appearance: %1.").arg(appearance);
    return appearance;
}

/*!
 * Implements AbstractPokitServicePrivate::characteristicRead to parse \a value, then emit a
 * specialised signal, for each supported \a characteristic.
 */
void GenericAccessServicePrivate::characteristicRead(const QLowEnergyCharacteristic &characteristic,
                                              const QByteArray &value)
{
    AbstractPokitServicePrivate::characteristicRead(characteristic, value);

    Q_Q(GenericAccessService);
    if (characteristic.uuid() == GenericAccessService::CharacteristicUuids::appearance) {
        Q_EMIT q->appearanceRead(parseAppearance(value));
        return;
    }

    if (characteristic.uuid() == GenericAccessService::CharacteristicUuids::deviceName) {
        const QString deviceName = QString::fromUtf8(value);
        qCDebug(lc).noquote() << tr(R"(Device name: "%1")").arg(deviceName);
        Q_EMIT q->deviceNameRead(deviceName);
        return;
    }

    qCWarning(lc).noquote() << tr("Unknown characteristic read for Generic Access service")
        << serviceUuid << characteristic.name() << characteristic.uuid();
}

/*!
 * Implements AbstractPokitServicePrivate::characteristicWritten to parse \a newValue, then emit a
 * specialised signal, for each supported \a characteristic.
 */
void GenericAccessServicePrivate::characteristicWritten(const QLowEnergyCharacteristic &characteristic,
                                                 const QByteArray &newValue)
{
    AbstractPokitServicePrivate::characteristicWritten(characteristic, newValue);

    Q_Q(GenericAccessService);
    if (characteristic.uuid() == GenericAccessService::CharacteristicUuids::appearance) {
        qCWarning(lc).noquote() << tr("Appearance haracteristic is read-only, but somehow written")
            << serviceUuid << characteristic.name() << characteristic.uuid();
        return;
    }

    if (characteristic.uuid() == GenericAccessService::CharacteristicUuids::deviceName) {
        Q_EMIT q->deviceNameWritten();
        return;
    }

    qCWarning(lc).noquote() << tr("Unknown characteristic written for Generic Access service")
        << serviceUuid << characteristic.name() << characteristic.uuid();
}

/// \endcond
