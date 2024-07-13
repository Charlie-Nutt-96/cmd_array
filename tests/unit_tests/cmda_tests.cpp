#include <array.h>
#include <gtest/gtest.h>

TEST(ConstructorTests, testInt1dConstructNull)
{
    cmda::Array<int,1> arr;
    ASSERT_EQ(arr.dim(),1);
    ASSERT_EQ(arr.data(),nullptr);
    ASSERT_EQ(arr.size(),0);
    ASSERT_EQ(arr.n(0),0);
    ASSERT_TRUE(arr.is_empty());
}

TEST(ConstructorTests, testInt2dConstructNull)
{
    cmda::Array<int,2> arr;
    ASSERT_EQ(arr.dim(),2);
    ASSERT_EQ(arr.data(),nullptr);
    ASSERT_EQ(arr.size(),0);
    ASSERT_EQ(arr.n(0),0);
    ASSERT_TRUE(arr.is_empty());
}


TEST(ConstructorTests, testInt3dConstructNull)
{
    cmda::Array<int,3> arr;
    ASSERT_EQ(arr.dim(),3);
    ASSERT_EQ(arr.data(),nullptr);
    ASSERT_EQ(arr.size(),0);
    ASSERT_EQ(arr.n(0),0);
    ASSERT_TRUE(arr.is_empty());
}


TEST(ConstructorTests, testInt4dConstructNull)
{
    cmda::Array<int,4> arr;
    ASSERT_EQ(arr.dim(),4);
    ASSERT_EQ(arr.data(),nullptr);
    ASSERT_EQ(arr.size(),0);
    ASSERT_EQ(arr.n(0),0);
    ASSERT_TRUE(arr.is_empty());
}