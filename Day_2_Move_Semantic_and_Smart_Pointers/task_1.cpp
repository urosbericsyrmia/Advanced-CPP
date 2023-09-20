#include "MyBigInt.h"

#include <iostream>
#include <cstring>

int count_digits(long long int number) {
    int counter = 0;
    do {
        counter += 1;
        number /= 10;
    } while (number != 0);
    return counter;
}

MyBigInt::MyBigInt(const MyBigInt& mbi) {
    this->size = mbi.size;
    this->digits = new int_least8_t[this->size];
    for (int i = 0; i < this->size; i++) {
        this->digits[i] = mbi.digits[i];
    }
}

MyBigInt::MyBigInt(MyBigInt&& mbi) {
    this->size = mbi.size;
    this->digits = mbi.digits;
    mbi.size = 0;
    mbi.digits = nullptr;
}

MyBigInt::MyBigInt(long long int mbi) {
    this->size = count_digits(mbi);
    this->digits = new int_least8_t[this->size];
    int idx = 0;
    do {
        this->digits[idx++] = mbi % 10;
        mbi /= 10;
    } while (mbi != 0);
}

MyBigInt::MyBigInt(const char* mbi) {
    this->size = strlen(mbi);
    this->digits = new int_least8_t[this->size];
    for (int i = 0; i < this->size; i++) {
        this->digits[i] = mbi[this->size - 1 - i] - '0';
    }
}

MyBigInt& MyBigInt::operator=(const MyBigInt& mbi) {
    this->size = mbi.size;
    this->digits = new int_least8_t[this->size];
    for (int i = 0; i < this->size; i++) {
        this->digits[i] = mbi.digits[i];
    }
    return *this;
}

MyBigInt& MyBigInt::operator=(MyBigInt&& mbi) {
    this->size = mbi.size;
    this->digits = mbi.digits;
    mbi.size = 0;
    mbi.digits = nullptr;
    return *this;
}

std::ostream & operator<<(std::ostream& os, const MyBigInt& mbi) {
    for (int i = mbi.size - 1; i >= 0; i--) {
        os << int(mbi.digits[i]);
    }
    return os;
}

bool MyBigInt::operator==(const MyBigInt& mbi) {
    if (this->size != mbi.size) {
        return false;
    }
    
    for (int i = 0; i < this->size; i++) {
        if (this->digits[i] != mbi.digits[i]) {
            return false;
        }
    }

    return true;
}

bool MyBigInt::operator>(const MyBigInt& mbi) {
    if (this->size > mbi.size) {
        return true;
    }

    if (this->size < mbi.size) {
        return false;
    }

    for (int i = this->size - 1; i >= 0; i--) {
        if (this->digits[i] > mbi.digits[i]) {
            return true;
        }

        if (this->digits[i] < mbi.digits[i]) {
            return false;
        }
    }

    return false;
}

bool MyBigInt::operator<(const MyBigInt& mbi) {
    if (this->size < mbi.size) {
        return true;
    }

    if (this->size > mbi.size) {
        return false;
    }

    for (int i = this->size - 1; i >= 0; i--) {
        if (this->digits[i] < mbi.digits[i]) {
            return true;
        }

        if (this->digits[i] > mbi.digits[i]) {
            return false;
        }
    }

    return false;
}

bool MyBigInt::operator!=(const MyBigInt& mbi) {
    return !(*this == mbi);
}

bool MyBigInt::operator>=(const MyBigInt& mbi) {
    return !(*this < mbi);
}

bool MyBigInt::operator<=(const MyBigInt& mbi) {
    return !(*this > mbi);
}

MyBigInt::~MyBigInt() {
    delete[] this->digits;
}

MyBigInt operator+(const MyBigInt& lhs, const MyBigInt& rhs)
{
	MyBigInt ret;
	const MyBigInt& longerArg = (lhs.size < rhs.size) ? rhs : lhs;
	const MyBigInt& shorterArg = (lhs.size < rhs.size) ? lhs : rhs;
	ret.digits = new int_least8_t[longerArg.size + 1];
	int_least8_t reminder = 0;
	for (int i = 0; i < shorterArg.size; i++) {
		int_least8_t tempSum = longerArg.digits[i] + shorterArg.digits[i] + reminder;
		ret.digits[i] = (tempSum > 9) ? tempSum - 10 : tempSum;
		reminder = (tempSum > 9) ? 1 : 0;
	}
	for (int i = shorterArg.size; i < longerArg.size; i++) {
		int_least8_t tempSum = longerArg.digits[i] + reminder;
		ret.digits[i] = (tempSum > 9) ? tempSum - 10 : tempSum;
		reminder = (tempSum > 9) ? 1 : 0;
	}
	if (reminder == 0) {
		ret.size = longerArg.size;
	} else {
		ret.size = longerArg.size + 1;
	}
	ret.digits[longerArg.size] = reminder;
	return ret;
}

MyBigInt operator""_mbi(const char* mbi) {
    return MyBigInt(mbi);
}

// Овде написати следећу функцију:
// MyBigInt fibo(int n);
MyBigInt fibo(int n) {
    if (n == 0) return 0_mbi;

    MyBigInt result[] = {0_mbi, 1_mbi};
    for (int i = 2; i <= n; i++) {
        MyBigInt temp = std::move(result[0]);
        result[0] = result[1];
        result[1] = result[1] + temp;
    }
    return result[1];
}

int main()
{
	MyBigInt k{123};
	std::cout << k << std::endl;
	k = fibo(9);
	std::cout << k << std::endl;
	k = k + 5;
	std::cout << k << std::endl;
	k = 7 + fibo(11);
	std::cout << k << std::endl;
	k = 13_mbi;
	std::cout << k << std::endl;
	MyBigInt l = fibo(300);
	if (l != 222232244629420445529739893461909967206666939096499764990979600_mbi) {
		std::cout << "Greska!" << std::endl;
		return 1;
	}
	std::cout << l << std::endl;
	{
		MyBigInt m = l;
		std::cout << m << std::endl;
	}
	std::cout << l << std::endl;
	{
		MyBigInt n = std::move(l);
		std::cout << n << std::endl;
	}
	return 0;
}


