#pragma once

#include <vector>
#include "ICar.hpp"

class ITeam;

using namespace std;

class Race
{
public:
    //vector<int> run(const vector<ITeam *>& p_teams, const Track& p_track);
    bool validate(const ICar& p_car);
};
