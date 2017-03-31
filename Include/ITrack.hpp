#pragma once

class ITrack
{
public:
    virtual ~ITrack(){}
    virtual unsigned int getLength() const = 0;
    virtual unsigned int getTurns() const = 0;
};
