#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Race.hpp"
#include "CarMock.hpp"
#include "TrackMock.hpp"
#include <vector>
#include "TeamMock.hpp"
#include <functional>

using ::testing::Return;

struct CarStatus
{
    int tire;
    int engine;
    int suspension;
};

class CarRacingTestSuite : public ::testing::Test
{
public:
    void setExpectionForCarStatus(const CarMock&, const CarStatus&);

    Race      m_race;
    TrackMock m_trackMock{};
    CarMock   m_carMock{};
    CarStatus m_carStatus{};

};

void CarRacingTestSuite::setExpectionForCarStatus(const CarMock& p_car, const CarStatus& p_status)
{
    EXPECT_CALL(p_car, statusOfTire()).WillOnce(Return(p_status.tire));
    EXPECT_CALL(p_car, statusOfEngine()).WillOnce(Return(p_status.engine));
    EXPECT_CALL(p_car, statusOfSuspension()).WillOnce(Return(p_status.suspension));
}

TEST_F(CarRacingTestSuite, notFullyPreparedCarShouldNotJoinTheRace)
{
    m_carStatus = {100, 100, 20};
    setExpectionForCarStatus(m_carMock, m_carStatus);

    ASSERT_FALSE(m_race.validate(m_carMock));
}

TEST_F(CarRacingTestSuite, fullyPreparedCarShouldBeAdmitted)
{
    m_carStatus = {100, 100, 100};
    setExpectionForCarStatus(m_carMock, m_carStatus);

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
                            RaceCalcTimeTestParams{EngineQuality::Low,  Handling::Good, {500, 6}, 33.0},
                            RaceCalcTimeTestParams{EngineQuality::High, Handling::Bad,  {500, 6}, 34.0},
                            RaceCalcTimeTestParams{EngineQuality::Low,  Handling::Bad,  {500, 6}, 39.0},
                            RaceCalcTimeTestParams{EngineQuality::High, Handling::Good, {1000, 3}, 51.5},
                            RaceCalcTimeTestParams{EngineQuality::Low,  Handling::Good, {1000, 3}, 61.5},
                            RaceCalcTimeTestParams{EngineQuality::High, Handling::Bad,  {1000, 3}, 54.5},
                            RaceCalcTimeTestParams{EngineQuality::Low,  Handling::Bad,  {1000, 3}, 64.5}));

TEST_P(RaceCalcTimeTestSuite, RaceCalcTimeInDifferentParams)
{
    EXPECT_CALL(m_carMock, qualityOfEngine()).WillOnce(Return(GetParam().engineQuality));
    EXPECT_CALL(m_carMock, handling()).WillOnce(Return(GetParam().handling));
    EXPECT_CALL(m_trackMock, getLength()).WillOnce(Return(GetParam().trackParams.length));
    EXPECT_CALL(m_trackMock, getTurns()).WillOnce(Return(GetParam().trackParams.turns));

    ASSERT_EQ(GetParam().expectedTime, m_race.calcTime(m_carMock, m_trackMock));
}

TEST_F(CarRacingTestSuite, TeamWithLessTimeShouldWin)
{
    CarMock l_car1;
    CarMock l_car2;
    TeamMock l_team1;
    TeamMock l_team2;
    std::vector<ITeam*> l_teams{&l_team1, &l_team2};
    std::vector<unsigned int> l_seq{2, 1};

    EXPECT_CALL(l_team1, getCar()).WillRepeatedly(Return(&l_car1));
    EXPECT_CALL(l_team2, getCar()).WillRepeatedly(Return(&l_car2));
    EXPECT_CALL(l_team1, getId()).WillRepeatedly(Return(1));
    EXPECT_CALL(l_team2, getId()).WillRepeatedly(Return(2));

    //EXPECT_CALL(l_car1, statusOfTire()).WillOnce(Return(100));
    //EXPECT_CALL(l_car1, statusOfEngine()).WillOnce(Return(100));
    //EXPECT_CALL(l_car1, statusOfSuspension()).WillOnce(Return(100));
    EXPECT_CALL(l_car1, qualityOfEngine()).WillOnce(Return(EngineQuality::High));
    EXPECT_CALL(l_car1, handling()).WillOnce(Return(Handling::Bad));

    //EXPECT_CALL(l_car2, statusOfTire()).WillOnce(Return(100));
    //EXPECT_CALL(l_car2, statusOfEngine()).WillOnce(Return(100));
    //EXPECT_CALL(l_car2, statusOfSuspension()).WillOnce(Return(100));
    EXPECT_CALL(l_car2, qualityOfEngine()).WillOnce(Return(EngineQuality::High));
    EXPECT_CALL(l_car2, handling()).WillOnce(Return(Handling::Good));

    EXPECT_CALL(m_trackMock, getLength()).WillRepeatedly(Return(500));
    EXPECT_CALL(m_trackMock, getTurns()).WillRepeatedly(Return(6));

    ASSERT_EQ(l_seq, m_race.run(l_teams, m_trackMock));

}

