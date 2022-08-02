#ifndef SRC_CALCMODEL_H_
#define SRC_CALCMODEL_H_
#include <cstring>
#include <list>
#include <cmath>
#include "iostream"
#include "CalcControler.h"


using std::list;

namespace s21 {

class CalcModel {
    // Controller interface
    friend bool CalcControler::SetInput(const char *str);
    friend bool CalcControler::NeedX();
    friend double CalcControler::RequestCalculations(double X);
    friend void CalcControler::FreeCalcData();

 public:
    CalcModel();
    ~CalcModel();

 private:
    char input_[256];
    unsigned curr_index_;

    void FreeData();

    // Functions for checking correct input string
    bool CheckCorrectInput();
    void RemoveSpaces();
    bool CheckCorrectParentheses();
    bool CheckOnlyOneFunction();
    bool CheckOperatorAndOperandsOrder(const bool is_checking_inside_function);
    bool IsSign();
    bool IsOperator();
    bool IsNumber();
    bool IsFunction();
    bool CheckExponentialForm();

    // Functions for adding lexemes to list
    enum lexeme_type {
        number = 1, x, open_p, close_p, plus, minus, mult, division, mod_t, pow_t,
        cos_t, sin_t, tan_t, acos_t, asin_t, atan_t, sqrt_t, ln_t, log_t
    };
    struct lexeme {
        double value;
        int priority;
        lexeme_type type;
        lexeme(double _val, int _prio, lexeme_type _t) {
            value = _val;
            priority = _prio;
            type = _t;
        }
    };
    list<lexeme> lexemes_list_;
    bool have_x_;
    void ParseStringIntoLexemes();
    void AddNumberToList();
    void AddOperatorToList(bool &unary_sign_flag_, bool &first_sign_flag);
    void AddFunctionToList();
    void AddParanthesesToList(bool &unary_sign_flag);

    // Functions for Reverse Polish Notation
    list<lexeme> support_stack_;
    list<lexeme> ready_stack_;
    void MakeReversePolishNotationStack();
    void HandleSupportStack(lexeme lex);
    void MoveFromSupportToReady();
    void HandleCloseParantheses();

    // Functions for calculations
    using iterator = list<lexeme>::iterator;
    double x_value_;
    double CalculateFullExpression();
    double CalculateFunction(const lexeme function, lexeme value);
    double SqueezeFunctionResultWithNmb(const iterator function, const iterator value);
    double CalculateTwoOperators(lexeme operand1, lexeme operand2, const lexeme operation);
    void ApplyValueToX(lexeme &value);
};

}  // namespace s21

#endif  // SRC_CALCMODEL_H_
