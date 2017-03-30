#pragma once

class ICar
{
public:
    virtual ~ICar(){};
    virtual int statusOfTire() const = 0;
    virtual int statusOfEngine() const = 0;
    virtual int statusOfSuspension() const = 0;
};
