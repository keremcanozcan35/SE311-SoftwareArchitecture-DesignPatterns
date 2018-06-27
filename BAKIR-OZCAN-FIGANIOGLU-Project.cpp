//*********************//
/*
Project - A Classroom Allocation System
Bekir Batuhan BAKIR - 20140601006
Kerem Can OZCAN - 20140601039
Kerim FIGANIOGLU - 20140601026
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int classCount = 0;

class Secretary;
class Security;

class Department
{
public:
    Department(string name, Secretary* secretary, Security* security) : name(name), secretary(secretary), security(security){};
    string getName() const{return name;};
    Secretary* getSecretary(){return secretary;};
    Security* getSecurity(){return security;};
private:
    Secretary* secretary;
    Security* security;
    string name;
};

//Component
class Floor
{
public:
    virtual ~Floor(){};
    virtual void DisplayAndCount(){cout << name << " - " << depName << endl;};
    virtual void AllocateALL() {}
    virtual void EmptyAll(){}
    string getname() {return name;};
    virtual int getCount(){};
    virtual Floor* get(int){};
    bool getLocked(){return isLocked;};
    virtual void UnlockAll(){}
    virtual void LockAll(){}
    virtual void Unlock()
    {
        if(isLocked==true)
        {
             isLocked = false;
             cout << getname() << " is unlocked" << endl;
        }
        else
        {
            cout << getname() << " is already unlocked" << endl;
        }
    }
    virtual void Lock()
    {
        if(isLocked == false)
        {
            isLocked = true;
            cout << getname() << " is locked" << endl;
        }
        else
        {
            cout << getname() << " is already locked" << endl;
        }
    }
    void setDepartmentName(Department* dep){depName = dep->getName();};
    Floor(string name) : name(name){depName = "No Department"; isLocked = true;};
private:
    string name;
    string depName;
    bool isLocked;
};

//Abstract Iterator
class AbstractIterator {
public:
    virtual ~AbstractIterator(){};
	virtual void First() = 0;
	virtual void Next() = 0;
    virtual bool IsDone () const = 0;
	virtual Floor* CurrentItem() const = 0 ;
protected:
	AbstractIterator(){};
};

//Composite Iterator
class CompositeIterator : public AbstractIterator {
public:
	CompositeIterator(Floor *composite) :
		_composite(composite), _current(0) {
	}
	void First() {_current = 0;};
    void Next() {_current++;};
    Floor* CurrentItem() const {
		return (IsDone()?NULL:_composite->get(_current));
	};
	bool IsDone() const {
		return _current >= _composite->getCount();
	};
private:
	Floor *_composite;
	int _current;
};

//Subject
class Subject {
public:
	virtual ~Subject(){};
	Subject(string name) : name(name){};
	virtual void Attach (Secretary* secretary);
	virtual void Detach (Secretary* secretary);
	virtual string getName(){return name;};
	virtual void Notify();
	virtual bool getAvailable(){};
protected:
    bool isAvailable;
	vector<Secretary*> secretaries;
	string name;
};

//Leaf && Concrete Subject
class Classroom : public Floor, public Subject
{
public:
    Classroom(string name) : Floor(name), Subject(name){isAvailable = true;};
    void DisplayAndCount()
    {
        if(isAvailable==true)
        {
            cout << " " << getname() << endl;
            ++classCount;
        }
    }
    void AllocateALL() {
        AllocateClassroom();
    }
    bool getAvailable(){return isAvailable;};
    void AllocateClassroom()
    {
        if(isAvailable==false)
        {
            cout << getname() << " is already allocated for another exam" << endl;
        }
        else
        {
            isAvailable = false;
            Notify();
        }
    }
    void EmptyClass()
    {
        if(isAvailable==false)
        {
            isAvailable = true;
            Notify();
        }
        else
        {
            cout << this->getname() << " is already empty!" << endl;
        }
    }
    void EmptyAll()
    {
        if(isAvailable==false)
        {
            EmptyClass();
        }
    }
    void UnlockAll()
    {
        Unlock();
    }
    void LockAll()
    {
        Lock();
    }
private:
    bool isLocked;
    bool isAvailable;
};

//Composite
class Building : public Floor
{
public:
    Building(string name) : Floor(name){};
    Floor* get(int index){return elements[index];};
    int getCount(){return elements.size();};
    void Add(Floor* f){elements.push_back(f);};
    void DisplayAndCount() {
        cout <<  " " + getname() << endl;
		CompositeIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
            i->CurrentItem()->DisplayAndCount();
		delete i;
	}
    void AllocateALL() {
		CompositeIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
			i->CurrentItem()->AllocateALL();
		delete i;
	}
	void EmptyAll()
	{
        CompositeIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
			i->CurrentItem()->EmptyAll();
		delete i;
	}
	void UnlockAll()
	{
	    CompositeIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
			i->CurrentItem()->UnlockAll();
		delete i;
	}
    void LockAll()
	{
	    CompositeIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
			i->CurrentItem()->LockAll();
		delete i;
	}
    CompositeIterator* CreateIterator() {
		return new CompositeIterator(this);
    }
private:
    bool isLocked;
    vector<Floor*> elements;
};

//Composite
class Campus : public Floor
{
public:
    Campus(string name) : Floor(name){};
    Floor* get(int index){return buildings[index];};
    int getCount(){return buildings.size();};
    void Add(Floor* f){buildings.push_back(f);};
    void DisplayAndCount() {
        cout <<  " " + getname() << endl;
		CompositeIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
            i->CurrentItem()->DisplayAndCount();
        cout << "Number of available classes are: " << classCount << endl;
        classCount = 0;
		delete i;
	}
	void AllocateALL() {
		CompositeIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
			i->CurrentItem()->AllocateALL();
		delete i;
	}
	void EmptyAll()
	{
        CompositeIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
			i->CurrentItem()->EmptyAll();
		delete i;
	}
	void UnlockAll()
	{
	    CompositeIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
			i->CurrentItem()->UnlockAll();
		delete i;
	}
    void LockAll()
	{
	    CompositeIterator *i = CreateIterator();
		for (i->First(); !i->IsDone(); i->Next())
			i->CurrentItem()->LockAll();
		delete i;
	}
	CompositeIterator* CreateIterator() {
		return new CompositeIterator(this);
    }
private:
    vector<Floor*> buildings;
};

//Base Test
//Type 1 = RegularTest
//Type 2 = GeneralTest
class Test
{
public:
    Test(string name) : name(name){};
    virtual int getType(){return type;};
protected:
    int type;
private:
    string name;
};


class RegularTest : public Test
{
public:
    RegularTest(string name) : Test(name){type = 1;};
};

class GeneralTest : public Test
{
public:
    GeneralTest(string name) : Test(name){type = 2;};
};

//Abstract Command
class Command
{
public:
    virtual void Execute() = 0;
    virtual void UnExecute() = 0;
protected:
    Command(){};
};

// Abstract Observer
class Observer {
public:
	virtual ~Observer(){};
	virtual void Update(Subject* classroom) = 0;
};

//Receiver Base
class Staff
{
public:
    Staff(string name): name(name){};
    virtual void Action()
    {
        cout << "Default action" << endl;
    }
    virtual void ActionUnlock(Floor* f)
    {
        f->Unlock();
    }
    virtual void ActionLock(Floor* f)
    {
        f->Lock();
    }
    virtual string getName(){
        return name;
    }
    virtual void ActionUnlockALL(Floor* f)
    {
        f->UnlockAll();
    }
    virtual void ActionLockALL(Floor* f)
    {
        f->LockAll();
    }
private:
    string name;
};

//Receiver no1 && ConcreteObserver
class Secretary : public Staff, public Observer
{
public:
    Secretary(string name): Staff(name){};
    void ActionQuerry(Campus* b)
    {
        b->DisplayAndCount();
    }
    void ActionAllocate(Classroom* c)
    {
        if(c->getAvailable()==true)
        {
            c->AllocateClassroom();
        }
    }
    void ActionAllocateAll(Campus* b)
    {
        b->AllocateALL();
    }
    void ActionDeAllocate(Classroom* c)
    {
        c->EmptyClass();
    }
    void ActionDeAllocateAll(Campus* b)
    {
        b->EmptyAll();
    }
    void Update(Subject* classroom){
        cout<<"Classroom: "<<classroom->getName()<<"'s state has changed to: ";
        if(classroom->getAvailable()){
        cout<<"Available"<<endl;
        }
        else{
        cout<<"Not Available"<<endl;
        }
    }
private:
    Classroom* classroom;
};

//Receiver no2
class Security : public Staff
{
public:
    Security(string name): Staff(name){};
    void Action()
    {
        cout << "Security Action" << endl;
    }
};

void Subject::Attach(Secretary* secretary){
      secretaries.push_back(secretary);
    }

void Subject::Detach (Secretary* secretary){
        for (unsigned int i= 0; i< secretaries.size(); i++) {
            if (secretaries[i]->getName() == secretary->getName()) {
                secretaries.erase(secretaries.begin()+i);
                return;
            }
        }
	}

void Subject::Notify(){
        for (unsigned int i = 0; i < secretaries.size(); i++) {
            secretaries[i]->Update(this);
        }
	}

//Concrete Command
class QuerryCommand : public Command
{
public:
    QuerryCommand(Secretary* secretary, Campus* campus) : secretary(secretary), campus(campus){};
    void Execute()
    {
        secretary->ActionQuerry(campus);
    }
    void UnExecute(){cout<<"Can't undo a querry"<<endl;}
private:
    Secretary* secretary;
    Campus* campus;
};

//Concrete Command
class CreateTestCommand : public Command
{
public:
    CreateTestCommand(Secretary* secretary, Test* test, Campus* campus) : secretary(secretary), test(test), campus(campus){};
    CreateTestCommand(Secretary* secretary, Test* test, Classroom* classroom) : secretary(secretary), test(test), classroom(classroom){};
    void Execute()
    {
        if(test->getType()==1)
        {
            secretary->ActionAllocate(classroom);
        }
        else
        {
            secretary->ActionAllocateAll(campus);
        }
    }
    void UnExecute()
    {
        if(test->getType()==1)
        {
            secretary->ActionDeAllocate(classroom);
        }
        else
        {
            secretary->ActionDeAllocateAll(campus);
        }
    }
private:
    Secretary* secretary;
    Test* test;
    Campus* campus;
    Classroom* classroom;
};

//Concrete Command
class UnlockCommand : public Command
{
public:
    UnlockCommand(Staff* staff, Floor* lock) : staff(staff), lock(lock){};
    void Execute()
    {
        staff->ActionUnlock(lock);
    }
    void UnExecute()
    {
        staff->ActionLock(lock);
    }
private:
    Staff* staff;
    Floor* lock;
};

//Concrete Command
class UnlockAllCommand : public Command
{
public:
    UnlockAllCommand(Staff* staff, Floor* lock) : staff(staff), lock(lock){};
    void Execute()
    {
        staff->ActionUnlockALL(lock);
    }
    void UnExecute()
    {
        staff->ActionLockALL(lock);
    }
private:
    Staff* staff;
    Floor* lock;
};

//Invoker
class ResourceAllocationCenter
{
public:
    static ResourceAllocationCenter* getInstance()
    {
        if(instance==NULL)
        {
            instance = new ResourceAllocationCenter();
        }
        return instance;
    }
    void Compute(Command* command)
    {
        command->Execute();
        current++;
    }
    void Undo(Command* command)
    {
        command->UnExecute();
        --current;
    }
private:
    ResourceAllocationCenter() : current(0){};
    static ResourceAllocationCenter* instance;
    unsigned int current;
};

ResourceAllocationCenter* ResourceAllocationCenter::instance = 0;

int main()
{
    Campus* campus = new Campus("Balcova Campus");

    Building* tesla = new Building("Tesla Building");

    Floor* t1 = new Floor("Floor 1");

    Classroom* tc1 = new Classroom("Class 01");
    Classroom* tc2 = new Classroom("Class 02");
    Classroom* tc3= new Classroom("Class 03");
    Classroom* tc4 = new Classroom("Class 04");
    Classroom* tc5= new Classroom("Class 05");
    Classroom* tc6 = new Classroom("Class 06");

    campus->Add(tesla);

    tesla->Add(t1);

    tesla->Add(tc1);
    tesla->Add(tc2);
    tesla->Add(tc3);
    tesla->Add(tc4);
    tesla->Add(tc5);
    tesla->Add(tc6);

    Secretary* seSecretary = new Secretary("SE Secretary");
    Security* seSecurity = new Security("SE Security");
    Department* software = new Department("Software Engineering", seSecretary, seSecurity);

    t1->setDepartmentName(software);

    tc1->Attach(seSecretary);
    tc2->Attach(seSecretary);
    tc3->Attach(seSecretary);
    tc4->Attach(seSecretary);
    tc5->Attach(seSecretary);
    tc6->Attach(seSecretary);

    ResourceAllocationCenter* rac = ResourceAllocationCenter::getInstance();

    RegularTest* se311Test = new RegularTest("Se311");
    GeneralTest* turkishTest = new GeneralTest("Turkish");
    cout<<endl;
    cout<<"***************Creation of Regular Test***************"<<endl;
    cout<<endl;
    Command* queryCommand = new QuerryCommand(software->getSecretary(), campus);
    rac->Compute(queryCommand);

    Command* testCommand = new CreateTestCommand(software->getSecretary(), se311Test, tc1);
    rac->Compute(testCommand);

    Command* unlockCommand = new UnlockCommand(software->getSecurity(),tc1);
    rac->Compute(unlockCommand);

    rac->Compute(queryCommand);
    rac->Undo(testCommand);
    rac->Undo(unlockCommand);
    rac->Compute(queryCommand);

    cout<<endl;
    cout<<"***************Creation of General Test***************"<<endl;
    cout<<endl;
    testCommand= new CreateTestCommand(software->getSecretary(),turkishTest,campus);
    rac->Compute(testCommand);

    unlockCommand = new UnlockAllCommand(software->getSecurity(),campus);
    rac->Compute(unlockCommand);
    rac->Compute(queryCommand);
    rac->Undo(testCommand);
    rac->Undo(unlockCommand);
    rac->Compute(queryCommand);

    return 0;
}
