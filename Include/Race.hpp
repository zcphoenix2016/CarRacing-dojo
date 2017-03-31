#pragma once

#include <vector>

class ICar;
class ITrack;
class ITeam;

class Race
{
public:
    std::vector<unsigned int> run(const std::vector<ITeam*>&, const ITrack&);
    bool validate(const ICar&);
    float calcTime(const ICar&, const ITrack&);
};
