#include "CalcModel.h"
#include <gtest/gtest.h>

using s21::CalcModel;
using s21::CalcControler;

TEST(Controler_Model_Interactions, input_test) {
    CalcModel model;
    CalcControler controler(&model);
    EXPECT_TRUE(controler.
    SetInput("((123+15))-(cos((-5)))+ln((10/(5*7))^2)-(tan(sin(-3mod2))-5*log(10)mod3*4/5/7)"));
    EXPECT_FALSE(controler.SetInput(" -acos(123) ++ 15"));
    EXPECT_TRUE(controler.SetInput("123-(-5)"));
    EXPECT_FALSE(controler.SetInput("   123sin"));
    EXPECT_FALSE(controler.SetInput("(+)(-)"));
    EXPECT_FALSE(controler.SetInput("mod 5"));
    EXPECT_FALSE(controler.SetInput(" *13^2"));
    EXPECT_FALSE(controler.SetInput(" -tan(atan(  )/2)"));
    EXPECT_FALSE(controler.SetInput(" asin / 2"));
    EXPECT_TRUE(controler.SetInput("  sin(cos(x))"));
    EXPECT_FALSE(controler.SetInput(""));
    EXPECT_TRUE(controler.SetInput("-(-sin(21))"));
    EXPECT_TRUE(controler.SetInput("5.3e-5 + 6"));
    EXPECT_TRUE(controler.SetInput("5.3e5 + 6"));
    EXPECT_TRUE(controler.SetInput("-3.5*cos(x*5)+2.21e-05*(-2+sqrt(x*2.1^2))/3"));
    EXPECT_TRUE(controler.SetInput("cos(x)-xe * 21"));
    EXPECT_FALSE(controler.SetInput("21e-x * 21"));
    EXPECT_FALSE(controler.SetInput("2.142ecos(5)"));
    EXPECT_TRUE(controler.SetInput("cos(x)-xe05 * 21"));
}

TEST(Controler_Model_Interactions, calculations_test) {
    CalcModel model;
    CalcControler controler(&model);
    controler.SetInput("((123+15))-(cos((-5)))+ln((10/(5*7))^2)-(tan(sin(-3mod2))-5*log(10)mod3*4/5/7)");
    EXPECT_TRUE(fabs(controler.RequestCalculations(0) -  136.558) < 1e-3);
    controler.SetInput("-3.5*cos(x*5)+2.21e-05*(-2+sqrt(x*2.1^2))/3");
    EXPECT_TRUE(fabs(controler.RequestCalculations(3) -  2.658) < 1e-3);
    controler.FreeCalcData();
    controler.SetInput("x + 21");
    EXPECT_TRUE(controler.NeedX());
}


int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
