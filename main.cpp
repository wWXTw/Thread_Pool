#include "threadPool.h"
#include <iostream>

using namespace std;

int print(int a) {
	a *= 10;
	cout << this_thread::get_id() << "-" << a << endl;
	return a;
}

int main() {
	ThreadPool tp(5);
	for (int i = 0; i < 20; i++) {
		auto res = tp.addTask(print, i);
		cout << res.get() << endl;
	}
	return 0;
}