#include "MinePredictionsPresenter.h"
#include <QString>
#include <asw/algorithm2d.h>
#include <gsl/narrow>

namespace aswui {

namespace {

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

}  // namespace

MinePredictionsPresenter::MinePredictionsPresenter(MinePredictionsView& view)
    : view_{view} {
}

void MinePredictionsPresenter::set(
        asw::ConstPredictionSpan const& predictions) {
    view_.set_row_count(gsl::narrow_cast<int>(predictions.extent(0)));
    view_.set_column_count(gsl::narrow_cast<int>(predictions.extent(1)));
    asw::indexed_for_each(
            predictions,
            [this](asw::Position const& i, asw::Prediction const prediction) {
                view_.set_cell(
                        gsl::narrow_cast<int>(i.row),
                        gsl::narrow_cast<int>(i.column),
                        to_qstring(prediction));
            });
}

}  // namespace aswui