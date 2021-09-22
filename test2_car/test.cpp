#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

class EcuBrainInterface
{
public:
    virtual double getProximitySensorData() = 0;
    virtual bool emergencyAlert(string report) {return true;}
    virtual bool getConnectionStatus(string status) {return true;}
    virtual double getSpeed(double speed) = 0;
    virtual double getRoadDistance(double roadDistance) = 0;
};

class MockEcuBrain : public EcuBrainInterface{
    public : 
    MOCK_METHOD0(getProximitySensorData,double());
    MOCK_METHOD1(emergencyAlert, bool (string report));
    MOCK_METHOD1(getConnectionStatus, bool (string status));
    MOCK_METHOD1(getSpeed, double (double speed));
    MOCK_METHOD1(getRoadDistance, double (double roadDistance));
};

class Obu {
    EcuBrainInterface & Ecu;
    public:
    Obu(EcuBrainInterface & _ecu) : Ecu(_ecu) {}

    double checkTheDistance(){
        double distance{0.0};

        distance= Ecu.getProximitySensorData();
        if (distance < 10)
    {
        cout<<"\nToo Close "; return distance;
    }
        else {
            cout<<"Safe "; return distance;
        }
    }

    double brakeTheCar(){
        if (checkTheDistance() < 5)
    {
        cout<<"\nBraking is Done ";
            return 1;
    }   else {
            cout<<"Braking is not necessary ";
            return 0;
        }
    }

    bool reportEmergencyAlert(string report,int numberOfAlerts){
        int i = 0;
        for(i=0;i<numberOfAlerts;i++){
             if(Ecu.emergencyAlert(report) == true) {
                 cout<<"\nEmergency signal is being distributed "<< i+1 <<"nd Time";
            } else {
                cout<<"\nEmergencey Status Nonactive "; return false;
            }
        }
        return true;
    }

    bool ConnectDB(string status){
        if(Ecu.getConnectionStatus(status) != true){
            cout<<"\nNot Connected ";
            return true;
        } else{
            cout<<"\nConnected ";
            return false;
        }
    };

    
    double calculateArrivalTime(double speed, double roadDistance){

        /*
         double time = ecu.getRoadDistance(roadDistance)/ecu.getSpeed(speed);
        if(speed >0 && roadDistance>0) {
            cout<<"\nEstimated Time Of Arrival: "<< time;
        } else if(roadDistance<=0) {
            cout<<"\nYour Distance Cannot be Negative or Zero! \n";
        } else if(speed<=0) {
            cout<<"\nYour Speed is Zero or an Undefined Value! \n";
        }
         * */

        //double time = roadDistance / speed;

        if(speed > 0 && roadDistance > 0) {
            double time = roadDistance / speed;
            cout<<"\nEstimated Time Of Arrival: "<< time;
        } else if(roadDistance<=0) {
            cout<<"\nYour Distance Cannot be Negative or Zero! \n";
            return 0.0;
        } else if(speed<=0) {
            cout<<"\nYour Speed is Zero or an Undefined Value! \n";
            return 0.0;
        }
    };
    

    double RequestProximitySensorData() {
        return checkTheDistance();
    }
};

TEST(EcuTest, Distance) {
    MockEcuBrain meb;
    Obu ob(meb);
    
    EXPECT_CALL(meb, getProximitySensorData).Times(1).WillOnce(Return(4));
    double retValue = ob.RequestProximitySensorData();

    EXPECT_LT(retValue, 5);

}

TEST(EcuTest, AutoBrake) {
    MockEcuBrain meb;
    Obu ob(meb);
    
    EXPECT_CALL(meb, getProximitySensorData).Times(1).WillOnce(Return(-5));
    double retValue = ob.brakeTheCar();

    EXPECT_EQ(retValue, 1);

}

TEST(EcuTest, Alert){
    MockEcuBrain meb;
    Obu ob(meb);
    int numberOfAlerts =3;

    /*
     *    EXPECT_CALL(meb, getConnectionStatus("Alert")).Times(numberOfAlerts).WillRepeatedly(Return(true));
     * */
    EXPECT_CALL(meb, emergencyAlert("Alert")).Times(numberOfAlerts).WillRepeatedly(Return(true));
    bool retValue = ob.reportEmergencyAlert("Alert",numberOfAlerts);

    EXPECT_EQ(retValue, true);
}

TEST(EcuTest, DBStatus){
    MockEcuBrain meb;
    Obu ob(meb);

    ON_CALL(meb, getConnectionStatus(_)).WillByDefault(Return(true));
    bool retValue = ob.ConnectDB("Connect");

    ASSERT_EQ(retValue, 0);
}

TEST(EcuTest, ArrivalTimeGreaterThanTest){
    MockEcuBrain meb;
    Obu ob(meb);
    double defaultDistance{1800.0};
    double defaultSpeed{80.0};

    ON_CALL(meb, getConnectionStatus).WillByDefault(Return(defaultDistance));
    ON_CALL(meb, getSpeed(_)).WillByDefault(Return(defaultSpeed));

    double retValue = ob.calculateArrivalTime(defaultSpeed,defaultDistance);

    EXPECT_GT(retValue, 0) << "Expected value is not greater than zero!";

}

TEST(EcuTest, ArrivalTimeNotEqualTest){
    MockEcuBrain meb;
    Obu ob(meb);
    double defaultDistance{1800.0};
    double defaultSpeed{-60};

    ON_CALL(meb, getSpeed(_)).WillByDefault(Return(defaultSpeed));
    ON_CALL(meb, getRoadDistance(_)).WillByDefault(Return(defaultDistance));
    int retValue = ob.calculateArrivalTime(defaultSpeed,defaultDistance);

    EXPECT_EQ(retValue, 0) << "Expected value is not equal to zero!";
}

TEST(EcuTest, ArrivalTimeZeroSpeed){
    MockEcuBrain meb;
    Obu ob(meb);
    double defaultDistance{1800.0};
    double defaultSpeed{0.0};

    /*
     *  ON_CALL ve EXPECT_CALL arasindaki anlatirken ON_CALL un cagirdigi fonksiyonu degistirerek anlat ve hata vermedigini
     *  goster.
     *  *  ON_CALL(meb, getProximitySensorData()).WillByDefault(Return(0));
     * */
    ON_CALL(meb, getSpeed(_)).WillByDefault(Return(0));
    ON_CALL(meb, getRoadDistance(_)).WillByDefault(Return(1800));
    int retValue = ob.calculateArrivalTime(defaultSpeed,defaultDistance);

    EXPECT_EQ(retValue, 0) << "Zero speed is not handled!";
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
