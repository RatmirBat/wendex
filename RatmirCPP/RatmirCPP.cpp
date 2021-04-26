#include "WendexTaxi.h"
using namespace std;

int main()
{   
    Driver* d = new Driver("test", "123");
    Car* c = new Car("Chevrolet", "Kazan, Kremlevskaya 35", "Black", "11-012R405", CarType::Economy);
    DriverGateway dgateway(d);
    dgateway.driver->Car = c;

    dgateway.Register();
    dgateway.UpdateDatabase();

    Passanger* p = new Passanger("lox", "password");
    PassangerGateway pgateway(p);

    pgateway.Register();
    pgateway.UpdateDatabase();

    PassangerOrder *porder = new PassangerOrder(d, "today", "hz", "huy znaet", 12);
    pgateway.passanger->OrderHistory.push_back(*porder);
    pgateway.passanger->OrderHistory.push_back(*porder);
    pgateway.UpdateDatabase();

    return 0;
}