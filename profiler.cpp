#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "rbset.h"
#include "avlset.h"
#include <fstream>
#include <cmath>
using namespace std;

#define TEST_SIZE (1 << 22)
#define PROFILE_SIZE (1 << 25)

template<template<class T> class testset>
void test_mod(int base) {
	testset<int> set_object;

	int CHECK_SIZE = TEST_SIZE;
	for (int i = 0; i < CHECK_SIZE; i++) {
		if (i % base == 0) {
			set_object.insert(i);
		}
	}
	for (int i = CHECK_SIZE - 1; i >= 0; i--) {
		bool in_set = set_object.find(i);
		assert(in_set == (i % base == 0));
	}
}

template<template<class T> class testset>
void test_random() {
	testset<int> set_object;
	set<int> set_checker;

	int CHECK_SIZE = TEST_SIZE;
	for (int i = 0; i < CHECK_SIZE; i++) {
		if (rand() % 2 == 0) {
			set_object.insert(i);
			set_checker.insert(i);
		}
	}

	for (int i = CHECK_SIZE - 1; i >= 0; i--) {
		assert(set_object.find(i) == (set_checker.find(i) != set_checker.end()));
	}
}

template<template<class T> class testset>
void test_erase_random() {
	testset<int> set_object;
	set<int> set_checker;

	int CHECK_SIZE = TEST_SIZE;
	for (int i = 0; i < CHECK_SIZE; i++) {
		if (rand() % 2 == 0) {
			set_object.insert(i);
			set_checker.insert(i);
		}
	}

	set_object.erase(CHECK_SIZE);

	for (int x: set_checker) {
		assert(set_object.find(x));
		set_object.erase(x);
		assert(!set_object.find(x));
	}

}


template<template<class T> class testset>
void profile() {
	string setname = testset<int>().name();
	ofstream fout(setname + ".data");

	vector<clock_t> insert_times(PROFILE_SIZE);
	vector<clock_t> find_times(PROFILE_SIZE);
	vector<clock_t> erase_times(PROFILE_SIZE);

	testset<int> set_object;

	time_t begin = clock();
	for (int i = 0; i < PROFILE_SIZE; i++) {
		set_object.insert(i);
		insert_times[i] = clock() - begin;
	}

	cout << "insertion profiling done" << endl;

	testset<int> set_object_find;
	for (int i = 0; i < PROFILE_SIZE; i++) {
		set_object_find.insert(i);
		begin = clock();
		set_object_find.find(i);
		find_times[i] = clock() - begin + (i == 0 ? 0:find_times[i - 1]);
	}

	cout << "find profiling done" << endl;

	testset<int> set_object_erase;
	for (int i = 0; i < PROFILE_SIZE; i++) {
		set_object_erase.insert(i);
		begin = clock();
		set_object_erase.erase(i);
		erase_times[i] = clock() - begin + (i == 0 ? 0:erase_times[i - 1]);
		set_object_erase.insert(i);
	}

	cout << "erase profiling done" << endl;

	for (int i = 1; i < PROFILE_SIZE; i += PROFILE_SIZE / 100000) {
		fout << log2(i) << " ";
		fout << ((double)insert_times[i]) / i << " ";
		fout << ((double)erase_times[i]) / i << " ";
		fout << ((double)find_times[i]) / i << "\n";
	}

}

template<template<class T> class testset>
bool test() {
	string setname = testset<int>().name();

	cout << "TESTING " << setname << " on 0x" << hex << TEST_SIZE << " elements" << dec << endl;
	for (int i = 1; i < 10; i++) {
		test_mod<testset>(i);
	}
	cout << "test_mod \033[1;32mOK\033[0m" << endl;

	test_random<testset>();
	cout << "test_random \033[1;32mOK\033[0m" << endl;

	test_erase_random<testset>();
	cout << "test_erase_random \033[1;32mOK\033[0m" << endl;

	cout << "all tests are \033[1;32mOK\033[0m" << endl;

	cout << endl;

	cout << "PROFILING " << setname << " on 0x" << hex << PROFILE_SIZE << " elements" << dec << endl;
	profile<testset>();
	cout << "profiling is \033[1;32mdone\033[0m" << endl;
	cout << "there is a file " << setname << ".data in this folder." << endl;
	cout << "you can analyse data in it using gnuplot" << endl;
	cout << "just run gnuplot " << setname << ".plot" << endl;
	cout << endl;
}

int main() {
	std::ios::sync_with_stdio(false);
	test<rbset>();
	test<avlset>();
}