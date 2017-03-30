#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Race.hpp"
#include "CarMock.hpp"
#include "TrackMock.hpp"

using ::testing::Return;

class CarRacingTestSuite : public ::testing::Test
{
public:
    CarMock m_carMock{};
    Race m_race;
    TrackMock m_trackMock{};

};

TEST_F(CarRacingTestSuite, notFullyPreparedCarShouldNotJoinTheRace)
{
    EXPECT_CALL(m_carMock, statusOfTire()).WillOnce(Return(100));
    EXPECT_CALL(m_carMock, statusOfEngine()).WillOnce(Return(100));
    EXPECT_CALL(m_carMock, statusOfSuspension()).WillOnce(Return(20));

    ASSERT_FALSE(m_race.validate(m_carMock));
}

TEST_F(CarRacingTestSuite, fullyPreparedCarShouldBeAdmitted)
{
    EXPECT_CALL(m_carMock, statusOfTire()).WillOnce(Return(100));
    EXPECT_CALL(m_carMock, statusOfEngine()).WillOnce(Return(100));
    EXPECT_CALL(m_carMock, statusOfSuspension()).WillOnce(Return(100));

    ASSERT_TRUE(m_race.validate(m_carMock));
}

TEST_F(CarRacingTestSuite, testRaceTimeForCarWithHighEngineWithGoodHandlingOnTrack1)
{
    const float l_expectTime = 28.0;

    EXPECT_CALL(m_carMock, qualityOfEngine()).WillOnce(Return(ENGINEQUALITY_HIGH));
    EXPECT_CALL(m_carMock, handling()).WillOnce(Return(HANDLING_GOOD));
    EXPECT_CALL(m_trackMock, getLength()).WillOnce(Return(500));
    EXPECT_CALL(m_trackMock, getTurns()).WillOnce(Return(6));

    ASSERT_EQ(l_expectTime, m_race.calcTime(m_carMock, m_trackMock));
}

