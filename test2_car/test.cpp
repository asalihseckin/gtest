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
    virtual bool emergencyAlert(string report) {return true;}
    virtual bool getConnectionStatus(string status) {return true;}
    virtual double getSpeeed(double speed) = 0;
    virtual double getRoadDistance(double roadDistance) = 0;
};

class MockEcuBrain : public EcuBrain{
    public : 
    MOCK_METHOD0(getProximitySensorData,double());
    MOCK_METHOD1(emergencyAlert, bool (string report));
    MOCK_METHOD1(getConnectionStatus, bool (string status));
    MOCK_METHOD1(getSpeeed, double (double speed));
    MOCK_METHOD1(getRoadDistance, double (double roadDistance));
};


class Obu {
    EcuBrain & ecu;
    public:
    Obu(EcuBrain & _ecu) : ecu(_ecu) {}


    double checkTheDistance(){
        double distance{0.0};

        distance= ecu.getProximitySensorData();
        if (distance < 10)
    {
        cout<<"\nToo Close "; return distance;
    }
        else {
            cout<<"Safe "; return distance;
        }
    };

    double brakeTheCar(){
        if (checkTheDistance() < 5)
    {
        cout<<"\nBraking is Done "; return 1;
    }   else {
            cout<<"Braking is not nececary "; return 2;
        }
    };

    bool reportEmergencyAlert(string report,int numberofalerts){
        int i = 0;
        for(i=0;i<numberofalerts;i++){
             if(ecu.emergencyAlert(report) == true){
            cout<<"\nEmergency signal is being distributed "<< i+1 <<"nd Time";
            }
            else{
            cout<<"\nEmergencey Status Nonactive "; return 0;
        }

        }
        return 1;
    };

    bool ConnectDB(string status){
        if(ecu.getConnectionStatus(status) != true){
            cout<<"\nNot Connected "; return 1;
        }  
        else{
            cout<<"\nConnected "; return 0;
        }
    };

    
    double calculateArrivalTime(double speed, double roadDistance){

        double time = ecu.getRoadDistance(roadDistance)/ecu.getSpeeed(speed);
        if(speed >0 && roadDistance>0) {
            cout<<"\nEstimated Time Of Arrival: "<< time;
        } else if(roadDistance<=0){
            cout<<"\nYour Distance Cannot be Negative or Zero! \n";
        } else if(speed<=0){
            cout<<"\nYour Speed is Zero or an Undefined Value! \n";
        }

        return time;
    };
    

    double RequestProximitySensorData() {
        return checkTheDistance();
    }
};

TEST(EcuTest, Distance){
    MockEcuBrain meb;
    Obu ob(meb);
    
    EXPECT_CALL(meb, getProximitySensorData).Times(1).WillOnce(Return(4));
    double retValue = ob.RequestProximitySensorData();

    EXPECT_LT(retValue, 5);

}

TEST(EcuTest, AutoBrake){
    MockEcuBrain meb;
    Obu ob(meb);
    
    EXPECT_CALL(meb, getProximitySensorData).Times(1).WillOnce(Return(-5));
    double retValue = ob.brakeTheCar();

    EXPECT_EQ(retValue, 1);

}

TEST(EcuTest, Alert){
    MockEcuBrain meb;
    Obu ob(meb);
    int numberofalerts =3;

    EXPECT_CALL(meb, emergencyAlert("Alert")).Times(numberofalerts).WillRepeatedly(Return(true));
    double retValue = ob.reportEmergencyAlert("Alert",numberofalerts);

    EXPECT_EQ(retValue, 1);
}

TEST(EcuTest, DBStatus){
    MockEcuBrain meb;
    Obu ob(meb);

    ON_CALL(meb, getConnectionStatus(_)).WillByDefault(Return(true));
    double retValue = ob.ConnectDB("Connect");

    ASSERT_EQ(retValue, 0);
}


TEST(EcuTest, ArrivalTimeGreaterThanTest){
    MockEcuBrain meb;
    Obu ob(meb);
/**
 * TODO: ON_CALL with ile 2 fonksiyonu birden cagirmaya bak
 * */
    ON_CALL(meb, getSpeeed(_)).WillByDefault(Return(80));
    ON_CALL(meb, getRoadDistance(_)).WillByDefault(Return(1800));
    int retValue = ob.calculateArrivalTime(-60,1800);

    EXPECT_GT(retValue, 0) << "Expected value is not greater than zero!";

}

TEST(EcuTest, ArrivalTimeNotEqualTest){
    MockEcuBrain meb;
    Obu ob(meb);

    ON_CALL(meb, getSpeeed(_)).WillByDefault(Return(-60));
    ON_CALL(meb, getRoadDistance(_)).WillByDefault(Return(1800));
    int retValue = ob.calculateArrivalTime(-60,1800);

    EXPECT_NE(retValue, 0) << "Expected value is not equal to zero!";
}

TEST(EcuTest, ArrivalTimeZeroSpeed){
    MockEcuBrain meb;
    Obu ob(meb);

    ON_CALL(meb, getSpeeed(_)).WillByDefault(Return(-60));
    ON_CALL(meb, getRoadDistance(_)).WillByDefault(Return(1800));
    int retValue = ob.calculateArrivalTime(-60,1800);

    EXPECT_NE(retValue, -2147483648) << "Zero speed is not handled!";
}



int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
