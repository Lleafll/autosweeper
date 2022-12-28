#include "MinePredictionsPresenter.h"
#include <QString>

namespace aswui::detail {

QString to_qstring(asw::Prediction const prediction) {
    switch (prediction) {
        case asw::Prediction::Unknown:
            return QStringLiteral("?");
        case asw::Prediction::Safe:
            return QStringLiteral("✓");
        case asw::Prediction::Unsafe:
            return QStringLiteral("⚠");
        case asw::Prediction::Empty:
            return QStringLiteral(" ");
        case asw::Prediction::One:
            return QStringLiteral("1");
        case asw::Prediction::Two:
            return QStringLiteral("2");
        case asw::Prediction::Three:
            return QStringLiteral("3");
        case asw::Prediction::Four:
            return QStringLiteral("4");
        case asw::Prediction::Five:
            return QStringLiteral("5");
        case asw::Prediction::Six:
            return QStringLiteral("6");
        case asw::Prediction::Seven:
            return QStringLiteral("7");
        case asw::Prediction::Eight:
            return QStringLiteral("8");
        case asw::Prediction::Hidden:
            return QStringLiteral("☐");
        case asw::Prediction::Mine:
            return QStringLiteral("💣");
        case asw::Prediction::DetectedMine:
            return u"🚩"_qs;
    }
    abort();
}

}  // namespace aswui::detail