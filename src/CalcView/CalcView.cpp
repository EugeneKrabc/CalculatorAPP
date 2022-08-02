#include "CalcView.h"
#include "ui_CalcView.h"

CalcView::CalcView(CalcControler *_controler)
    : QWidget(nullptr)
    , ui(new Ui::CalcView)
{
    ui->setupUi(this);
    controler_ = _controler;
    Xmin_ = -10;
    Xmax_ = 10;
    Ymin_ = -10;
    Ymax_ = 10;
}

CalcView::~CalcView()
{
    delete ui;
}

void CalcView::on_equal_btn_clicked() {
    QByteArray ba = ui->input_string->text().toLocal8Bit();
    char *input = ba.data();
    if (controler_->SetInput(input)) {
        double X = 0;
        if (controler_->NeedX()) {
            bool double_convert_res = false;
            while (!double_convert_res) {
                X = QString(ui->X_value_str->text()).toDouble(&double_convert_res);
                if (!double_convert_res) {
                    QMessageBox::warning(this, "Error", "Invalid X value");
                    return;
                }
            }
        }
        double result = controler_->RequestCalculations(X);
        ui->input_string->clear();
        ui->input_string->insert(QString::number(result));
    } else {
        QMessageBox::warning(this, "Error", "Invalid expression");
        ui->input_string->clear();
    }
}

void CalcView::on_draw_graph_btn_clicked() {
    if (!ConvertXYInputs()) {
        QMessageBox::warning(this, "Error", "Invalid XY range for graphic");
        return;
    }
    QByteArray ba = ui->input_string->text().toLocal8Bit();
    char *input = ba.data();
    if (!controler_->SetInput(input)) {
        QMessageBox::warning(this, "Error", "Invalid input");
        ui->input_string->clear();
        return;
    }
    if (!controler_->NeedX()) {
        QMessageBox::warning(this, "Error", "Expression must have x");
    }
    DrawGraph(input);
}

bool CalcView::ConvertXYInputs() {
    bool convert_res = false;
    Xmin_ = QString(ui->Xmin_value_str->text()).toDouble(&convert_res);
    if (!convert_res || Xmin_ > MAX_VALUE_ || Xmin_ < MIN_VALUE_)
        return false;
    Xmax_ = QString(ui->Xmax_value_str->text()).toDouble(&convert_res);
    if (!convert_res || Xmax_ > MAX_VALUE_ || Xmax_ < MIN_VALUE_)
        return false;
    Ymin_ = QString(ui->Ymin_value_str->text()).toDouble(&convert_res);
    if (!convert_res || Ymin_ > MAX_VALUE_ || Ymin_ < MIN_VALUE_)
        return false;
    Ymax_ = QString(ui->Ymax_value_str->text()).toDouble(&convert_res);
    if (!convert_res || Ymax_ > MAX_VALUE_ || Ymax_ < MIN_VALUE_)
        return false;
    if (Ymin_ >= Ymax_ || Xmin_ >= Xmax_)
        return false;
    return true;
}

void CalcView::DrawGraph(char *input) {
    ui->Graphic->clearGraphs();
    double delta = (Xmax_ - Xmin_) / STEPS_NMB_;
    ui->Graphic->xAxis->setRange(Xmin_, Xmax_);
    ui->Graphic->yAxis->setRange(Ymin_, Ymax_);
    QVector<double> x, y;
    for (double X = Xmin_; X <= Xmax_; X += delta) {
        controler_->SetInput(input);
        double Y = controler_->RequestCalculations(X);
        x.push_back(X);
        y.push_back(Y);
    }
    ui->Graphic->addGraph();
    ui->Graphic->graph(0)->addData(x, y);
    ui->Graphic->replot();
}

void CalcView::on_btn1_clicked() {
    ui->input_string->insert("1");
}

void CalcView::on_btn2_clicked() {
    ui->input_string->insert("2");
}

void CalcView::on_btn3_clicked() {
    ui->input_string->insert("3");
}

void CalcView::on_btn4_clicked() {
    ui->input_string->insert("4");
}

void CalcView::on_btn5_clicked() {
    ui->input_string->insert("5");
}

void CalcView::on_btn6_clicked() {
    ui->input_string->insert("6");
}

void CalcView::on_btn7_clicked() {
    ui->input_string->insert("7");
}

void CalcView::on_btn8_clicked() {
    ui->input_string->insert("8");
}

void CalcView::on_btn9_clicked() {
    ui->input_string->insert("9");
}

void CalcView::on_btn0_clicked() {
    ui->input_string->insert("0");
}

void CalcView::on_plus_btn_clicked() {
    ui->input_string->insert("+");
}

void CalcView::on_minus_btn_clicked() {
    ui->input_string->insert("-");
}

void CalcView::on_mult_btn_clicked() {
    ui->input_string->insert("*");
}

void CalcView::on_div_btn_clicked() {
    ui->input_string->insert("/");
}

void CalcView::on_ln_btn_clicked() {
    ui->input_string->insert("ln(");
}

void CalcView::on_sqrt_btn_clicked() {
    ui->input_string->insert("sqrt(");
}

void CalcView::on_mod_btn_clicked() {
    ui->input_string->insert("mod");
}

void CalcView::on_pow_btn_clicked() {
    ui->input_string->insert("^");
}

void CalcView::on_log_btn_clicked() {
    ui->input_string->insert("log(");
}

void CalcView::on_sin_btn_clicked() {
    ui->input_string->insert("sin(");
}

void CalcView::on_cos_btn_clicked() {
    ui->input_string->insert("cos(");
}

void CalcView::on_tan_btn_clicked() {
    ui->input_string->insert("tan(");
}

void CalcView::on_asin_btn_clicked() {
    ui->input_string->insert("asin(");
}

void CalcView::on_acos_btn_clicked() {
    ui->input_string->insert("acos(");
}

void CalcView::on_atan_btn_clicked() {
    ui->input_string->insert("atan(");
}

void CalcView::on_clear_btn_clicked() {
    ui->input_string->clear();
}

void CalcView::on_backspace_btn_clicked() {
    ui->input_string->backspace();
}

void CalcView::on_dot_btn_clicked() {
    ui->input_string->insert(".");
}

void CalcView::on_open_p_btn_clicked() {
    ui->input_string->insert("(");
}

void CalcView::on_close_p_btn_clicked() {
    ui->input_string->insert(")");
}
