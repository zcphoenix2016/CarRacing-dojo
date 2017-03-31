#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Race.hpp"
#include "CarMock.hpp"
#include "TrackMock.hpp"
#include <vector>
#include "TeamMock.hpp"
#include <stdexcept>

using ::testing::Return;
using ::testing::AnyNumber;

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
    CarStatus m_carStatus{100, 100, 100};

};

void CarRacingTestSuite::setExpectionForCarStatus(const CarMock& p_car, const CarStatus& p_status)
{
    EXPECT_CALL(p_car, statusOfTire()).Times(AnyNumber()).WillRepeatedly(Return(p_status.tire));
    EXPECT_CALL(p_car, statusOfEngine()).Times(AnyNumber()).WillRepeatedly(Return(p_status.engine));
    EXPECT_CALL(p_car, statusOfSuspension()).Times(AnyNumber()).WillRepeatedly(Return(p_status.suspension));
}

TEST_F(CarRacingTestSuite, notFullyPreparedCarShouldNotJoinTheRace)
{
    CarStatus l_carStatus = {20, 100, 100};
    setExpectionForCarStatus(m_carMock, l_carStatus);

    ASSERT_FALSE(m_race.validate(m_carMock));
}

TEST_F(CarRacingTestSuite, fullyPreparedCarShouldBeAdmitted)
{
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

TEST_F(CarRacingTestSuite, numOfValidCarsShouldNotBeLessThanTwo)
{
    CarMock l_car1, l_car2;
    TeamMock l_team1, l_team2;
    CarStatus l_carStatus{100, 20, 90};
    std::vector<ITeam*> l_teams{&l_team1, &l_team2};

    setExpectionForCarStatus(l_car1, m_carStatus);
    setExpectionForCarStatus(l_car2, l_carStatus);

    EXPECT_CALL(l_team1, getCar()).WillRepeatedly(Return(&l_car1));
    EXPECT_CALL(l_team2, getCar()).WillRepeatedly(Return(&l_car2));

    ASSERT_THROW(m_race.run(l_teams, m_trackMock), std::out_of_range);
}

TEST_F(CarRacingTestSuite, numOfValidCarsShouldNotBeMoreThanSix)
{
    CarMock l_car1, l_car2, l_car3, l_car4, l_car5, l_car6, l_car7;
    TeamMock l_team1, l_team2, l_team3, l_team4, l_team5, l_team6, l_team7;
    std::vector<CarMock*> l_cars{&l_car1, &l_car2, &l_car3, &l_car4, &l_car5, &l_car6, &l_car7};
    std::vector<ITeam*> l_teams{&l_team1, &l_team2, &l_team3, &l_team4, &l_team5, &l_team6, &l_team7};

    for(auto l_car : l_cars)
    {
        setExpectionForCarStatus(*l_car, m_carStatus);
    }

    EXPECT_CALL(l_team1, getCar()).WillRepeatedly(Return(&l_car1));
    EXPECT_CALL(l_team2, getCar()).WillRepeatedly(Return(&l_car2));
    EXPECT_CALL(l_team3, getCar()).WillRepeatedly(Return(&l_car3));
    EXPECT_CALL(l_team4, getCar()).WillRepeatedly(Return(&l_car4));
    EXPECT_CALL(l_team5, getCar()).WillRepeatedly(Return(&l_car5));
    EXPECT_CALL(l_team6, getCar()).WillRepeatedly(Return(&l_car6));
    EXPECT_CALL(l_team7, getCar()).WillRepeatedly(Return(&l_car7));

    ASSERT_THROW(m_race.run(l_teams, m_trackMock), std::out_of_range);
}

TEST_F(CarRacingTestSuite, TeamWithLessTimeShouldWin)
{
    CarMock l_car1, l_car2, l_car3, l_car4, l_car5;
    TeamMock l_team1, l_team2, l_team3, l_team4, l_team5;
    std::vector<ITeam*> l_teams{&l_team1, &l_team2, &l_team3, &l_team4, &l_team5};

    setExpectionForCarStatus(l_car1, m_carStatus);
    setExpectionForCarStatus(l_car2, m_carStatus);
    setExpectionForCarStatus(l_car3, m_carStatus);
    setExpectionForCarStatus(l_car4, m_carStatus);
    CarStatus l_carStatus{100, 60, 90};
    setExpectionForCarStatus(l_car5, l_carStatus);

    EXPECT_CALL(l_team1, getCar()).WillRepeatedly(Return(&l_car1));
    EXPECT_CALL(l_team2, getCar()).WillRepeatedly(Return(&l_car2));
    EXPECT_CALL(l_team3, getCar()).WillRepeatedly(Return(&l_car3));
    EXPECT_CALL(l_team4, getCar()).WillRepeatedly(Return(&l_car4));
    EXPECT_CALL(l_team5, getCar()).WillRepeatedly(Return(&l_car5));

    EXPECT_CALL(l_team1, getId()).WillRepeatedly(Return(1));
    EXPECT_CALL(l_team2, getId()).WillRepeatedly(Return(2));
    EXPECT_CALL(l_team3, getId()).WillRepeatedly(Return(3));
    EXPECT_CALL(l_team4, getId()).WillRepeatedly(Return(4));

    EXPECT_CALL(l_car2, qualityOfEngine()).WillOnce(Return(EngineQuality::High));
    EXPECT_CALL(l_car2, handling()).WillOnce(Return(Handling::Good));
    EXPECT_CALL(l_car1, qualityOfEngine()).WillOnce(Return(EngineQuality::Low));
    EXPECT_CALL(l_car1, handling()).WillOnce(Return(Handling::Good));
    EXPECT_CALL(l_car4, qualityOfEngine()).WillOnce(Return(EngineQuality::High));
    EXPECT_CALL(l_car4, handling()).WillOnce(Return(Handling::Bad));
    EXPECT_CALL(l_car3, qualityOfEngine()).WillOnce(Return(EngineQuality::Low));
    EXPECT_CALL(l_car3, handling()).WillOnce(Return(Handling::Bad));

    EXPECT_CALL(m_trackMock, getLength()).WillRepeatedly(Return(500));
    EXPECT_CALL(m_trackMock, getTurns()).WillRepeatedly(Return(6));

    std::vector<unsigned int> l_seq{2, 1, 4, 3};
    ASSERT_EQ(l_seq, m_race.run(l_teams, m_trackMock));

}

