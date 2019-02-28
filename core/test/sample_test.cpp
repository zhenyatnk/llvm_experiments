#include <llvm_experiments/core/BaseExceptions.hpp>

#include <gtest/gtest.h>

//--------------------------------------------------
using namespace llvm_experiments::core;

class sample_test
    :public ::testing::Test
{
public:
    void SetUp()
    {}
};

TEST_F(sample_test, fake)
{
}
