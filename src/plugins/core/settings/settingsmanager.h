/*
 * Fooyin
 * Copyright 2022, Luke Taylor <LukeT1@proton.me>
 *
 * Fooyin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fooyin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fooyin.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include "settingsentry.h"
#include "settingtypes.h"

#include <QMetaEnum>
#include <QReadWriteLock>
#include <QSettings>

namespace Core {
class SettingsManager : public QObject
{
    Q_OBJECT

public:
    explicit SettingsManager(QObject* parent = nullptr);
    ~SettingsManager() override;
    SettingsManager(const SettingsManager& other)             = delete;
    SettingsManager& operator=(const SettingsManager& other)  = delete;
    SettingsManager(const SettingsManager&& other)            = delete;
    SettingsManager& operator=(const SettingsManager&& other) = delete;

    void loadSettings();
    void storeSettings();

    QString getKeyString(const SettingsEntry& setting);

    template <auto key>
    static int constexpr findType()
    {
        using E = decltype(key);
        static_assert(std::is_enum_v<E>, "The template parameter is not an enum!");
        static_assert(std::is_same_v<std::underlying_type_t<E>, uint32_t>, "Enum should be of type 'uint32_t'");

        // Use last 4 bits to determine type
        const auto type = (key & 0xF0'00'00'00);
        return static_cast<int>(type);
    }

    template <typename E>
    auto constexpr getMapKey(E key)
    {
        const auto meta      = QMetaEnum::fromType<E>();
        const auto enumName  = meta.name();
        const auto keyString = QString::fromLatin1(meta.valueToKey(key));
        const auto mapKey    = enumName + keyString;

        return mapKey;
    }

    template <typename E, typename T>
    void constexpr subscribe(E key, T* obj, void (T::*func)())
    {
        const auto mapKey = getMapKey(key);

        if(m_settings.count(mapKey)) {
            QObject::connect(&m_settings.at(mapKey), &SettingsEntry::settingChanged, obj, func);
        };
    }

    template <typename E, typename T>
    void constexpr unsubscribe(E key, T* obj)
    {
        const auto mapKey = getMapKey(key);

        if(m_settings.count(mapKey)) {
            QObject::disconnect(&m_settings.at(mapKey), nullptr, obj, nullptr);
        }
    }

    template <typename E>
    void constexpr createSetting(E key, const QVariant& value, const QString& group = {})
    {
        const auto meta      = QMetaEnum::fromType<E>();
        const auto enumName  = meta.name();
        const auto keyString = QString::fromLatin1(meta.valueToKey(key));
        const auto mapKey    = enumName + keyString;

        if(!m_settings.count(mapKey)) {
            m_settings.emplace(mapKey, SettingsEntry{keyString, value, true, group});
        }
    }

    template <typename E>
    void constexpr createTempSetting(E key, const QVariant& value, const QString& group = {})
    {
        const auto meta      = QMetaEnum::fromType<E>();
        const auto enumName  = meta.name();
        const auto keyString = QString::fromLatin1(meta.valueToKey(key));
        const auto mapKey    = enumName + keyString;

        if(!m_settings.count(mapKey)) {
            m_settings.emplace(mapKey, SettingsEntry{keyString, value, false, group});
        }
    }

    template <auto key>
    auto constexpr value()
    {
        const auto mapKey = getMapKey(key);

        m_lock.lockForRead();

        const auto value = m_settings.count(mapKey) ? m_settings.at(mapKey).value() : -1;
        const auto type  = findType<key>();

        m_lock.unlock();

        if constexpr(type == Settings::Type::Bool) {
            return value.toBool();
        }
        else if constexpr(type == Settings::Type::Double) {
            return value.toDouble();
        }
        else if constexpr(type == Settings::Type::Int) {
            return value.toInt();
        }
        else if constexpr(type == Settings::Type::String) {
            return value.toString();
        }
        else if constexpr(type == Settings::Type::ByteArray) {
            return value.toByteArray();
        }
        else {
            return value;
        }
    }

    template <auto key, typename V>
    void constexpr set(V value)
    {
        const auto mapKey = getMapKey(key);

        m_lock.lockForWrite();

        if(!m_settings.count(mapKey)) {
            m_lock.unlock();
            return;
        }
        m_settings.at(mapKey).setValue(value);

        m_lock.unlock();

        m_settings.at(mapKey).changedSetting();
    }

private:
    QSettings m_settingsFile;
    std::map<QString, SettingsEntry> m_settings;
    QReadWriteLock m_lock;
};
}; // namespace Core
