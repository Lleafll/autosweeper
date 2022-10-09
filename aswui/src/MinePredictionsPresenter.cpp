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
            return QStringLiteral(" ");
        case asw::Prediction::Unsafe:
            return QStringLiteral("⚠");
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
            [this](std::size_t const row,
                   std::size_t const column,
                   asw::Prediction const prediction) {
                view_.set_cell(
                        gsl::narrow_cast<int>(row),
                        gsl::narrow_cast<int>(column),
                        to_qstring(prediction));
            });
}

}  // namespace aswui