#include <iostream>

//nonreversive version remains  bug to fix

void swap(int array[], int low, int high)
{
	int temp;
	temp = array[low];
	array[low] = array[high];
	array[high] = temp;
}

int    partition(int array[], int low, int high)
{
	int    p;
	p = array[low];
	while (low < high) {
		while (low < high && array[high] >= p)
			high--;
		swap(array,low,high);
		while (low < high && array[low] <= p)
			low++;
		swap(array,low,high);
	}
	return low;
}

void       qsort_nonrecursive(int array[], int low, int high)
{
	int m[50], n[50], cp, p;
	/* 初始化栈和栈顶指针 */
	cp = 0;
	m[0] = low;
	n[0] = high;
	while (m[cp] < n[cp]) {
		while (m[cp] < n[cp]) {       /* 向左走到尽头 */
			p = partition(array, m[cp], n[cp]); /* 对当前结点的访问 */
			cp++;
			m[cp] = m[cp - 1];
			n[cp] = p - 1;
		}
		/* 向右走一步 */
		m[cp + 1] = n[cp] + 2;
		n[cp + 1] = n[cp - 1];
		cp++;
	}
}

void qsort_recursive(int array[], int low, int high)
{
	if(low >= high) return;
	int pos = partition(array,low,high);
	qsort_nonrecursive(array,low,pos);
	qsort_nonrecursive(array,pos+1,high);
	return;
}

int main()
{
	int iArr[6] = {11,29,3,48,5,1};
	qsort_recursive(iArr,0,5);
	for(int i=0;i<6;i++)
		std::cout<<iArr[i]<<std::endl;
	return 0;
}
