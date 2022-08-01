#include <iostream>
#include <cstring>
#include <vector>
using namespace std;
struct Date{
    int day, month, year;
};
class Product
{
private:
    string name;
    float price;
public:
    Product(string name=" ", float price=0) : name(name), price(price) {}

    friend istream &operator>>(istream &is, Product &product) {
        cout<<"Product name: ";
        is>>product.name;
        cout<<"Price: ";
        is>>product.price;
        return is;
    }

    float getPrice() const {
        return price;
    }

    const string &getName() const {
        return name;
    }

    friend ostream &operator<<(ostream &os, const Product &product) {
        os<<"Product name: "<<product.name<<", Price: "<<product.price;
        return os;
    }
};

class Order
{
protected:
    static int lastid;
    int uid;
    Product prod;
    int servingsNo;
    Date date;
public:

    Order(Product prod={" ", 0}, int servingsNo=0, Date date={0,0,0}) : uid(lastid++), prod(prod), servingsNo(servingsNo), date(date) {}
    virtual ~Order() {}

    virtual void del(){
        prod={};
        servingsNo=0;
        cout<<"Order canceled";
    }

    int getServingsNo() const;

    const Date &getDate() const;

    const Product &getProd() const;

    virtual Order operator + (int x) {
        servingsNo=servingsNo+x;
        return *this;
    }

    virtual Order operator ++ (int){
        servingsNo++;
        return *this;
    }

    virtual float OrderValue (){
        return prod.getPrice()*float(servingsNo);
    }

    virtual istream& read (istream &is) {
        cout<<"Unique ID: ";
        is>>uid;
        cout<<"Product: ";
        Product p;
        is>>p;
        cout<<"Number of servings: ";
        is>>servingsNo;
        cout<<"Date: ";
        is>>date.day>>date.month>>date.year;
        return is;
    }

    virtual ostream& write (ostream &os) {
        os<<" Unique ID: "<<uid;
        os<<", Product: "<<prod;
        os<<", Number of servings: "<<servingsNo;
        os<<", Date: "<<date.day<<" "<<date.month<<" "<<date.year;
        return os;
    }

    friend istream &operator>>(istream &is, Order &order) {
        order.read(is);
        return is;
    }
    friend ostream  &operator<<(ostream &os, Order &order) {
        order.write(os);
        return os;
    }
};

int Order::lastid = 1000;

const Date &Order::getDate() const {
    return date;
}

const Product &Order::getProd() const {
    return prod;
}

int Order::getServingsNo() const {
    return servingsNo;
}

class SpecialOrder : public Order{
protected:
    string obs;
    float surcharge;
public:
    SpecialOrder(Product prod={" ", 0}, int servingsNo=0, Date date={0,0,0}, string obs=" ", float surcharge=0)
            : Order(prod , servingsNo, date), obs(obs), surcharge(surcharge) {}

    float OrderValue() override {
        return Order::OrderValue()+surcharge;
    }

    istream &read (istream &is){
        Order::read(is);
        cout<<"Observations: ";
        is>>obs;
        cout<<"Surcharge: ";
        is>>surcharge;
        return is;
    }

    const string &getObs() const {
        return obs;
    }

    ostream &write (ostream &os) {
        Order::write(os);
        os<<", Observations: "<<obs<<", Surcharge: "<<surcharge;
        return os;
    }
};

class OnlineOrder : public Order {
private:
    string adress;
    int comission;
public:
    OnlineOrder(Product prod={" ", 0}, int servingsNo=0, Date date={0,0,0}, string adress=" ", int comission=5)
            : Order(prod , servingsNo, date), adress(adress), comission(comission) {}

    float OrderValue() override {
        return Order::OrderValue()+comission/100*Order::OrderValue();
    }

    istream &read (istream &is) {
        Order::read(is);
        cout<<"Shipping adress: ";
        is>>adress;
        cout<<"Comission: ";
        is>>comission;
        return is;
    }

    ostream &write (ostream &os){
        Order::write(os);
        os<<", Shipping adress: "<<adress<<", Comission: "<<comission;
        return os;
    }
};


class OnlineSpecialOrder : public SpecialOrder {
private:
    string adress;
    int comission;
public:
    OnlineSpecialOrder(Product prod={" ", 0}, int servingsNo=0, Date date={0,0,0}, string obs=" ", float surcharge=0, string adress=" ", int comission=0)
            : SpecialOrder(prod , servingsNo, date, obs, surcharge), adress(adress), comission(comission) {}

    float OrderValue() override {
        return SpecialOrder::OrderValue()+comission/100*Order::OrderValue();
    }

    istream &read (istream &is) {
        SpecialOrder::read(is);
        cout<<"Shipping adress: ";
        is>>adress;
        cout<<"Comission: ";
        is>>comission;
        return is;
    }

    ostream &write (ostream &os){
        SpecialOrder::write(os);
        os<<"Shipping adress: "<<adress<<"Comission: "<<comission;
        return os;
    }
};

class Waiter
{
private:
    string name;
    char gender;
    int age;
    int orderNo;
    vector<Order*> orders;

public:
    Waiter(string name=" ",vector<Order*> orders ={},  int orderNo=0, char gender=' ', int age=0) : name(name), gender(gender), age(age), orderNo(orderNo), orders(orders) {}

    virtual int operator > (Waiter w) {
        return orderNo>w.orderNo;
    }

    virtual int operator < (Waiter w) {
        return orderNo<w.orderNo;
    }

    const string &getName() const {
        return name;
    }

    const vector<Order *> &getOrders() const {
        return orders;
    }

    friend istream &operator>>(istream &is, Waiter &waiter) {
        cout<<"Waiter name: ";
        is>>waiter.name;
        cout<<"Gender: ";
        is>>waiter.gender;
        cout<<"Age: ";
        is>>waiter.age;
        cout<<"Number of orders: ";
        is>>waiter.orderNo;
        cout<<"Orders: ";
        for(int i=0;i<waiter.orderNo;i++){
            Order* o;
            cout<<"Order "<<i+1<<" :";
            is>>*o;
            waiter.orders.push_back(o);
        }
        return is;
    }

    friend ostream &operator<<(ostream &os, const Waiter &waiter) {
        os<<"Waiter name: "<<waiter.name<<", Gender: "<<waiter.gender<<", Age: "<<waiter.age<<", Number of orders: "<<waiter.orderNo<<", Orders: ";
        for(int i=0;i<waiter.orders.size();i++){
            os<<"\n"<<"Order "<<i+1<<" :";
            os<<*waiter.orders[i];
        }
        return os;
    }

};

class OrderManagement
{
private:
    vector<Order*> orders;
    vector<Waiter*> waiters;
public:
    OrderManagement(vector<Order*> orders={}, vector<Waiter*> waiters={}) : orders(orders), waiters(waiters) {}

    template<class Order>void addOrder(Order o) {
        Order* obj;
        obj=new Order;
        *obj=o;
        orders.push_back(obj);
    }

    void addWaiter(Waiter w) {
        Waiter* obj;
        obj=new Waiter;
        *obj=w;
        waiters.push_back(obj);
    }

    void DateSummary() {
        Date tempDate;
        tempDate.day=31;
        tempDate.month=5;
        tempDate.year=2016;
        for(int i=0;i<orders.size();i++)
            if(orders[i]->getDate().day==tempDate.day && orders[i]->getDate().month==tempDate.month && orders[i]->getDate().year==tempDate.year)
                cout<<*orders[i]<<" ";
    }

    void PapanasiSummary() {

        Date tempDate;
        tempDate.month=6;
        tempDate.year=2016;
        int c1=0, c2=0;
        for(int i=0;i<orders.size();i++)
            if(orders[i]->getDate().month==tempDate.month && orders[i]->getDate().year==tempDate.year)
            {
                if(orders[i]->getProd().getName()=="papanasi")
                {
                    SpecialOrder *as1 =dynamic_cast<SpecialOrder*>(orders[i]);
                    Order *as2 =dynamic_cast<Order*>(orders[i]);
                    OnlineOrder *as3 =dynamic_cast<OnlineOrder*>(orders[i]);
                    OnlineSpecialOrder *as4 =dynamic_cast<OnlineSpecialOrder*>(orders[i]);
                    if(as1!=NULL)
                    {if(as1->getObs()=="cu nuca de cocos")
                        {c2=c2+as1->getServingsNo();
                            c1=c1-as1->getServingsNo();}}
                    else if(as4!=NULL)
                        if(as4->getObs()=="cu nuca de cocos")
                        {c2=c2+as4->getServingsNo();
                            c1=c1-as4->getServingsNo();}
                    c1=c1+orders[i]->getServingsNo();
//                    if(as2!=NULL)
//                        c1=c1+as2->getServingsNo();
//                    if(as3!=NULL)
//                        c1=c1+as3->getServingsNo();
                }
            }
        cout<<"Numarul de portii de papanasi simpli este: "<<c1<<"\n";
        cout<<"Numarul de portii de papanasi cu nuca de cocos este: "<<c2<<"\n";
    }

    void OnlineOrderPercentage () {
        Date tempDate;
        tempDate.month=6;
        tempDate.year=2016;
        float s=0, so=0;
        for(int i=0;i<orders.size();i++)
            if(orders[i]->getDate().month==tempDate.month && orders[i]->getDate().year==tempDate.year)
            {
                SpecialOrder *as1 =dynamic_cast<SpecialOrder*>(orders[i]);
                Order *as2 =dynamic_cast<Order*>(orders[i]);
                OnlineOrder *as3 =dynamic_cast<OnlineOrder*>(orders[i]);
                OnlineSpecialOrder *as4 =dynamic_cast<OnlineSpecialOrder*>(orders[i]);
                if(as4!=NULL){
                    s=s+as4->OrderValue();
                    so=so+as4->OrderValue();
                }
                else if(as1!=NULL)
                    s=s+as1->OrderValue();
                else if(as3!=NULL){
                    s=s+as3->OrderValue();
                    so=so+as3->OrderValue();
                }
                else if(as2!=NULL)
                    s=s+as2->OrderValue();


            }
        float percentage;
        percentage=100*so/s;
        cout<<percentage;
    }

    void BestWaiter(){
        float maxv=0;
        int waiterNumber=0;
        for(int i=0;i<waiters.size();i++){
            float tempv=0;
            for (int j=0;j<waiters[i]->getOrders().size();j++)
                if(waiters[i]->getOrders()[j]->getDate().year>=2016)
                    tempv=tempv+waiters[i]->getOrders()[j]->OrderValue();
            if(tempv>maxv){
                maxv=tempv;
                waiterNumber=i;
            }

        }
        cout<<*waiters[waiterNumber];
    }
//    friend ostream &operator<<(ostream &os, const OrderManagement &om) {
//        int c=1;
//        for (auto i:om.waiters){
//            os << "Waiters " << c << ": ";
//            os << *i <<"\n";
//            c++;}
//        return os;
//    }

};

int main() {
///DEMO:
//    Product p1("frigarui", 30);
//    Product p2("cola", 7.5);
//    Product p3("cafea", 5);
//    Order c1(p1, 2, { 30,5,2016 }), c2(p2, 3), c3(p3, 1), c4 = c2, c5;
//
//    c3 = c3 + 4;
//    c2++;
//    c1.del();
//    cout << endl;
//    cin >> c5;
//    cout << c4 << endl << c5;
//
//    vector<Order*> com1, com2;
//
//    com1.push_back(&c1);
//    com1.push_back(&c2);
//    com1.push_back(&c3);
//    com2.push_back(&c4);
//    com2.push_back(&c5);
//    Waiter o1("Ionescu", com1, 3, 'M', 25), o2("Popescu", com2, 2, 'F', 30);
//    if (o2 > o1)
//        cout << "Ospatarul " << o2.getName() << " a servit mai multe comenzi decat ospatarul " << o1.getName() << endl;
//    else
//        cout << "Ospatarul " << o1.getName() << " a servit mai multe comenzi decat ospatarul " << o2.getName() << endl;

///FULL:

    Product p1{"papanasi", 15};
    Order o1{p1, 1, {31,6,2016}};
    SpecialOrder o2{p1, 2, {31,6,2016}, "cu nuca de cocos" ,5};
    OnlineOrder o3{p1, 5, {30,6,2016}, "craiova", 5};
    OnlineSpecialOrder o4{p1, 7, {31,6,2016}, "cu nuca de cocos" ,5, "craiova", 5};

    vector<Order*> com1, com2,com3;
    com1.push_back(&o1);
    com1.push_back(&o2);
    com1.push_back(&o3);
    com3.push_back(&o4);

    Waiter w1{"Bianca", {com1}, 2, 'F',21 };
    Waiter w2{"Dragos", {com2}, 3, 'M', 21};
    Waiter w3{"Corrado", {com3}, 1, 'M', 21};

    OrderManagement whatever;
    whatever.addOrder(o1);
    whatever.addOrder(o2);
    whatever.addOrder(o3);
    whatever.addOrder(o4);
    whatever.addWaiter(w1);
    whatever.addWaiter(w2);
    whatever.addWaiter(w3);

    whatever.DateSummary();
    cout<<"\n";
    whatever.PapanasiSummary();
    cout<<"\n";
    whatever.OnlineOrderPercentage();
    cout<<"\n \n";
    whatever.BestWaiter();
    cout<<"\n";
//    cout<<whatever;
}
