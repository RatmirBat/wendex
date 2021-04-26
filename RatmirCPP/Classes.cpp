#include "WendexTaxi.h"

vector<string> split(string s, string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}
vector<string> ParseStringToValues(string s, string delimeter) {
    return split(s, delimeter);
}
string ParseValuesToString(vector<string> values, string delimeter)
{
    string result = "";
    for (string item : values)
        result += item + delimeter;
    return result;
}

Passanger::Passanger(string name, string password)
    : User() {
    Type = UserType::IsPassanger;
    Name = name;
    Password = password;
};
string Passanger::ParsePinnedAdresses() {
    int size = this->PinnedAdresses.size();
    return "";
}
Driver::Driver(string name, string password)
    : User() {
    Type = UserType::IsDriver;
    Name = name;
    Password = password;
};
bool Gateway::UserRegistered(string database, string username)
{
    ifstream input;
    string line;
    input.open(database);
    if (input.is_open()) {
        while (getline(input, line)) {
            if (ParseStringToValues(line, "/;")[0] == username) {
                return true;
            }
        }
    }
    return false;
}
vector<string> Gateway::SelectFromDatabase(string database, string username)
{
    ifstream input;
    string line;
    vector<string> values;
    input.open(database);
    if (input.is_open()) {
        while (getline(input, line)) {
            values = split(line, "/;");
            if (values[0] == username) {
                break;
            }
        }
    }
    return values;
}

PassangerGateway::PassangerGateway(Passanger *p)
    : Gateway() {
    passanger = p;
}
bool PassangerGateway::Login()
{
    if (UserRegistered(this->database, this->passanger->Name)) {
        vector<string> values = SelectFromDatabase(this->database, this->passanger->Name);
        return this->passanger->Password == values[1];
    }
}
void PassangerGateway::UpdateDatabase()
{
    ifstream input;
    ofstream output;
    input.open(this->database);
    vector<string> orderhistory;
    for (PassangerOrder order : this->passanger->OrderHistory)
        orderhistory.push_back(order.ToString("[;"));
    string passanger_data = this->passanger->Name + "/;" + // name
        this->passanger->Password + "/;" +
        "5" + "/;" + 
        ParseValuesToString(orderhistory, "") + "/;" + 
        to_string(this->passanger->PaymentMethod) + "/;" + 
        ParseValuesToString(this->passanger->PinnedAdresses, "];") + "/;";
    vector<string> val;
    if (input.is_open()) {
        string line;
        while (getline(input, line)) {
            if (split(line, "/;")[0] == this->passanger->Name) {
                val.push_back(passanger_data);
            } else {
                val.push_back(line);
            }
        }
    }
    input.close();
    output.open(this->database);
    if (output.is_open()) {
        output << ParseValuesToString(val, "\n");
    }
}
void PassangerGateway::Register()
{
    if (UserRegistered(this->database, this->passanger->Name)) {
        cout << "User \"" << this->passanger->Name + "\" already registered.";
    } else {
        ofstream output;
        output.open(this->database, ios::app);
        if (output.is_open()) {
            output <<
                this->passanger->Name + "/;" +
                this->passanger->Password + "/;" +
                "5" + "/;" + 
                "1" + "/;" + 
                to_string(this->passanger->PaymentMethod) + "/;" + 
                ParseValuesToString(this->passanger->PinnedAdresses, "];") + "/;\n";
        }
    }
}
void PassangerGateway::UpdatePaymentMethod(PaymentMethod p) {
    this->passanger->PaymentMethod = p;
}

DriverGateway::DriverGateway(Driver* p)
    : Gateway()
{
    driver = p;
}
bool DriverGateway::Login()
{
    if (UserRegistered(this->database, this->driver->Name)) {
        vector<string> values = SelectFromDatabase(this->database, this->driver->Name);
        return this->driver->Password == values[1];
    }
}
void DriverGateway::UpdateDatabase()
{
    ifstream input;
    ofstream output;
    input.open(this->database);
    string driver_data = this->driver->Name + "/;" + // name
        this->driver->Password + "/;" +
        "5" + "/;" + 
        "1" + "/;" + 
        this->driver->Car->ToString("];") + "/;" + 
        to_string(this->driver->Status) + "/;";
    vector<string> val;
    if (input.is_open()) {
        string line;
        while (getline(input, line)) {
            if (split(line, "/;")[0] == this->driver->Name) {
                val.push_back(driver_data);
            } else {
                val.push_back(line);
            }
        }
    }
    input.close();
    output.open(this->database);
    if (output.is_open()) {
        output << ParseValuesToString(val, "\n");
    }
}
void DriverGateway::Register()
{
    if (UserRegistered(this->database, this->driver->Name)) {
        cout << "User \"" << this->driver->Name + "\" already registered.";
    } else {
        ofstream output;
        output.open(this->database, ios::app);
        if (output.is_open()) {
            output << this->driver->Name << "/;" << // name
                this->driver->Password << "/;" << // password
                5 << "/;" << // rating
                1 << "/;" << // order history
                this->driver->Car->ToString("];") << "/;" << // car
                this->driver->Status << "/;" << // status
                endl;
        }
    }
}

Car::Car(string m, string cc, string clr, string n, CarType ctype)
{
    Model = m;
    CurrentCoordinates = cc;
    Color = clr;
    Number = n;
    Type = ctype;
}
string Car::ToString(string delimetr) {
    return Model + delimetr + to_string(Type) + delimetr + CurrentCoordinates + delimetr + Color + delimetr + Number + delimetr + to_string(freeBottleOfwater);
}

PassangerOrder::PassangerOrder(Driver* d, string time, string from, string to, double price) {
    driver = d;
    OrderTime = time;
    From = from;
    To = to;
    Price = price;
}
string PassangerOrder::ToString(string delimetr) {
    return this->driver->Name + delimetr + OrderTime + delimetr + this->From + delimetr + this->To + delimetr + to_string(this->Price) + "=;";
}

vector<PassangerOrder> PassangerGateway::GetOrderHistory()
{
    vector<PassangerOrder> values;
    for ()
    return values;
}