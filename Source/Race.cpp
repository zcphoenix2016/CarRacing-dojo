#include "Race.hpp"
#include "ICar.hpp"
#include "ITrack.hpp"

/*
vector<int> Race::run(const vector<ITeam *>& p_teams, const Track& p_track)
{
    vector<pair<int, int>> l_vecSeq;

    for_each(p_teams.begin(), p_teams.end(),
             [&](auto p){ l_vecSeq.push_back(make_pair(p->getId(), p->getTotalTime()));});

    sort(l_vecSeq.begin(), l_vecSeq.end(),
         [](auto p1, auto p2){ return p1.second < p2.second;});

    vector<int> l_res;
    for_each(l_vecSeq.begin(), l_vecSeq.end(),
             [&](auto p){ l_res.push_back(p.first);});

    return move(l_res);
}
*/

bool Race::validate(const ICar& p_car)
{
    return (100 == p_car.statusOfTire() and
             100 == p_car.statusOfEngine() and
             100 == p_car.statusOfSuspension());
}

float Race::calcTime(const ICar& p_car, const ITrack& p_track)
{
    int l_timeOfLength = (EngineQuality::High == p_car.qualityOfEngine()) ? 5 : 6;
    float l_timeOfTurns = (Handling::Good == p_car.handling()) ? 0.5 : 1.5;

    return p_track.getLength() / 100 * l_timeOfLength + p_track.getTurns()* l_timeOfTurns;
}


