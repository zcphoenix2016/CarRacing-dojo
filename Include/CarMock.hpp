#include "ICar.hpp"

class CarMock : public ICar
{
public:

    MOCK_CONST_METHOD0(statusOfTire, int());
    MOCK_CONST_METHOD0(statusOfEngine, int());
    MOCK_CONST_METHOD0(statusOfSuspension, int());
};
