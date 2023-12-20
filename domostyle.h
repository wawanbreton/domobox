#pragma once

#include <QObject>

#include <QColor>

#include "resource/daymode.h"

class QVariantAnimation;
class QPropertyAnimation;

class DomoStyle : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor foreground READ getForeground NOTIFY changed)
    Q_PROPERTY(QColor buttonBackground READ getButtonBackground NOTIFY changed)
    Q_PROPERTY(QColor buttonBackgroundDisabled READ getButtonBackgroundDisabled NOTIFY dummyChanged)
    Q_PROPERTY(QColor buttonIcon READ getButtonIcon NOTIFY dummyChanged)
    Q_PROPERTY(QColor buttonIconDisabled READ getButtonIconDisabled NOTIFY dummyChanged)
    Q_PROPERTY(QColor componentBackground READ getComponentBackground NOTIFY changed)
    Q_PROPERTY(QColor componentBackgroundAlternative READ getComponentBackgroundAlternative NOTIFY changed)
    Q_PROPERTY(QColor backgroundCenter READ getBackgroundCenter NOTIFY changed)
    Q_PROPERTY(QColor backgroundOuter READ getBackgroundOuter NOTIFY changed)
    Q_PROPERTY(QColor cold READ getCold NOTIFY dummyChanged)
    Q_PROPERTY(QColor coldOverlay READ getColdOverlay NOTIFY dummyChanged)
    Q_PROPERTY(QColor hot READ getHot NOTIFY dummyChanged)
    Q_PROPERTY(int fontSizeSmall READ getFontSizeSmall NOTIFY dummyChanged)
    Q_PROPERTY(int fontSizeMedium READ getFontSizeMedium NOTIFY dummyChanged)
    Q_PROPERTY(int fontSizeBig READ getFontSizeBig NOTIFY dummyChanged)
    Q_PROPERTY(int fontSizeHuge READ getFontSizeHuge NOTIFY dummyChanged)
    Q_PROPERTY(int margin READ getMargin NOTIFY dummyChanged)

    public:
        explicit DomoStyle(QObject *parent = nullptr);

        int getFontSizeSmall() const { return 22; }

        int getFontSizeMedium() const { return 28; }

        int getFontSizeBig() const { return 36; }

        int getFontSizeHuge() const { return 130; }

        int getMargin() const { return 18; }

        void onDayModeChanged(DayMode::Enum mode);

        QColor getForeground() const;

        QColor getButtonBackground() const;

        const QColor &getButtonBackgroundDisabled() const { return buttonBackgroundDisabled; }

        const QColor &getButtonIcon() const { return buttonIcon; }

        const QColor &getButtonIconDisabled() const { return buttonIconDisabled; }

        QColor getComponentBackground() const;

        QColor getComponentBackgroundAlternative() const;

        QColor getBackgroundCenter() const;

        QColor getBackgroundOuter() const;

        const QColor &getCold() const { return cold; }

        const QColor &getColdOverlay() const { return coldOverlay; }

        const QColor &getHot() const { return hot; }

    signals:
        void changed();

        void dummyChanged();

    private:
        static constexpr QColor foregroundDay{111, 147, 20};
        static constexpr QColor foregroundNight{228, 148, 61};
        static constexpr QColor buttonBackgroundDisabled{63, 63, 63};
        static constexpr QColor buttonBackgroundDay{foregroundDay};
        static constexpr QColor buttonBackgroundNight{foregroundNight};
        static constexpr QColor buttonIcon{255, 255, 255};
        static constexpr QColor buttonIconDisabled{123, 125, 127};
        static constexpr QColor componentBackgroundDay{212, 230, 248};
        static constexpr QColor componentBackgroundNight{9, 23, 83};
        static constexpr QColor componentBackgroundAlternativeDay{169, 204, 242};
        static constexpr QColor componentBackgroundAlternativeNight{14, 37, 128};
        static constexpr QColor backgroundCenterDay{59, 120, 176};
        static constexpr QColor backgroundCenterNight{1, 13, 73};
        static constexpr QColor backgroundOuterDay{10, 68, 136};
        static constexpr QColor backgroundOuterNight{0, 7, 32};
        static constexpr QColor cold{41, 119, 190};
        static constexpr QColor coldOverlay{18, 53, 85};
        static constexpr QColor hot{190, 41, 41};

    private:
        DayMode::Enum _dayMode{DayMode::Day};
};
