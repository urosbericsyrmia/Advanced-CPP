#include <iostream>

// Овде написати следећу функцију:
// int fibo(int n)

constexpr int fibo(int n) {
	if (n == 0) return 0;

	int result[] = {0, 1};
	for (int i = 2; i <= n; i++) {
		int temp = std::move(result[0]);
		result[0] = result[1];
		result[1] += std::move(temp);
	}
	return result[1];
}

// constexpr int fibo(int n) {
//     if (n <= 1) return n;
//     return fibo(n - 1) + fibo(n - 2);
// }

int main()
{
	static_assert(fibo(7) == 13, "Greska!");
	const int k = fibo(9);
	std::cout << k << std::endl;
	// constexpr int l = fibo(300); // 300. Фибоначијев број је превелик за int
}