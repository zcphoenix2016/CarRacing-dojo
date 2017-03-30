#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Race.hpp"
#include "CarMock.hpp"

using ::testing::Return;

TEST(CarRacingTestSuite, NotFullyPreparedCarShouldNotJoinTheRace)
{
    CarMock l_carMock{};
    Race l_race;

    EXPECT_CALL(l_carMock, statusOfTire()).WillOnce(Return(100));
    EXPECT_CALL(l_carMock, statusOfEngine()).WillOnce(Return(100));
    EXPECT_CALL(l_carMock, statusOfSuspension()).WillOnce(Return(20));

    ASSERT_FALSE(l_race.validate(l_carMock));
}

TEST(CarRacingTestSuite, FullyPreparedCarShouldJoinTheRace)
{
    CarMock l_carMock{};
    Race l_race;

    EXPECT_CALL(l_carMock, statusOfTire()).WillOnce(Return(100));
    EXPECT_CALL(l_carMock, statusOfEngine()).WillOnce(Return(100));
    EXPECT_CALL(l_carMock, statusOfSuspension()).WillOnce(Return(100));

    ASSERT_TRUE(l_race.validate(l_carMock));
}


