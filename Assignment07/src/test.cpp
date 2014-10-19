#include <iostream>
#include <functional>

using namespace std;

class A
{
	public:

		function<void()> testf;
		A() : testf(bind(&A::test, this)) { }
		virtual void test() { cout << "A" << endl; }
		virtual ~A() { cout << "DA" << endl; }
}; //A

class B : public A
{
	public:
		B() : A() { }
		virtual void test() { A::test(); cout << "B" << endl; }
		virtual ~B() { cout << "DB" << endl; }
}; //B

int main2(int argc, char** argv)
{
	B b;
	
	b.testf();

	return 0;
} //main
