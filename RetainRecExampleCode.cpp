// Example program
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class Base
{
    private:
    string name;
    int* privNum; //not to be displayed
        
    protected:
    string nationality;
    
    public:
        
    Base(std::string pname, std::string pnationality, int pprivNum):name(pname), nationality(pnationality)
    {
        privNum = new int(pprivNum);
        cout<<"Base parametrized constructor"<<endl;
    }
    
    Base():Base("anonymous", "none", 0)
    {
        cout<<"Base constructor"<<endl;
    }
    
    Base( const Base &obj ) //copy constructor
    {
       cout<<"copy constructor"<<endl;
       //Deep copy
       privNum = new int(*(obj.privNum));
       string name = obj.name;
       nationality = obj.nationality;
       
    }
    
    Base& operator=(const Base &obj)
    {
        cout<<"Assignment operator overloading"<<endl;
        // self-assignment guard
        if (this == &obj)
           return *this;
           
        name = obj.name;
        nationality = obj.nationality;
        if (privNum!=NULL)
           delete privNum;
        privNum = new int(*(obj.privNum));
        return *this;
    }
    
    //virtual function - dynamic binding, polymorphism
    virtual void display() const
    {
        cout<<"Base Info virtual Display"<<endl;
        cout<<"Name:"<<(*this).name<<"  "<<"Nationality:"<<(*this).nationality<<endl;
    }
    
    //overloaded function - static binding, polymorphism
    void display(const Base &obj)
    {
        cout<<"Base class: overloaded fn Info display"<<endl;
        cout<<"Name:"<<obj.name<<" "<<"Nationality:"<<obj.nationality<<endl;        
    }
    
    ~Base()
    {
        cout<<"Base destructor"<<endl;
        delete privNum;
    }
};

class Derived1: public Base
{
    protected:
    int age;
    
    public:
    
    Derived1(string name, string nationality, int privNum, int paramage): Base(name, nationality, privNum),age(paramage)
    {
        cout<<"Derived1 parametrized constructor"<<endl;
    }
    
    Derived1(): age(0)
    {
        cout<<"Derived1 contructor"<<endl;
    }
    
    virtual void display() const
    {
        cout<<"Derived1 display"<<endl;
        this->Base::display();
        cout<<"Age:"<<this->age<<endl;
        return;
    }
    
    ~Derived1()
    {
        cout<<"Derived1 destructor"<<endl;
    }
};

//Interface with APIs
class Citizen
{
    public:
    // pure virtual funtions
    virtual void votingEligibilty() = 0;
    virtual void seniorCitizenConcession() = 0;
};

class Employer
{
    private:
       string employer;
       
    public:
       Employer()
       {
           cout<<"Employer constructor"<<endl;
           employer = "None";
       }
       
       Employer(const string paramEmpl)
       {
           cout<<"Employer param constructor"<<endl;
           employer = paramEmpl;
       }
       
       //mutator fn
       void setEmployer(const string pemployer)
       {
           employer = pemployer;
       }
       
       //inspector fn
       const string getEmployer() const
       {
           return this->employer;
       }
       
       ~Employer()
       {
           cout<<"Employer destructor"<<endl;
       }
};

class MultiDerived: public Derived1, public Citizen
{
    private:
    bool taxPayer;
    Employer obj;
    
    public:
    MultiDerived(string name, string nationality, int privNum, int age, bool ptaxPayer): Derived1(name, nationality, privNum, age), taxPayer(ptaxPayer)
    {
        cout<<"Parametrized Multizderived constructor"<<endl;
        obj.setEmployer("RetainRec");
    }
    
    MultiDerived(): taxPayer(false)
    {
        cout<<"Multizderived constructor"<<endl;
        obj.setEmployer("RetainRec");
    }
    
    virtual void votingEligibilty()
    {
        cout<<"Voting Eligibity"<<endl;
        if (this->Derived1::age > 18)
            cout<<"Citizen eligible to vote"<<endl;
        else
            cout<<"Citizen not eligible to vote"<<endl;
    }
    
    virtual void seniorCitizenConcession()
    {
        cout<<"Senior Citizen Consession applicable:";
        if (this->Derived1::age > 60)
           cout<<"Yes"<<endl;
        else
           cout<<"No"<<endl;
    }
    
    virtual void display()  const
    {
        cout<<"MultiDerived Info Display"<<endl;
        // cannot directly print name, nationality from Base class as class members(private & protected) become private in inherited class
        // inhertited protected fn() call
        this->Derived1::display();  
        cout<<"Employer:"<<this->obj.getEmployer()<<endl;
        cout<<"Taxpayer:"<<this->taxPayer<<endl;
    }
        
    ~MultiDerived()
    {
        cout<<"MultiDerived destructor"<<endl;
    }
};

int main()
{
    Base a("Bob", "NewZealand", 1231); 
    a.display(); //base class display fn is called
    cout<<"====================================="<<endl;
    
    Base *basePtr = new Base(); // calls assignment constructor
    cout<<"===================================="<<endl;   
    
    Base b(a); //calls copy constructor
    cout<<"==================================="<<endl;
        
    //dereferncing a pointer
    Base& ref = *basePtr;
    ref = a;
    cout<<"===================================="<<endl;
       
    Derived1 derivedObj("Ethan", "Australian", 7776, 35);
    derivedObj.display(); //virtual fn() for Derived class is called
    cout<<"===================================="<<endl;
        
    Base* ptr = new MultiDerived("Alice", "Australian", 1232, 30, true);
    cout<<"===================================="<<endl;
    ptr->display();
    cout<<"======================================="<<endl;
   //inherited overloaded Base fn display is called
    ptr->display(ref);
    cout<<"========================================"<<endl;
    // child object pointed by parent pointer can be casted to child pointer & is safe
    // but parent objecct pointed by parent pointer should not be casted to child pointer & is unsafe
    MultiDerived* pd = dynamic_cast<MultiDerived*>(ptr);
    cout<<"======================================="<<endl;
    ptr = NULL; // to handle dangling ptr issue
    pd->seniorCitizenConcession();
    cout<<"======================================="<<endl;
    
    try
    {
        // Parent object cannot be assigned to child pointer
        int div = 0;
        int result = 17/div;
        if (div == 0)
          throw overflow_error("Division By Zero");
    }
    catch(overflow_error)
    {
        cout<< "Change Div or denominator"<<endl;
    }
    
    //  Free up dynamically allocated memory
    delete basePtr;
    delete pd;
}
