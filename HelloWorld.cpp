#include "Add.h"

#include <iostream>
#include <string>
#include <memory>

class Person
{
public:
	int age{};
};

// これは警告になってエラー
// ローカル変数は破棄されるからそのポインタはdangling になってる
//Person* getPersonPointer() {
//	Person person{ 5 };
//	Person* ptr{ &person };
//	return ptr;
//}

// これはOK
Person* getPersonPointer() {
	auto ptr{ new Person {5} };
	return ptr;
}

std::unique_ptr<Person> getPersonUniquePointer() {
	return std::make_unique<Person>(5);
}

int main()
{
	Person p{ 5 };
	auto ptr{ getPersonPointer() };
	auto u_ptr{ getPersonPointer() };
	const Person& x{ *ptr };
	const Person& y{ *u_ptr };

	std::cout << p.age << '\n';
	std::cout << ptr->age << '\n';
	std::cout << u_ptr->age << '\n';
	std::cout << x.age << '\n';
	std::cout << y.age << '\n';

	return 0;
}

