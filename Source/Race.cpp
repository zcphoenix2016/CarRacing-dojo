#include "Race.hpp"
#include "ICar.hpp"
#include "ITrack.hpp"
#include "ITeam.hpp"
#include <utility>
#include <algorithm>
#include <stdexcept>

std::vector<ITeam*> Race::extractValidTeams(const std::vector<ITeam*>& p_teams)
{
    std::vector<ITeam*> l_validTeams;
    std::for_each(p_teams.begin(), p_teams.end(),
                  [&](auto p_team)
                     {
                         if(this->validate(*(p_team->getCar())))
                         {
                             l_validTeams.push_back(p_team);
                         }
                     });

    return std::move(l_validTeams);
}

std::vector<unsigned int> Race::race(const std::vector<ITeam*>& p_teams, const ITrack& p_track)
{
    std::vector<std::pair<unsigned int, float>> l_seq;

    std::for_each(p_teams.begin(), p_teams.end(),
                  [&](auto p_team)
                      {
                          l_seq.push_back(std::make_pair(p_team->getId(),
                                                         this->calcTime(*(p_team->getCar()), p_track)));
                      });

    std::sort(l_seq.begin(), l_seq.end(),
              [](auto p_pair1, auto p_pair2){return p_pair1.second < p_pair2.second;});

    std::vector<unsigned int> l_res;
    std::for_each(l_seq.begin(), l_seq.end(),
             [&](auto p_pair){l_res.push_back(p_pair.first);});

    return l_res;
}

std::vector<unsigned int> Race::run(const std::vector<ITeam*>& p_teams, const ITrack& p_track)
{
    std::vector<ITeam*> l_validTeams{extractValidTeams(p_teams)};

    if(2 > l_validTeams.size() or 6 < l_validTeams.size())
    {
        throw std::out_of_range("Number of valid teams is out of range.");
    }

    return race(p_teams, p_track);
}

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



