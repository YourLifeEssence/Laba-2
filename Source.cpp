#include "Sorting.h"
int main() {
	std::vector<int> myVec{ 55,132,72,2, 8, 13 };
	//std::function<bool(const int&, const int&)> myComp = [](const int& a, const int& b) { return a < b; };
	auto myComp = [](const int& a, const int& b) {return a < b; };

	Sorting<int>* sorting = new RadixSorting<int>();
	sorting->sort(myVec, myComp);

}