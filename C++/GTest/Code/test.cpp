#include <gtest/gtest.h>
#include <iostream>

using namespace std;

// 被测试的类
class Calculator {
public:
    int Add(int a, int b)
    { 
        return a + b; 
    }

    int Subtract(int a, int b)
    {
        return a - b;
    }
};

// 定义测试套件
class CalculatorTest : public ::testing::Test {
protected:
    // 在每个测试用例运行之前调用
    static void SetUpTestSuite()
    {
        sharedCalc = new Calculator();
    }
    // 在每个测试用例运行之后调用
    static void TearDownTestSuite()
    {
        delete sharedCalc;
    }

    // 测试套件中所有测试共享的资源
    static Calculator* sharedCalc;
};

Calculator* CalculatorTest::sharedCalc = nullptr;

// 定义属于测试套件的测试用例
TEST_F(CalculatorTest, SharedAddTest)
{
    ASSERT_EQ(sharedCalc->Add(1, 1), 2);
}

TEST_F(CalculatorTest, SharedSubtractTest)
{
    ASSERT_EQ(sharedCalc->Subtract(10, 5), 5);
}


int main(int argc, char *argv[])
{
    //g++ test.cpp -lgtest -lpthread
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
