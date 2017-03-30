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

struct TrackParams
{
    int length;
    int turns;
};

struct RaceCalcTimeTestParams
{
    EngineQuality engineQuality;
    Handling      handling;
    TrackParams   trackParams;
    float         expectedTime;
};

class RaceCalcTimeTestSuite
    : public CarRacingTestSuite,
      public ::testing::WithParamInterface<RaceCalcTimeTestParams>
      {};

INSTANTIATE_TEST_CASE_P(test_RaceCalcTimeWithDifferentParams,
                        RaceCalcTimeTestSuite,
                        ::testing::Values(
                            RaceCalcTimeTestParams{EngineQuality::High, Handling::Good, {500, 6}, 28.0},
                            RaceCalcTimeTestParams{EngineQuality::Low, Handling::Good, {500, 6}, 33.0},
                            RaceCalcTimeTestParams{EngineQuality::High, Handling::Bad, {500, 6}, 34.0},
                            RaceCalcTimeTestParams{EngineQuality::Low, Handling::Bad, {500, 6}, 39.0},
                            RaceCalcTimeTestParams{EngineQuality::High, Handling::Good, {1000, 3}, 51.5},
                            RaceCalcTimeTestParams{EngineQuality::Low, Handling::Good, {1000, 3}, 61.5},
                            RaceCalcTimeTestParams{EngineQuality::High, Handling::Bad, {1000, 3}, 54.5},
                            RaceCalcTimeTestParams{EngineQuality::Low, Handling::Bad, {1000, 3}, 64.5}));

TEST_P(RaceCalcTimeTestSuite, RaceCalcTimeInDifferentParams)
{
    EXPECT_CALL(m_carMock, qualityOfEngine()).WillOnce(Return(GetParam().engineQuality));
    EXPECT_CALL(m_carMock, handling()).WillOnce(Return(GetParam().handling));
    EXPECT_CALL(m_trackMock, getLength()).WillOnce(Return(GetParam().trackParams.length));
    EXPECT_CALL(m_trackMock, getTurns()).WillOnce(Return(GetParam().trackParams.turns));

    ASSERT_EQ(GetParam().expectedTime, m_race.calcTime(m_carMock, m_trackMock));
}


