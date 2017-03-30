#pragma once

#include <vector>

class ICar;
class ITrack;

using namespace std;

class Race
{
public:
    //vector<int> run(const vector<ITeam *>& p_teams, const Track& p_track);
    bool validate(const ICar&);
    float calcTime(const ICar&, const ITrack&);
};
