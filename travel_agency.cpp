#include <iostream>
#include <cstring>
#include <vector>
#include <typeinfo>

using namespace std;

class Discount
{
private:
public:
    virtual ostream & write(ostream &os)const
    {
        return os;
    }
};

class MountainDiscount : public Discount
{
private:
    float vald1 = 40;
public:
    void setMountainDiscount(int x)
    {
        vald1 = x;
    }
    float getMountainDiscount()
    {
        return vald1;
    };
};

class IslandDiscount : public Discount
{
private:
    float vald2 = 50;
public:
    void setIslandDiscount(int x)
    {
        vald2 = x;
    }
    float getIslandDiscount()
    {
        return vald2;
    };
};

class FinalDiscount : public Discount
{
private:
    float valdf = 15;
public:
    void setFinalDiscount(int x)
    {
        valdf = x;
    }
    float getFinalDiscount()
    {
        return valdf;
    };
};

class Review {
private:
    string clientName;
    int accomodationPeriod;
    int score;
public:
    Review(string clientName="", int accomodationPeriod=0, int score=0) : clientName(clientName), accomodationPeriod(accomodationPeriod), score(score) {}

    friend istream &operator>>(istream &is, Review &review) {
        cout << "Client Name: ";
        is >> review.clientName;
        cout << "Accomodation Period: ";
        is >> review.accomodationPeriod;
        cout << "Score: ";
        is >> review.score;
        return is;
    }

    friend ostream &operator<<(ostream &os, const Review &review) {
        os << "ClientName: " << review.clientName << " AccomodationPeriod: " << review.accomodationPeriod << " Score: " << review.score;
        return os;
    }
};

vector<Review> v;

class Destination {
protected:
    vector<Review> list;
    string country;
    string city;
    string name;
    float price;
public:
    Destination(const vector<Review> list=v, string country="", string city="", string name="", float price=0) :
                    list(list), country(country), city(city), name(name), price(price) {}

    float getPrice()
    {
        return price;
    }

    void setPrice(float price) {
        Destination::price = price;
    }

    virtual istream& read (istream &is) {
        cout << "Country: ";
        is >> country;
        cout << "City: ";
        is >> city;
        cout << "Name: ";
        is >> name;
        cout << "Price: ";
        is >> price;
        cout << "Number of reviews: ";
        int n;
        is >> n;
        for (int i=0;i<n;i++)
        {
            Review r;
            cout << "Review " <<i+1<<" :";
            is >> r;
            list.push_back(r);
        }
        return is;
    }

    virtual ostream& write (ostream &os) {
        os << " Country: " << country;
        os << " City: " << city;
        os << " Name: " << name;
        os << " Price: " << price;
        int i;
        for (i=0;i<list.size();i++) {
            os << "Review " << i + 1 << " :";
            os << list[i];
        }
        return os;
    }

    friend istream &operator>>(istream &is, Destination &destination) {
        destination.read(is);
        return is;
    }

    friend ostream &operator<<(ostream &os, Destination &destination) {
        destination.write(os);
        return os;
    }

};

class MountainDestination : public Destination {
private:
    float height;
    float distance;
    int routes;
public:
    MountainDestination(const vector<Review> list=v, string country="", string city="", string name="", float price=0, float height=0, float distance=0, int routes=0) :
                    Destination(list, country, city, name, price), height(height), distance(distance), routes(routes) {}

    istream& read (istream &is) {
        Destination::read(is);
        cout << "Height: ";
        is >> height;
        cout << "Distance: ";
        is >> distance;
        cout << "Routes: ";
        is >> routes;
        return is;
    }

    ostream& write (ostream &os) {
        Destination::write(os);
        os << " Height: " << height << " Distance: " << distance << " Routes: " << routes;
        return os;
    }
};

class IslandDestination : public Destination {
private:
    int islandNo;
    bool view; //0 pt mare, 1 pt ocean
public:
    IslandDestination(const vector<Review> list=v, string country="", string city="", string name="", float price=0, int islandNo=0, bool view=0) :
                    Destination(list, country, city, name, price), islandNo(islandNo), view(view) {}

    istream& read (istream &is) {
        Destination::read(is);
        cout << "IslandNo: ";
        is >> islandNo;
        cout << "View: ";
        is >> view;
        return is;
    }

    ostream& write (ostream &os) {
        Destination::write(os);
        if(view==0)
            os << " IslandNo: " << islandNo << " View: sea";
        if(view==1)
            os << " IslandNo: " << islandNo << " View: ocean";
        return os;
    }
};

vector<Destination*> d;

class TravelAgency {
private:
    vector<Destination*> destinations;
    vector<Discount*> discounts;
public:
//    TravelAgency(const vector<Destination*> &destinations=d) : destinations(destinations) {}
    int getDestinationNumber() {
        return destinations.size();
    }

    void deleteDestination(int n) {
        destinations.erase(destinations.begin()+n-1);
    }

    template<class Destination>void addDestination(Destination d) {
        Destination* obj;
        obj=new Destination;
        *obj=d;
        destinations.push_back(obj);
    }
    friend ostream &operator<<(ostream &os, const TravelAgency &travelAgency) {
        int c=1;
        for (auto i:travelAgency.destinations){
            os << "Destination " << c << ": ";
            os << *i <<"\n";
            c++;}
        return os;
    }
    void setDiscount(Discount &discount, int x )
    {
        try
        {
            dynamic_cast<MountainDiscount&>(discount).setMountainDiscount(x);
        }
        catch (bad_cast&) {};
        try
        {
            dynamic_cast<IslandDiscount&>(discount).setIslandDiscount(x);
        }
        catch (bad_cast&) {};
        try
        {
            dynamic_cast<FinalDiscount&>(discount).setFinalDiscount(x);
        }
        catch (bad_cast&) {};
    }

    void applyDiscount(Destination& destination)
    {
        cout << destination;
        int i,valM = 0;
        for (i = 0; i <discounts.size(); i++)
        {
            if (dynamic_cast<MountainDiscount*>(discounts[i]) != nullptr)
            {
                if (dynamic_cast<MountainDiscount*>(discounts[i])->getMountainDiscount() > valM)
                {
                    valM = dynamic_cast<MountainDiscount*>(discounts[i])->getMountainDiscount();

                }
            }
            else if (dynamic_cast<IslandDiscount*>(discounts[i]) != nullptr)
            {
                if (dynamic_cast<IslandDiscount*>(discounts[i])->getIslandDiscount() > valM)
                {
                    valM = dynamic_cast<IslandDiscount*>(discounts[i])->getIslandDiscount();
                }
            }
            else if (dynamic_cast<FinalDiscount*>(discounts[i]) != nullptr)
            {
                if ((dynamic_cast<FinalDiscount*>(discounts[i])->getFinalDiscount()/100)*destination.getPrice() > valM)
                {
                    valM = (dynamic_cast<FinalDiscount*>(discounts[i])->getFinalDiscount() / 100) * destination.getPrice();
                }
            }
        }
        cout << destination.getPrice() - valM;
    }

};

int main() {
    TravelAgency agency;

    MountainDestination m1;
    MountainDestination m2;
    MountainDestination m3;
    IslandDestination i1;
    IslandDestination i2;

    agency.addDestination(m1);
    agency.addDestination(m2);
    agency.addDestination(m3);
    agency.addDestination(i1);
    agency.addDestination(i2);

    MountainDiscount d1;
    IslandDiscount d2;

    cout<< "Chose a task \n";
    cout<< "1: Add a destination \n";
    cout<< "2: Remove a destination \n";
    cout<< "3: List all available destinations \n";
    cout<< "4: Edit discounts \n";
    cout<< "5: Exit \n";
    cout<< "Input:";
    int n;

    do {
        cin>>n;
        if(n<1 || n>5) {
            cout << "N/A \n";
        }
        if(n==1){
            cout<< "Destination type \n";
            cout<< "1: Mountain Destination \n";
            cout<< "2: Island destination \n";
            cout<< "Input:";
            int n1;
            cin>>n1;
            if(n1<1||n1>2){
                cout<< "N/A \n";
            }
            else if(n1==1){
                MountainDestination o;
                cin>>o;
                agency.addDestination(o);
            }
            else if(n1==2){
                IslandDestination o;
                cin>>o;
                agency.addDestination(o);
            }
        }
        else if(n==2) {
            cout << agency << "\n";
            if (agency.getDestinationNumber() == 0)
                cout << "There are no available destinations \n";
            else {
                cout << "Select the number of the destination you wish to remove \n";
                cout << "Input:";
                int n2;
                cin >> n2;
                if (n2 >= 1 && n2 <= agency.getDestinationNumber()) {
                    agency.deleteDestination(n2);
                }
                else
                    cout<<"There is no destination number " <<n2<<"\n";
            }
        }
        else if(n==3){
            if(agency.getDestinationNumber()==0)
                cout<<"There are no available destinations \n";
            else
                cout<<agency;
        }
        else if(n==4){
            cout<<"Select the discount type you wish to change \n";
            cout<<"1: Mountain discount \n";
            cout<<"2: Island discount \n";
            cout<<"Input:";
            int n3;
            cin>>n3;
            cout<<"Type in the new discount \n";
            cout<<"Input:";
            int nd;
            cin>>nd;
            if(n3==1)
                agency.setDiscount(d1,nd);
            else if(n3==2)
                agency.setDiscount(d2,nd);
            else
                cout<<"N/A \n";
        }
        else if(n==5){
            break;
        }

        cout<< "1: Add a destination \n";
        cout<< "2: Remove a destination \n";
        cout<< "3: List all available destinations \n";
        cout<< "4: Edit discounts \n";
        cout<< "5: Exit \n";
        cout<< "Input:";
    } while (n!=5);
}


