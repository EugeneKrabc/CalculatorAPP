#ifndef SRC_CALCCONTROLER_H_
#define SRC_CALCCONTROLER_H_

namespace s21 {

class CalcModel;

class CalcControler {
 private:
    CalcModel *model_;
 public:
    explicit CalcControler(CalcModel *m):model_(m) {}

    bool SetInput(const char *str);
    bool NeedX();
    double RequestCalculations(double X);
    void FreeCalcData();
};

}  // namespace s21

#endif  // SRC_CALCCONTROLER_H_
