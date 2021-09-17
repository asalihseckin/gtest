#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;


class EcuBrain
{
public:
    virtual double getProximitySensorData() = 0;
};

class MockEcuBrain : public EcuBrain{
    public : MOCK_METHOD0(getProximitySensorData,double());
};


class Obu {
    EcuBrain & ecu;
    public:
    Obu(EcuBrain & _ecu) : ecu(_ecu) {}

    double RequestProximitySensorData(){
        
        if (ecu.getProximitySensorData() < 10)
        {
            cout<<"Too Close"; return 1;
        }
        else {
            cout<<"Safe"; return 2;
        }
        
    }
};

TEST(EcuTest, Distance){
    MockEcuBrain meb;
    Obu ob(meb);
    
    EXPECT_CALL(meb, getProximitySensorData).Times(1).WillOnce(Return(4.2));
    double retValue = ob.RequestProximitySensorData();

    EXPECT_EQ(retValue, 1);

}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
