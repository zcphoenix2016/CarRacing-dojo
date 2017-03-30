#pragma once

enum ENGINEQUALITY
{
    HIGH,
    LOW
};

enum HANDLING
{
    GOOD,
    BAD
};

class ICar
{
public:
    virtual ~ICar(){}
    virtual int statusOfTire() const = 0;
    virtual int statusOfEngine() const = 0;
    virtual int statusOfSuspension() const = 0;
    virtual ENGINEQUALITY qualityOfEngine() const = 0;
    virtual HANDLING handling() const = 0;
};
