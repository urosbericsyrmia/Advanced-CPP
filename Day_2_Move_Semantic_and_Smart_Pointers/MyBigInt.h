#ifndef _MY_BIG_INT_
#define _MY_BIG_INT_

#include <cstdint>
#include <iostream>

struct MyBigInt {
public:
	MyBigInt() = default;
	MyBigInt(const MyBigInt&);
	MyBigInt(MyBigInt&&);
	MyBigInt(long long int);
	MyBigInt(const char*);

	MyBigInt& operator=(const MyBigInt&);
	MyBigInt& operator=(MyBigInt&&);

	friend std::ostream& operator<<(std::ostream&, const MyBigInt&);

	bool operator==(const MyBigInt&);
	bool operator>(const MyBigInt&);
	bool operator<(const MyBigInt&);
	bool operator!=(const MyBigInt&);
	bool operator>=(const MyBigInt&);
	bool operator<=(const MyBigInt&);

	~MyBigInt();

	friend MyBigInt operator+(const MyBigInt& lhs, const MyBigInt& rhs);
private:
	int_least8_t* digits = nullptr;
	int size = 0;
};

MyBigInt operator""_mbi(const char* mbi);

#endif
