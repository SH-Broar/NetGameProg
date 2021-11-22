#include <iostream>
using namespace std;

int main() {
	int a[10] = { 1,2,3,4,5,6,7,8,9,0 };

	int* p = &a[0];

	int* p2 = p;


	cout << p2[0] << endl;


}