#pragma once

#include <QObject>

#include <QDateTime>

#include "resource/firemode.h"

class QSettings;

class TemperatureConfiguration;

class ConfigurationManager : public QObject
{
    Q_OBJECT

    public:
        const FireMode::Enum &getFireMode() const { return _fireMode; }

        void setFireMode(FireMode::Enum fireMode, const QDateTime &holidayEnd=QDateTime());

        const QDateTime &getHolidayEnd() const { return _holidayEnd; }

        const TemperatureConfiguration *getTemperatureConfiguration() const
        { return _temperatureConfig; }

        void setTemperatureConfiguration(const TemperatureConfiguration &config);

    signals:
        void fireModeChanged(FireMode::Enum fireMode);

    public:
        static void init(QObject *parent);

        static ConfigurationManager *get() { return _instance; }

    private:
        explicit ConfigurationManager(QObject *parent=NULL);

        void onPeriodsConfigurationChanged() { _configChanged = true; }

    private:
        static ConfigurationManager *_instance;

    private:
        FireMode::Enum _fireMode{FireMode::Manual};
        TemperatureConfiguration *_temperatureConfig{nullptr};
        QDateTime _holidayEnd;
        bool _configChanged{false};
};
