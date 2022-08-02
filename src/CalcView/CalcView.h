#ifndef CALCVIEW_H
#define CALCVIEW_H

#include <QWidget>
#include <QMessageBox>
#include "../CalcModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CalcView; }
QT_END_NAMESPACE

using namespace s21;

class CalcView : public QWidget
{
    Q_OBJECT

public:
    CalcView(CalcControler *_controler);
    virtual ~CalcView();

private:
    Ui::CalcView *ui;
    CalcControler *controler_;
    bool ConvertXYInputs();
    double Xmin_;
    double Xmax_;
    double Ymin_;
    double Ymax_;
    const double MAX_VALUE_ = 1000000;
    const double MIN_VALUE_ = -1000000;
    const unsigned STEPS_NMB_ = 100;
    void DrawGraph(char *input);

private slots:
    void on_btn1_clicked();
    void on_btn2_clicked();
    void on_btn3_clicked();
    void on_btn4_clicked();
    void on_btn5_clicked();
    void on_btn6_clicked();
    void on_btn7_clicked();
    void on_btn8_clicked();
    void on_btn9_clicked();
    void on_btn0_clicked();
    void on_plus_btn_clicked();
    void on_minus_btn_clicked();
    void on_mult_btn_clicked();
    void on_div_btn_clicked();
    void on_ln_btn_clicked();
    void on_sqrt_btn_clicked();
    void on_mod_btn_clicked();
    void on_pow_btn_clicked();
    void on_log_btn_clicked();
    void on_sin_btn_clicked();
    void on_cos_btn_clicked();
    void on_tan_btn_clicked();
    void on_asin_btn_clicked();
    void on_acos_btn_clicked();
    void on_atan_btn_clicked();
    void on_clear_btn_clicked();
    void on_backspace_btn_clicked();
    void on_equal_btn_clicked();
    void on_draw_graph_btn_clicked();
    void on_open_p_btn_clicked();
    void on_close_p_btn_clicked();
    void on_dot_btn_clicked();
};

#endif // CALCVIEW_H
