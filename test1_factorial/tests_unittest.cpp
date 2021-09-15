#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "test1.h"

namespace {

TEST(FakoriyelTest, Negatif) {
  EXPECT_EQ(1, Fakoriyel(-4));
  EXPECT_EQ(1, Fakoriyel(-25));
  EXPECT_GT(Fakoriyel(-2), 0);
}

TEST(FakoriyelTest, Sifir) {
  EXPECT_EQ(1, Fakoriyel(0));
}

TEST(FakoriyelTest, Pozitif) {
  EXPECT_EQ(1, Fakoriyel(1));
  EXPECT_EQ(24, Fakoriyel(4));
  EXPECT_EQ(40320, Fakoriyel(8));
}


TEST(AsalMiTest, Negatif) {
  EXPECT_FALSE(AsalMi(-5));
  EXPECT_FALSE(AsalMi(-1));
  EXPECT_FALSE(AsalMi(SCHAR_MIN));
}

TEST(AsalMiTest, Onemsiz) {
  EXPECT_FALSE(AsalMi(0));
  EXPECT_FALSE(AsalMi(1));
  EXPECT_TRUE(AsalMi(2));
  EXPECT_TRUE(AsalMi(3));
}

TEST(AsalMiTest, Pozitif) {
  EXPECT_FALSE(AsalMi(6));
  EXPECT_TRUE(AsalMi(7));
  EXPECT_FALSE(AsalMi(122));
  EXPECT_TRUE(AsalMi(101));
}

}