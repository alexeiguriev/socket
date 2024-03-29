#include <iostream>
#include <winsock2.h>
#include <vector>
#include <thread>
#include <functional>

typedef void (*func)(int value);
class OtherClass
{
    public:
        OtherClass()
        {
        }

        void SetFunc(func inFunc)
        {
            testFunc = inFunc;
        }

        void CallFunc(int value)
        {
            if (testFunc)
            {
                testFunc(value);
            }
        }

    private:
        func testFunc{NULL};
};

class MyClass
{
    public:
        MyClass(std::string data) :_data(data)
        {
            _otherClass.SetFunc(std::bind(&MyClass::Print, this, std::placeholders::_1));
            // _otherClass.SetFunc(std::bind(&MyClass::Print, this));
        }
        
        void MyClassCallFunc()
        {
            _otherClass.CallFunc(_value);
        }
        
    private:
        OtherClass _otherClass;
        std::string _data;
        int _value{10};

        void Print(int value)
        {
            std::cout << _data << " " << value << std::endl;
            // std::cout << "Data" << std::endl;
        }
};

int main()
{
    // OtherClass otherClass(Print);

    // otherClass.CallFunc("Some info");
    
    MyClass myClass("Some info");

    myClass.MyClassCallFunc();

    // std::cin.get();

    return 0;
}
