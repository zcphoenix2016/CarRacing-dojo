#include "ICar.hpp"

class CarMock : public ICar
{
public:
    MOCK_CONST_METHOD0(statusOfTire, unsigned int());
    MOCK_CONST_METHOD0(statusOfEngine, unsigned int());
    MOCK_CONST_METHOD0(statusOfSuspension, unsigned int());
    MOCK_CONST_METHOD0(qualityOfEngine, EngineQuality());
    MOCK_CONST_METHOD0(handling, Handling());
};
