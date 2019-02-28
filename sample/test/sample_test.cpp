#include <templateproj/core/BaseExceptions.hpp>

#include <gtest/gtest.h>

//--------------------------------------------------
using namespace templateproj::core;

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