#include<iostream>
#include<vector>

using namespace std;
//二路快排
int pivot(vector<int>& nums, int start, int end) {
	int n = nums[end];
	int index = start;
	for (int i = start; i < end;) {
		if (nums[i] <= n) {
			swap(nums[i], nums[index]);
			index++;
			i++;
		}
		else {
			i++;
		}
	}
	swap(nums[index], nums[end]);
	return index;
}

void quickSort(vector<int>& nums, int start, int end) {
	//cout << "start:" << start << " end:" << end << endl;
	if (start < end) {
		int pos = pivot(nums, start, end);
		//cout << pos << endl;
		quickSort(nums,start, pos - 1);
		quickSort(nums, pos + 1, end);
	}
}

//三路快排
pair<int, int> pivot1(vector<int>& nums, int start, int end) {
	int num = nums[start];
	int i = start, j = start;
	int k = end;
	while (i <= k) {
		if (nums[i] < num) {
			swap(nums[i++], nums[j++]);
		}
		else if (nums[i] > num) {
			swap(nums[i], nums[k--]);
		}
		else {
			i++;
		}
	}
	return { j, k };
}

void quickSort1(vector<int>& nums, int start, int end) {
	if (start < end) {
		auto range = pivot1(nums, start, end);
		quickSort1(nums, start, range.first - 1);
		quickSort1(nums, range.second + 1, end);
	}
}
int main() {
	vector<int> nums{ 3,5,1,7,6,7,10,9 };
	quickSort1(nums, 0, nums.size() - 1);
	for (int i : nums) {
		cout << i << " ";
	}
}