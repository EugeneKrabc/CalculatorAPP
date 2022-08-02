#include "CalcModel.h"

namespace s21 {

CalcModel::CalcModel() {
    FreeData();
}

void CalcModel::FreeData() {
    curr_index_ = 0;
    input_[0] = '\0';
    ready_stack_.clear();
    support_stack_.clear();
    lexemes_list_.clear();
    have_x_ = false;
    x_value_ = 0;
}

CalcModel::~CalcModel() {
    FreeData();
}

// ****************************************** //
// Functions for checking correct input_
// ****************************************** //
bool CalcModel::CheckCorrectInput() {
    RemoveSpaces();
    if (input_[0] == '\0' || !CheckCorrectParentheses()) {
        return false;
    }
    if (CheckOnlyOneFunction())
        return true;
    curr_index_ = 0;
    return CheckOperatorAndOperandsOrder(false);
}

void CalcModel::RemoveSpaces() {
    int i, j;
    for (i = 0, j = 0; input_[i] != '\0'; i++) {
        if (input_[i] == ' ') continue;
        input_[j] = input_[i];
        j++;
    }
    input_[j] = '\0';
}

bool CalcModel::CheckCorrectParentheses() {
    int tmp_index = 0;
    int parentheses_counter = 0;
    while (input_[tmp_index]) {
        if (input_[tmp_index] == '(' && input_[tmp_index + 1] == ')')
            return false;
        else if (input_[tmp_index] == '(')
            parentheses_counter++;
        else if (input_[tmp_index] == ')')
            parentheses_counter--;
        tmp_index++;
    }
    return parentheses_counter == 0;
}

bool CalcModel::CheckOnlyOneFunction() {
    if (IsFunction() && input_[curr_index_] == '\0') {
        while (input_[curr_index_] == '(' || input_[curr_index_] == ')')
            (curr_index_)++;
        if (input_[curr_index_] == '\0') {
            curr_index_ = 0;
            return true;
        }
    }
    return false;
}

bool CalcModel::CheckOperatorAndOperandsOrder(const bool is_checking_inside_function) {
    int allow_sign = 1, allow_operand = 1, allow_operator = 0, p_counter = 1;
    while (input_[curr_index_]) {
        if (input_[curr_index_] == '(' || input_[curr_index_] == ')') {
            if (input_[curr_index_] == '(') {
                allow_sign = 1;
                if (is_checking_inside_function) p_counter++;
            } else {
                if (is_checking_inside_function) p_counter--;
            }
            if (is_checking_inside_function && p_counter == 0)
                break;
            (curr_index_)++;
        } else if (allow_sign && IsSign()) {
            allow_sign = 0;
            allow_operand = 1;
        } else if (allow_operand && (IsNumber() || IsFunction())) {
            allow_operand = 0;
            allow_operator = 1;
        } else if (allow_operator && IsOperator()) {
            allow_operator = 0;
            allow_operand = 1;
        } else {
            return false;
        }
    }
    return !allow_operand;
}

bool CalcModel::IsSign() {
    if (input_[curr_index_] == '-' || input_[curr_index_] == '+') {
        curr_index_++;
        return true;
    } else {
        return false;
    }
}

bool CalcModel::IsOperator() {
    if (IsSign())
        return true;
    if (input_[curr_index_] == '*' || input_[curr_index_] == '/' || input_[curr_index_] == '^') {
        curr_index_++;
        return true;
    }
    if (!strncmp(&input_[curr_index_], "mod", 3)) {
        curr_index_ += 3;
        return true;
    }
    return false;
}

bool CalcModel::IsNumber() {
    if (input_[curr_index_] == 'x') {
        curr_index_++;
        return CheckExponentialForm();
    }
    if (input_[curr_index_] >= '0' && input_[curr_index_] <= '9') {
        bool was_dot = false;
        while ((input_[curr_index_] >= '0' && input_[curr_index_] <= '9') || input_[curr_index_] == '.') {
            if (input_[curr_index_] == '.' &&
            (input_[curr_index_ + 1] > '9' || input_[curr_index_ + 1] < '0' || was_dot))
                return false;
            if (input_[curr_index_] == '.')
                was_dot = true;
            curr_index_++;
        }
        if (CheckExponentialForm())
            return true;
    }
    return false;
}

bool CalcModel::IsFunction() {
    bool did_caught_function = false;
    if (!strncmp(&input_[curr_index_], "cos", 3) || !strncmp(&input_[curr_index_], "sin", 3)
    || !strncmp(&input_[curr_index_], "tan", 3) || !strncmp(&input_[curr_index_], "log", 3)) {
        curr_index_ += 3;
        did_caught_function = true;
    } else if (!strncmp(&input_[curr_index_], "acos", 4) || !strncmp(&input_[curr_index_], "asin", 4)
    || !strncmp(&input_[curr_index_], "atan", 4) || !strncmp(&input_[curr_index_], "sqrt", 4)) {
        curr_index_ += 4;
        did_caught_function = true;
    } else if (!strncmp(&input_[curr_index_], "ln", 2)) {
        curr_index_ += 2;
        did_caught_function = true;
    }
    if (did_caught_function && input_[curr_index_++] == '(') {
        did_caught_function = CheckOperatorAndOperandsOrder(true);
        if (did_caught_function && input_[curr_index_] == ')') {
            curr_index_++;
            return true;
        }
    }
    return false;
}

bool CalcModel::CheckExponentialForm() {
    if (input_[curr_index_] == 'e' || input_[curr_index_] == 'E') {
        curr_index_++;
        if (input_[curr_index_] == '-' || input_[curr_index_] == '+')
            curr_index_++;
        if (input_[curr_index_] < '0' && input_[curr_index_] > '9')
            return false;
        while (input_[curr_index_] >= '0' && input_[curr_index_] <= '9')
            curr_index_++;
    }
    return true;
}

// ****************************************** //
// Functions for adding lexemes to list
// ****************************************** //

void CalcModel::ParseStringIntoLexemes() {
    bool unary_sign_flag = false, first_sign_flag = true;
    curr_index_ = 0;
    if (input_[curr_index_] != '-' && input_[curr_index_] != '+')
        first_sign_flag = false;
    while (input_[curr_index_]) {
        if ((input_[curr_index_] >= '0' && input_[curr_index_] <= '9') || input_[curr_index_] == 'x'
        || input_[curr_index_] == 'e' || input_[curr_index_] == 'E')
            AddNumberToList();
        else if (input_[curr_index_] == '+' || input_[curr_index_] == '-' || input_[curr_index_] == '*'
        || input_[curr_index_] == '/' || input_[curr_index_] == '^')
            AddOperatorToList(unary_sign_flag, first_sign_flag);
        else if (input_[curr_index_] == 'a' || input_[curr_index_] == 's' || input_[curr_index_] == 'l'
        || input_[curr_index_] == 't' || input_[curr_index_] == 'c' || input_[curr_index_] == 'm')
            AddFunctionToList();
        else if (input_[curr_index_] == '(' || input_[curr_index_] == ')')
            AddParanthesesToList(unary_sign_flag);
    }
}

void CalcModel::AddNumberToList() {
    if (input_[curr_index_] == 'x') {
        lexemes_list_.emplace_back(0, 0, x);
        have_x_ = true;
        curr_index_++;
        return;
    }
    double value = 0;
    int len = 0;
    sscanf(&input_[curr_index_], "%le%n", &value, &len);
    curr_index_ += len;
    if (len == 0) {
        /* This case can appear if we have X before e, so we skip e,
         *  read exponent and write it to X lexem.
         *  Later we take into account this exp when get X value
         */
        curr_index_ += 1;
        int exp = 0;
        sscanf(&input_[curr_index_], "%d%n", &exp, &len);
        (--lexemes_list_.end())->value = exp;
        curr_index_ += len;
        return;
    }
    lexemes_list_.emplace_back(value, 0, number);
}

void CalcModel::AddOperatorToList(bool &unary_sign_flag_, bool &first_sign_flag) {
    if (input_[curr_index_] == '+' || input_[curr_index_] == '-') {
        if (unary_sign_flag_ || first_sign_flag) {
            unary_sign_flag_ = false;
            first_sign_flag = false;
            lexemes_list_.emplace_back(0, 0, number);
        }
        if (input_[curr_index_] == '+')
            lexemes_list_.emplace_back(0, 1, plus);
        else
            lexemes_list_.emplace_back(0, 1, minus);
    } else if (input_[curr_index_] == '*') {
        lexemes_list_.emplace_back(0, 2, mult);
    } else if (input_[curr_index_] == '/') {
        lexemes_list_.emplace_back(0, 2, division);
    } else if (input_[curr_index_] == '^') {
        lexemes_list_.emplace_back(0, 3, pow_t);
    }
    curr_index_++;
}

void CalcModel::AddFunctionToList() {
    if (!strncmp(&(input_[curr_index_]), "mod", 3)) {
        lexemes_list_.emplace_back(0, 2, mod_t);
        curr_index_ += 3;
    } else if (!strncmp(&(input_[curr_index_]), "cos", 3)) {
        lexemes_list_.emplace_back(0, 4, cos_t);
        curr_index_ += 3;
    } else if (!strncmp(&(input_[curr_index_]), "sin", 3)) {
        lexemes_list_.emplace_back(0, 4, sin_t);
        curr_index_ += 3;
    } else if (!strncmp(&(input_[curr_index_]), "tan", 3)) {
        lexemes_list_.emplace_back(0, 4, tan_t);
        curr_index_ += 3;
    } else if (!strncmp(&(input_[curr_index_]), "log", 3)) {
        lexemes_list_.emplace_back(0, 4, log_t);
        curr_index_ += 3;
    } else if (!strncmp(&(input_[curr_index_]), "atan", 4)) {
        lexemes_list_.emplace_back(0, 4, atan_t);
        curr_index_ += 4;
    } else if (!strncmp(&(input_[curr_index_]), "acos", 4)) {
        lexemes_list_.emplace_back(0, 4, acos_t);
        curr_index_ += 4;
    } else if (!strncmp(&(input_[curr_index_]), "asin", 4)) {
        lexemes_list_.emplace_back(0, 4, asin_t);
        curr_index_ += 4;
    } else if (!strncmp(&(input_[curr_index_]), "sqrt", 4)) {
        lexemes_list_.emplace_back(0, 4, sqrt_t);
        curr_index_ += 4;
    } else if (!strncmp(&(input_[curr_index_]), "ln", 2)) {
        lexemes_list_.emplace_back(0, 4, ln_t);
        curr_index_ += 2;
    }
}

void CalcModel::AddParanthesesToList(bool &unary_sign_flag) {
    if (input_[curr_index_] == '(') {
        lexemes_list_.emplace_back(0, 0, open_p);
        if (input_[curr_index_ + 1] == '+' || input_[curr_index_ + 1] == '-')
            unary_sign_flag = true;
    } else {
        lexemes_list_.emplace_back(0, 0, close_p);
    }
    curr_index_++;
}

// ****************************************** //
// Functions for making reverse polish notation stack
// ****************************************** //

void CalcModel::MakeReversePolishNotationStack() {
    for (auto it = lexemes_list_.begin(); it != lexemes_list_.end(); it++) {
        if (it->type == number || it->type == x)
            ready_stack_.push_front(*it);
        else if (it->type >= plus)
            HandleSupportStack(*it);
        else if (it->type == open_p)
            support_stack_.push_front(*it);
        else if (it->type == close_p)
            HandleCloseParantheses();
    }
    for (auto it = support_stack_.begin(); it != support_stack_.end(); it++) {
        MoveFromSupportToReady();
    }
    ready_stack_.reverse();
}

void CalcModel::HandleSupportStack(CalcModel::lexeme lex) {
    if (support_stack_.empty()) {
        support_stack_.push_front(lex);
    } else {
        for (auto it = support_stack_.begin();
        it != support_stack_.end() && lex.priority <= it->priority; it++)
            MoveFromSupportToReady();
        support_stack_.push_front(lex);
    }
}

void CalcModel::MoveFromSupportToReady() {
    ready_stack_.push_front(*(support_stack_.begin()));
    support_stack_.pop_front();
}

void CalcModel::HandleCloseParantheses() {
    while (support_stack_.begin()->type != open_p)
        MoveFromSupportToReady();
    support_stack_.pop_front();
}

// ****************************************** //
// Functions for calculating
// ****************************************** //

double CalcModel::CalculateFullExpression() {
    if (ready_stack_.size() == 1) {
        if (have_x_ && ready_stack_.begin()->type == x)
            ApplyValueToX(*(ready_stack_.begin()));
        return ready_stack_.begin()->value;
    }
    auto prev_prev_it = ready_stack_.begin();
    auto prev_it = ++ready_stack_.begin();
    if (ready_stack_.size() == 2)  // This case can occur when we have cos(5) for example;
        return CalculateFunction(*prev_it, *prev_prev_it);
    auto current_it = ++(++ready_stack_.begin());
    while ((current_it->type == number || current_it->type == x) && prev_it->type < 11) {
        current_it++;
        prev_it++;
        prev_prev_it++;
    }
    if (prev_it->type >= 11) {  // functions >= 11, math operations < 11
        return SqueezeFunctionResultWithNmb(prev_it, prev_prev_it);
    }
    if (current_it->type >= 11) {
        return SqueezeFunctionResultWithNmb(current_it , prev_it);
    }
    prev_prev_it->value = CalculateTwoOperators(*prev_prev_it, *prev_it, *current_it);
    prev_prev_it->type = number;
    prev_prev_it->priority = 0;
    ready_stack_.erase(current_it);
    ready_stack_.erase(prev_it);
    return CalculateFullExpression();
}

double CalcModel::
SqueezeFunctionResultWithNmb(const list<lexeme>::iterator function, const list<lexeme>::iterator value) {
    value->value = CalculateFunction(*function, *value);
    value->type = number;
    value->priority = 0;
    ready_stack_.erase(function);
    return CalculateFullExpression();
}

double CalcModel::CalculateFunction(const lexeme function, lexeme value) {
    double return_value = 0;
    if (have_x_ && value.type == x)
        ApplyValueToX(value);
    if (function.type == cos_t)
        return_value = cos(value.value);
    else if (function.type == sin_t)
        return_value = sin(value.value);
    else if (function.type == tan_t)
        return_value = tan(value.value);
    else if (function.type == acos_t)
        return_value = acos(value.value);
    else if (function.type == asin_t)
        return_value = asin(value.value);
    else if (function.type == atan_t)
        return_value = atan(value.value);
    else if (function.type == log_t)
        return_value = log10(value.value);
    else if (function.type == ln_t)
        return_value = log(value.value);
    else if (function.type == sqrt_t)
        return_value = sqrt(value.value);
    return  return_value;
}

double CalcModel::CalculateTwoOperators(lexeme operand1, lexeme operand2, const lexeme operation) {
    double return_value = 0;
    if (have_x_) {
        if (operand1.type == x) ApplyValueToX(operand1);
        if (operand2.type == x) ApplyValueToX(operand2);
    }
    if (operation.type == plus)
        return_value += operand1.value + operand2.value;
    else if (operation.type == minus)
        return_value += operand1.value - operand2.value;
    else if (operation.type == mult)
        return_value += operand1.value * operand2.value;
    else if (operation.type == division)
        return_value += operand1.value / operand2.value;
    else if (operation.type == mod_t)
        return_value += fmod(operand1.value, operand2.value);
    else if (operation.type == pow_t)
        return_value += pow(operand1.value, operand2.value);
    return return_value;
}

void CalcModel::ApplyValueToX(lexeme &value) {
    double multiplier = 1;
    while (value.value != 0) {  // Handle situation if we need apply exponent to x value;
        if (value.value > 0) {
            multiplier *= 10;
            value.value--;
        } else {
            multiplier /= 10;
            value.value++;
        }
    }
    value.value = x_value_ * multiplier;
}

}  // namespace s21
