#include "CalcModel.h"

namespace s21 {

bool CalcControler::SetInput(const char *str) {
    if (!str)
        return false;
    model_->FreeData();
    strncpy(model_->input_, str, 255);
    return model_->CheckCorrectInput();
}

bool CalcControler::NeedX() {
    model_->ParseStringIntoLexemes();  // After parsing we will know about x
    return model_->have_x_;
}

void CalcControler::FreeCalcData() {
    model_->FreeData();
}

double CalcControler::RequestCalculations(double X) {
    if (model_->lexemes_list_.empty())
        model_->ParseStringIntoLexemes();
    if (model_->have_x_)
        model_->x_value_ = X;
    model_->MakeReversePolishNotationStack();
    double result = model_->CalculateFullExpression();
    model_->FreeData();
    return result;
}

}  // namespace s21
