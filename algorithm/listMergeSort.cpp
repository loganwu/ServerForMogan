#include <iostream>
#include <list>
using namespace std;
typedef list<int> IList;

void print(const IList& list)
{

	IList::const_iterator ite = list.begin();
	for (; ite != list.end(); ++ite)
	{
		cout << *ite << "  ";
	}
	cout << endl;

}

int main()
{
	IList s;

//	s.push_back(7);
//	s.push_back(6);
//	s.push_back(5);
	s.push_back(4);
	s.push_back(3);
	s.push_back(2);
	s.push_back(1);
	s.push_back(0);

	IList carry;
	IList counter[64];
	int fill = 0;
	int num = 0;
	while (!s.empty())
	{
		cout << "取第" << num << "个数据: fill = " << fill << endl;
		carry.splice(carry.begin(), s, s.begin());
		int i = 0;
		while (i < fill && !counter[i].empty())
		{
			counter[i].merge(carry);
			carry.swap(counter[i++]);
		}
		carry.swap(counter[i]);
		if (i == fill)
			++fill;
			//我自己加的计数
			num++;

			//打印每次完的结果
			for (int i = 0; i < fill; ++i)
			{
				cout << i << "==";
				print(counter[i]);
			}
	}
	cout<<"while over"<<endl;
	for (int i = 1; i < fill; ++i)
		counter[i].merge(counter[i - 1]);
		s.swap(counter[fill - 1]);

		getchar();
		return 0;
}
