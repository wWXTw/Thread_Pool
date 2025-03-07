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
	vector<future<int>> fv(100);
	fv[0] = tp.addTask(print, TaskPriority::Low, 1);
	fv[1] = tp.addTask(print, TaskPriority::Low, 2);
	fv[2] = tp.addTask(print, TaskPriority::High, 3);
	fv[3] = tp.addTask(print, TaskPriority::Low, 4);
	fv[4] = tp.addTask(print, TaskPriority::High, 5);
	fv[5] = tp.addTask(print, TaskPriority::Medium, 6);
	fv[6] = tp.addTask(print, TaskPriority::High, 7);
	fv[7] = tp.addTask(print, TaskPriority::Medium, 8);
	fv[8] = tp.addTask(print, TaskPriority::Low, 9);
	fv[9] = tp.addTask(print, TaskPriority::Medium, 10);
	fv[10] = tp.addTask(print, TaskPriority::Low, 1);
	fv[11] = tp.addTask(print, TaskPriority::Low, 2);
	fv[12] = tp.addTask(print, TaskPriority::High, 3);
	fv[13] = tp.addTask(print, TaskPriority::Low, 4);
	fv[14] = tp.addTask(print, TaskPriority::High, 5);
	fv[15] = tp.addTask(print, TaskPriority::Medium, 6);
	fv[16] = tp.addTask(print, TaskPriority::High, 7);
	fv[17] = tp.addTask(print, TaskPriority::Medium, 8);
	fv[18] = tp.addTask(print, TaskPriority::Low, 9);
	fv[19] = tp.addTask(print, TaskPriority::Medium, 10);
	fv[20] = tp.addTask(print, TaskPriority::Low, 1);
	fv[21] = tp.addTask(print, TaskPriority::Low, 2);
	fv[22] = tp.addTask(print, TaskPriority::High, 3);
	fv[23] = tp.addTask(print, TaskPriority::Low, 4);
	fv[24] = tp.addTask(print, TaskPriority::High, 5);
	fv[25] = tp.addTask(print, TaskPriority::Medium, 6);
	fv[26] = tp.addTask(print, TaskPriority::High, 7);
	fv[27] = tp.addTask(print, TaskPriority::Medium, 8);
	fv[28] = tp.addTask(print, TaskPriority::Low, 9);
	fv[29] = tp.addTask(print, TaskPriority::Medium, 10);
	fv[30] = tp.addTask(print, TaskPriority::Low, 1);
	fv[31] = tp.addTask(print, TaskPriority::Low, 2);
	fv[32] = tp.addTask(print, TaskPriority::High, 3);
	fv[33] = tp.addTask(print, TaskPriority::Low, 4);
	fv[34] = tp.addTask(print, TaskPriority::High, 5);
	fv[35] = tp.addTask(print, TaskPriority::Medium, 6);
	fv[36] = tp.addTask(print, TaskPriority::High, 7);
	fv[37] = tp.addTask(print, TaskPriority::Medium, 8);
	fv[38] = tp.addTask(print, TaskPriority::Low, 9);
	fv[39] = tp.addTask(print, TaskPriority::Medium, 10);
	fv[40] = tp.addTask(print, TaskPriority::Low, 1);
	fv[41] = tp.addTask(print, TaskPriority::Low, 2);
	fv[42] = tp.addTask(print, TaskPriority::High, 3);
	fv[43] = tp.addTask(print, TaskPriority::Low, 4);
	fv[44] = tp.addTask(print, TaskPriority::High, 5);
	fv[45] = tp.addTask(print, TaskPriority::Medium, 6);
	fv[46] = tp.addTask(print, TaskPriority::High, 7);
	fv[47] = tp.addTask(print, TaskPriority::Medium, 8);
	fv[48] = tp.addTask(print, TaskPriority::Low, 9);
	fv[49] = tp.addTask(print, TaskPriority::Medium, 10);
	fv[50] = tp.addTask(print, TaskPriority::Low, 1);
	fv[51] = tp.addTask(print, TaskPriority::Low, 2);
	fv[52] = tp.addTask(print, TaskPriority::High, 3);
	fv[53] = tp.addTask(print, TaskPriority::Low, 4);
	fv[54] = tp.addTask(print, TaskPriority::High, 5);
	fv[55] = tp.addTask(print, TaskPriority::Medium, 6);
	fv[56] = tp.addTask(print, TaskPriority::High, 7);
	fv[57] = tp.addTask(print, TaskPriority::Medium, 8);
	fv[58] = tp.addTask(print, TaskPriority::Low, 9);
	fv[59] = tp.addTask(print, TaskPriority::Medium, 10);
	fv[60] = tp.addTask(print, TaskPriority::Low, 1);
	fv[61] = tp.addTask(print, TaskPriority::Low, 2);
	fv[62] = tp.addTask(print, TaskPriority::High, 3);
	fv[63] = tp.addTask(print, TaskPriority::Low, 4);
	fv[64] = tp.addTask(print, TaskPriority::High, 5);
	fv[65] = tp.addTask(print, TaskPriority::Medium, 6);
	fv[66] = tp.addTask(print, TaskPriority::High, 7);
	fv[67] = tp.addTask(print, TaskPriority::Medium, 8);
	fv[68] = tp.addTask(print, TaskPriority::Low, 9);
	fv[69] = tp.addTask(print, TaskPriority::Medium, 10);
	fv[70] = tp.addTask(print, TaskPriority::Low, 1);
	fv[71] = tp.addTask(print, TaskPriority::Low, 2);
	fv[72] = tp.addTask(print, TaskPriority::High, 3);
	fv[73] = tp.addTask(print, TaskPriority::Low, 4);
	fv[74] = tp.addTask(print, TaskPriority::High, 5);
	fv[75] = tp.addTask(print, TaskPriority::Medium, 6);
	fv[76] = tp.addTask(print, TaskPriority::High, 7);
	fv[77] = tp.addTask(print, TaskPriority::Medium, 8);
	fv[78] = tp.addTask(print, TaskPriority::Low, 9);
	fv[79] = tp.addTask(print, TaskPriority::Medium, 10);
	fv[80] = tp.addTask(print, TaskPriority::Low, 1);
	fv[81] = tp.addTask(print, TaskPriority::Low, 2);
	fv[82] = tp.addTask(print, TaskPriority::High, 3);
	fv[83] = tp.addTask(print, TaskPriority::Low, 4);
	fv[84] = tp.addTask(print, TaskPriority::High, 5);
	fv[85] = tp.addTask(print, TaskPriority::Medium, 6);
	fv[86] = tp.addTask(print, TaskPriority::High, 7);
	fv[87] = tp.addTask(print, TaskPriority::Medium, 8);
	fv[88] = tp.addTask(print, TaskPriority::Low, 9);
	fv[89] = tp.addTask(print, TaskPriority::Medium, 10);
	fv[90] = tp.addTask(print, TaskPriority::Low, 1);
	fv[91] = tp.addTask(print, TaskPriority::Low, 2);
	fv[92] = tp.addTask(print, TaskPriority::High, 3);
	fv[93] = tp.addTask(print, TaskPriority::Low, 4);
	fv[94] = tp.addTask(print, TaskPriority::High, 5);
	fv[95] = tp.addTask(print, TaskPriority::Medium, 6);
	fv[96] = tp.addTask(print, TaskPriority::High, 7);
	fv[97] = tp.addTask(print, TaskPriority::Medium, 8);
	fv[98] = tp.addTask(print, TaskPriority::Low, 9);
	fv[99] = tp.addTask(print, TaskPriority::Medium, 10);

	// 超时控制
	for (auto& f : fv) {
		if (f.wait_for(std::chrono::seconds(60)) == std::future_status::timeout) {
			throw std::runtime_error("连接超时");
		}
	}

	return 0;
}