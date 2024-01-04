#include <vector>
#include <algorithm>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <future>
#include <random>
#include <iostream>


std::vector<int> randNum(int range, int N) 
{
	std::vector<int> res;
	for (int It = 0; It < N; It++) 
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(0, range);
		res.push_back(dist(gen));
	}
	return res;
}

template <class T>
void print(const std::vector<T>& vec) 
{
	auto it = vec.begin();
	while (it != vec.end())
	{
		std::cout << *it << " ";
		++it;
	}
	std::cout << "\n";
}

template<typename It, typename func>
void for_each_par(It begin, It end, func&& f) 
{
	auto range = std::distance(begin, end);
	if (range > 1) 
	{
		auto l_begin = begin;
		auto l_end = begin + (range / 2);
		auto r_begin = begin + (range / 2) + 1;
		auto r_end = end;
		std::async(std::launch::async, [l_begin, l_end, &f]() 
			{
			for_each_par(l_begin, l_end, f);
			});

		std::async(std::launch::async, [r_begin, r_end, &f]() 
			{
			for_each_par(r_begin, r_end, f);
			});
	}
	else if (range == 1)
	{
		std::for_each(begin, end, f);
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russia");

	//создадим вектор случайных чисел
	std::vector<int> randVec = randNum(10, 30);
	std::cout << "Случайный вектор: \n";
	print(randVec);
	auto f_square = [](int& n) {n = n * n;};
	for_each_par(randVec.begin(), randVec.end(), f_square);
	std::cout << std::endl;
	std::cout << "Вектор в квадрате: \n";
	print(randVec);

	return 0;
}