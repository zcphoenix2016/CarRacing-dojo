#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Race.hpp"
#include "CarMock.hpp"
#include "TrackMock.hpp"

using ::testing::Return;

TEST(CarRacingTestSuite, notFullyPreparedCarShouldNotJoinTheRace)
{
    CarMock l_carMock{};
    Race l_race;

    EXPECT_CALL(l_carMock, statusOfTire()).WillOnce(Return(100));
    EXPECT_CALL(l_carMock, statusOfEngine()).WillOnce(Return(100));
    EXPECT_CALL(l_carMock, statusOfSuspension()).WillOnce(Return(20));

    ASSERT_FALSE(l_race.validate(l_carMock));
}

TEST(CarRacingTestSuite, fullyPreparedCarShouldBeAdmitted)
{
    CarMock l_carMock{};
    Race l_race;

    EXPECT_CALL(l_carMock, statusOfTire()).WillOnce(Return(100));
    EXPECT_CALL(l_carMock, statusOfEngine()).WillOnce(Return(100));
    EXPECT_CALL(l_carMock, statusOfSuspension()).WillOnce(Return(100));

    ASSERT_TRUE(l_race.validate(l_carMock));
}

TEST(CarRacingTestSuite, testRaceTimeForCarWithHighEngineWithGoodHandlingOnTrack1)
{
    CarMock l_carMock{};
    TrackMock l_trackMock{};
    Race l_race;
    const float l_expectTime = 28.0;

    EXPECT_CALL(l_carMock, qualityOfEngine()).WillOnce(Return(ENGINEQUALITY_HIGH));
    EXPECT_CALL(l_carMock, handling()).WillOnce(Return(HANDLING_GOOD));
    EXPECT_CALL(l_trackMock, getLength()).WillOnce(Return(500));
    EXPECT_CALL(l_trackMock, getTurns()).WillOnce(Return(6));

    ASSERT_EQ(l_expectTime, l_race.calcTime(l_carMock, l_trackMock));
}




