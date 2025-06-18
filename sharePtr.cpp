//
// Created by Rambo.Liu on 2025/4/16.
//
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>

template<typename T>
class SharePtr {
public:

    SharePtr(T *_ptr) {
        printf("11---------智能指针构造\n");
        count = 1;
        ptr = _ptr;
    }

    ~SharePtr() {
        //如果引用计数为0 将指针指向的对象释放
        count--;
        printf("11---------智能指针析构\n");
        if (count == 0) {
            delete ptr;
            ptr = nullptr;
        }
    }

    //拷贝构造
    SharePtr(const SharePtr &sharePtr) {
        if (sharePtr.ptr != ptr) {
            count++;
            ptr = sharePtr.ptr;
        }
    }

    //拷贝赋值
    SharePtr &operator=(const SharePtr &sharePtr) {
        if (sharePtr.ptr != ptr) {
            ptr = sharePtr.ptr;
            count++;
        }
    }

private:
    int count;
    //指向对象的指针
    T *ptr;
};

using namespace std;


class A {
public:
    A() {
        printf("49--------A 构造\n");
    }

    ~A() {
        printf("51--------A 析构\n");
    }
};

void test() {
    A *a = new A();
    SharePtr<A> a_sp(a);


}

void fun(char **p) {
    *p = (char *) malloc(100);
}

void bubbleSort(int *a, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (*(a + j) > *(a + j + 1)) {
                int tmp = *(a + j);
                *(a + j) = *(a + j + 1);
                *(a + j + 1) = tmp;
            }
        }

    }
    printf("排序之后的顺序为：\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", a[i]);
    }
}

/**
 * int a[8] = {3,2,4,1,7,8,5,6}; 目标增序
 * 排序过程：
 * 1. low = 0, high = 7;
 * a[low] > a[high],交换 将a[low] 与 a[high]交换，此时 变成
 *  https://cloud.tencent.com/developer/article/2103396
 * @param a
 * @param low
 * @param high
 */

void quickSort(int *a, int low, int high) {
    if (low >= high)
        return;
    int i = low;
    int j = high;
    int base = a[low];
    while (i < j) {
        while (a[j] >= base && i < j)
            j--;
        while (a[i] <= base && i < j)
            i++;
        if (i < j) {
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }

    //基准数归位
    a[low] = a[i];
    a[i] = base;

    //递归遍历左边
    quickSort(a, low, i - 1);
    //递归遍历右半部分
    quickSort(a, i + 1, high);
}

std::stack<int> s1;
std::stack<int> s2;

//使用2个栈模拟队列
void pushQueue(int e) {
    s1.push(e);

}

void popQueue() {
    if (s2.empty()) {
        while (!s1.empty()) {
            s2.push(s1.top());
            s1.pop();
        }
        int top = s2.top();
        printf("153 ------出队元素 = %d\n", top);
        s2.pop();
        return;
    }
    int top = s2.top();
    printf("158 ------出队元素 = %d\n", top);
    s2.pop();
}

//2个队列模拟一个栈
std::queue<int> q1;
std::queue<int> q2;

void pushStack(int e) {
    printf("入栈元素：e = %d\n", e);
    if (q1.size() > 0) {
        q1.push(e);
        return;
    }
    if (q2.size() > 0) {
        q2.push(e);
        return;
    }

    if (q1.size() == 0 && q2.size() == 0) {
        q1.push(e);
        return;
    }
}

void stackPop() {
    //先判断其中一个队列中的元素是否为空，如果不为空，将所有元素存入另一个队列中，并将末尾元素弹出，
    //达到模拟出栈的效果
    if (q1.size() > 0) {
        int head = -1;
        while (q1.size() != 0) {
            head = q1.front();
            q1.pop();
            if (q1.size() >= 1) {
                q2.push(head);
            }

        }
        printf("177-----出栈元素 = %d\n", head);
        return;
    }

    if (q2.size() > 0) {
        int head = -1;
        while (q2.size() != 0) {
            head = q2.front();
            q2.pop();
            if (q2.size() >= 1) {
                q1.push(head);
            }
        }
        printf("188-----出栈元素 = %d\n", head);
        return;
    }
}


//算法：一个连续子数组最大和：如[1，-2，3，4，-2，5] 结果为10

class Solution {
public:
    int maxSubArray(std::vector<int> &nums) {
        int sum = 0;
        int max_val = INT32_MIN;
        for (int i = 0; i < nums.size(); i++) {
            sum += nums[i];
            if (sum > max_val) {
                max_val = sum;
            }
            if (sum < 0) {
                sum = 0;
            }
        }
        return max_val;
    }

};

//kmp 算法
/*
* Author: 酒馆店小二
* Description: kmp算法学习
* Date: 2022-2-17 22:41:52 星期四
* FileName: kmp.cpp
* Location: D:\VSCODE_CPP\algorithm\kmp\kmp.cpp
*/

int violentMatch(const std::string &pattern, const std::string &substr) {
    int pLen = pattern.size();
    int sLen = substr.size();
    int i = 0, j = 0; // i、j 分别指向 pattern 和 substr 的下标
    while (i < pLen && j < sLen) {
        if (pattern[i] == substr[j]) { // 当前字符匹配成功
            i++;
            j++;
        } else { // 当前字符匹配失败
            i = i - j + 1;
            j = 0;
        }
    }
    if (j == sLen) { // 匹配成功，返回子串在模式串中的位置，否则返回 -1
        return i - j;
    } else {
        return -1;
    }
}

/**
 * 给定一个字符串 s ，请你找出其中不含有重复字符的 最长 子串 的长度
 * 示例 1:

输入: s = "abcabcbb"
输出: 3
解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
示例 2:

输入: s = "bbbbb"
输出: 1
解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
示例 3:

输入: s = "pwwkew"
输出: 3
解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。
     请注意，你的答案必须是 子串 的长度，"pwke" 是一个子序列，不是子串。
     给定一个字符串 s ，请你找出其中不含有重复字符的 最长 子串 的长度
 */
int lengthOfLongestSubstring(std::string s) {
    std::string ans;
    int max = 0;
    for (char e: s) {
        int index = ans.find(e); //返回e在字符串中第一次出现的下标值
        if (index != std::string::npos) { //如果ans中没有e，find函数会返回string::npos，所以!=时表示有e，需要删除
            ans.erase(0, index + 1); //ans.erase(pos,n)删除pos下标开始的n个值，截至e出现的字符都删除
            printf("283------当前遍历的元素：%c   结果值：%s\n", e, ans.c_str());
        }
        ans += e; //新字符添加到字符串中
        max = ans.size() > max ? ans.size() : max; //记录ans字符串元素个数最大值
    }
    printf("最长不重复子串：%s   个数 = %d \n", ans.c_str(), max);
    return max;
}

/**
 * 求数组的全排列
 */
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void perm(int list[], int low, int high) {
    if (low == high) {   //当low==high时,此时list就是其中一个排列,输出list
        for (int i = 0; i <= low; i++)
            std::cout << list[i];
        std::cout << std::endl;
    } else {
        for (int i = low; i <= high; i++) {//每个元素与第一个元素交换
            swap(list[i], list[low]);
            perm(list, low + 1, high); //交换后,得到子序列,用函数perm得到子序列的全排列
            swap(list[i], list[low]);//最后,将元素交换回来,复原,然后交换另一个元素
        }
    }
}
/********************求数组的全排列*************/

/*****-----------------------------*/
//给你一个整数数组 nums ，数组中的元素 互不相同 。返回该数组所有可能的子集
// [1, 2, 3, 4],  结果：
// 1.    ans = [[ ]],  nums[0] = 1,  将元素1和ans内所有子集组合并添加进ans
//       此时：ans = [[ ], [1]]；
//2.    ans = [[ ], [1]],  nums[1] = 2,  将元素2和ans内所有子集组合并添加进ans
//       此时：ans = [[ ], [1], [2], [1, 2]]；
//3.    ans = [[ ], [1], [2], [1, 2]],  nums[2] = 3,  将元素3和ans内所有子集组合并添加进ans
//       此时：ans = [[ ], [1], [2], [1, 2], [3], [1, 3], [2, 3], [1, 2, 3]]；
//3.    ans = [[ ], [1], [2], [1, 2], [3], [1, 3], [2, 3], [1, 2, 3]],  nums[3] = 3,  将元素3和ans内所有子集组合并添加进ans
//       此时：ans = [[ ], [1], [2], [1, 2], [3], [1, 3], [2, 3], [1, 2, 3], [4], [1, 4], [2, 4], [1, 2, 4], [3, 4], [1, 3, 4], [2, 3, 4], [1, 2, 3, 4]]；
class Solution78 {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> ans = {{}};
        for (int i = 0; i < nums.size(); i++) {
            int len = ans.size();
            for (int j = 0; j < len; j++) {
                ans[j].emplace_back(nums[i]);
                ans.emplace_back(ans[j]);
                ans[j].pop_back();
            }
        }
        return ans;
    }
};
/*****-----------------------------*/
//给你一个以字符串表示的非负整数 num 和一个整数 k ，移除这个数中的 k 位数字，使得剩下的数字最小。请你以字符串形式返回这个最小的数字
class Solution402 {
public:
    string removeKdigits(string num, int k) {
        for(int i=0;k!=0&&i<num.size();){
            if(num[i]>num[i+1]){
                num.erase(i,1);
                k--;
                if(i>0)i--;
                continue;
            }
            i++;
        }
        num.erase(num.size()-k,k);
        while(num[0]=='0')num.erase(0,1);
        if(num=="")return "0";
        return num;
    }
};



/**
 * 给定不同面额的硬币和一个总金额。写出函数来计算可以凑成总金额的硬币组合数。假设每一种面额的硬币有无限个。
 * @param amount
 * @return
 */
int change(int amount, int *coins, int size) {
    int *dp = new int[amount + 1];
    dp[0] = 1;

    for (int i = 0; i < size; i++) {
        int x = *(coins + i);
        for (int j = x; j < amount + 1; j++) {
            dp[j] += dp[j - x];
        }
    }

//    for (int coin : coins) {
//        for (int x = coin; x < amount + 1; ++x) {
//            dp[x] += dp[x - coin];
//        }
//    }
    return dp[amount];
}

/**
 * 给你一个 无重复元素 的整数数组 candidates 和一个目标整数 target ，
 * 找出 candidates 中可以使数字和为目标数 target 的 所有 不同组合 ，
 * 并以列表形式返回。你可以按 任意顺序 返回这些组合
 */
class Solution39 {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<int> state;              // 状态（子集）
        sort(candidates.begin(), candidates.end()); // 对 candidates 进行排序
        int start = 0;                  // 遍历起始点
        vector<vector<int>> res;        // 结果列表（子集列表）
        backtrack(state, target, candidates, start, res);
        return res;
    }
private:
    void backtrack(vector<int> &state, int target, vector<int> &choices, int start, vector<vector<int>> &res) {
        // 子集和等于 target 时，记录解
        if (target == 0) {
            res.push_back(state);
            return;
        }
        // 遍历所有选择
        // 剪枝二：从 start 开始遍历，避免生成重复子集
        for (int i = start; i < choices.size(); i++) {
            // 剪枝一：若子集和超过 target ，则直接结束循环
            // 这是因为数组已排序，后边元素更大，子集和一定超过 target
            if (target - choices[i] < 0) {
                break;
            }
            // 尝试：做出选择，更新 target, start
            state.push_back(choices[i]);
            // 进行下一轮选择
            backtrack(state, target - choices[i], choices, i, res);
            // 回退：撤销选择，恢复到之前的状态
            state.pop_back();
        }
    }
};



/*给定一个单链表 L 的头节点 head ，单链表 L 表示为：

L0 → L1 → … → Ln - 1 → Ln
        请将其重新排列后变为：

L0 → Ln → L1 → Ln - 1 → L2 → Ln - 2 → …
不能只是单纯的改变节点内部的值，而是需要实际的进行节点交换。*/
/* Definition for singly-linked list.*/
struct ListNode {
    int val;
    ListNode *next;

    ListNode() : val(0), next(nullptr) {}

    ListNode(int x) : val(x), next(nullptr) {}

    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution2 {
public:
    void reorderList(ListNode *head) {
        if (head == nullptr || head->next == nullptr) return;
        ListNode *mid = FindMiddleNode(head);   // 找到中间节点
        ListNode *l1 = head;
        ListNode *l2 = mid->next;
        mid->next = nullptr;
        l2 = ReverseList(l2);   // 反转后半段的链表节点
        mergeList(l1, l2);      // 合并两端长度相差不超过1的链表
    }

    ListNode *FindMiddleNode(ListNode *head) {  // 快慢指针找到中间节点
        ListNode *slow = head;
        ListNode *fast = head;
        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    ListNode *ReverseList(ListNode *head) { // 反转链表
        ListNode *pre = nullptr;
        ListNode *cur = head;
        ListNode *next;
        while (cur != nullptr) {
            next = cur->next;
            cur->next = pre; // 反转
            pre = cur;       // 向后移动
            cur = next;
        }
        return pre;
    }

    // 合并链表
    void mergeList(ListNode *l1, ListNode *l2) {
        ListNode *temp1;
        ListNode *temp2;
        while (l1 != nullptr && l2 != nullptr) {
            temp1 = l1->next;
            temp2 = l2->next;
            l1->next = l2;
            l1 = temp1;
            l2->next = l1;
            l2 = temp2;
        }
    }
};

//找到链表的倒数第n 个元素
ListNode *removeNthFromEnd(ListNode *head, int n) {
    ListNode *dummynode = new ListNode(0);
    dummynode->next = head;
    ListNode *fast = dummynode;
    ListNode *slow = dummynode;
    for (int i = 0; i <= n; i++) {
        fast = fast->next;
    }
    while (fast != nullptr) {
        fast = fast->next;
        slow = slow->next;
    }
    ListNode *temp = slow->next;
    slow->next = temp->next;
    delete temp;
    ListNode *result = dummynode->next;
    delete dummynode;
    return result;

}

/**
 * 输入一个n*n的正方形矩阵a，要求填充逻辑代码，将矩阵a顺时针旋转90度
 * 设行索引编号为x，列索引编号为y，通过观察可发现，经过旋转后的列索引变为n - x - 1，行索引变为y。
 * 因此可根据上述关系可知，旋转前matrix[x][y]位置在旋转后的新位置便变为matrix[y][n-x-1]
 * @param matrix
 */
void rotate(std::vector<std::vector<int>> &matrix) {
    int n = matrix.size();
    auto matrix_new = matrix;            // 这里的 = 拷贝值，将拷贝的值传给一个新的数组
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix_new[j][n - i - 1] = matrix[i][j];
        }
    }
    matrix = matrix_new;
}


class Solution3 {
public:
    int calculate(std::string s) {
        int begin = 0;
        return calHelper(s, begin);
    }

    int calHelper(std::string s, int &i) {
        char operation = '+';
        std::stack<int> nums;
        int num = 0;
        int res = 0;
        bool flag = false;
        for (i; i < s.size(); i++) {
            if (s[i] >= '0' && s[i] <= '9') {
                num = num * 10 + (s[i] - '0');
            }
            if (s[i] == '(') {
                num = calHelper(s, ++i); //从i的下一个开始计算， 进入递归
                i++; //计算完之后的i指向)所以再++
            }
            if (((s[i] < '0' || s[i] > '9') && s[i] != ' ') || i >= s.size() - 1) {
                int pre = 0;
                switch (operation) {
                    case '+':
                        nums.push(num);
                        break;
                    case '-':
                        nums.push(-num);
                        break;
                    case '*':
                        pre = nums.top();
                        nums.pop();
                        nums.push(pre * num);
                        break;
                    case '/':
                        pre = nums.top();
                        nums.pop();
                        nums.push(pre / num);
                        break;
                }
                operation = s[i];
                num = 0;
            }
            if (s[i] == ')') {
                break;
            }
        }
        while (!nums.empty()) {
            res += nums.top();
            nums.pop();
        }
        return res;
    }
};

/**
 * 最大公约数
 * @param x
 * @param y
 * @return
 */
int Gcd(int x, int y) {
    int z = y;
    while (x % y != 0) {
        z = x % y;
        x = y;
        y = z;
    }
    return z;
}

/***-------------3个线程交替打印abc----------------**/
std::mutex mtx;
std::condition_variable cv;
int ready = 0;

void PrintString_1() {
    std::unique_lock<std::mutex> lk(mtx);
    int cnt = 0;
    while (cnt < 10) {
        while (ready != 0)
            //std::cout << " 线程1 阻塞" << std::endl;
            cv.wait(lk);
        std::cout << " 线程1打印 A" << std::endl;
        ready = 1;
        cnt++;
        cv.notify_all();
    }
}

void PrintString_2() {
    std::unique_lock<std::mutex> lk(mtx);
    int cnt = 0;
    while (cnt < 10) {
        while (ready != 1)
            //std::cout << " 线程2 阻塞" << std::endl;
            cv.wait(lk);
        std::cout << " 线程2打印 B" << std::endl;
        ready = 2;
        cnt++;
        cv.notify_all();
    }
}

void PrintString_3() {
    std::unique_lock<std::mutex> lk(mtx);
    int cnt = 0;
    while (cnt < 10) {
        while (ready != 2)
            //std::cout << " 线程3 阻塞" << std::endl;
            cv.wait(lk);
        std::cout << " 线程3打印 C" << std::endl;
        ready = 0;
        cnt++;
        cv.notify_all();
    }
}

int printABC() {
    std::thread t1(PrintString_1);
    std::thread t2(PrintString_2);
    std::thread t3(PrintString_3);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}


/***-----------------------------**/



/***-----------------有一串首尾相连的珠子，共有m个，每一个珠子有一种颜色，并且颜色的总数不超过n(n<=10)，求连续的珠子的颜色总数为n时，长度最小的区间-------------------**/
// https://www.iteye.com/blog/wangminshe89-1419910
int getmin(int *arr,int len)
{
    if(arr == NULL || len < 1)
        return -1;
    int min = arr[0];
    for(int i = 1; i < len; ++i)
    {
        if( arr[i] < min)
            min = arr[i];
    }
    return min;
}

int getmax(int *arr,int len)
{
    if(arr == NULL || len < 1)
        return -1;
    int max = arr[0];
    for(int i = 1; i < len; ++i)
    {
        if( arr[i] > max)
            max = arr[i];
    }
    return max;
}

int minlength(int *arr, int len, int colors)
{
    if(len < colors)
        return -1;

    int *tarr = new int [colors];
    int curcolors=0;
    int maxposition=0;
    int minposition=len;
    int minlen = 0;
    memset(tarr,-1,sizeof(int)*colors);

    int index=0;
    /* 查找第一次出现的子段 */
    for(;index<len;++index)
    {
        if( tarr[ arr[index] -1 ] < 0)
            ++curcolors;
        tarr[ arr[index]-1 ] = index; //颜色数组

        if(curcolors == colors)
        {
            minposition = getmin(tarr,colors);
            maxposition = getmax(tarr,colors);
            minlen = maxposition - minposition + 1;
            break;
        }
    }

    int minp = 0;
    int maxp = 0;
    for(++index;index < len; ++index)
    {
        /* 这样是上面提到的再次找到A */
        if( tarr[ arr[index]-1 ] == minposition)
        {
            tarr[ arr[index]-1 ] = index;
            minp = getmin(tarr,colors);
            maxp = getmax(tarr,colors);
            if(maxposition - minposition + 1 < minlen)
            {
                minlen = maxposition - minposition + 1;
                minposition = minp;
                maxposition = maxp;
            }
            continue;
        }

        tarr[ arr[index]-1 ] = index;

    }
    cout<<"minposition: "<<minposition<<endl;
    cout<<"maxposition: "<<maxposition<<endl;
    cout<<"len : "<<minlen<<endl;
    return minlen;
}

int findMinLength()
{
    int arr[] = {1,2,2,3,1,3,4,2,3,5,4,2,3,2,4,3,2,1,2,1,4,5,2,2,2,2,3};
    int minlen = minlength(arr,27,5);
    printf("687--------最小 的长度为  %d\n",minlen);
    return(0);
}

/***-----------------有一串首尾相连的珠子，共有m个，每一个珠子有一种颜色，并且颜色的总数不超过n(n<=10)，求连续的珠子的颜色总数为n时，长度最小的区间-------------------**/

//回字形打印 数据  [[1,2,3],[8,9,4],[7,6,5]]
/**
 * 对于每层，从左上方开始以顺时针的顺序遍历所有元素。假设当前层的左上角位于 (top,left)，右下角位于 (bottom,right)，按照如下顺序遍历当前层的元素。
从左到右遍历上侧元素，依次为 (top,left) 到 (top,right)。
从上到下遍历右侧元素，依次为 (top+1,right) 到 (bottom,right)。
如果 left<right 且 top<bottom，则从右到左遍历下侧元素，依次为 (bottom,right−1) 到 (bottom,left+1)，以及从下到上遍历左侧元素，依次为 (bottom,left) 到 (top+1,left)。
遍历完当前层的元素之后，将 left 和 top 分别增加 1，将 right 和 bottom 分别减少 1，进入下一层继续遍历，直到遍历完所有元素为止。
 */
class Solution4 {
public:
    vector<int> spiralArray(vector<vector<int>>& array) {
        if (array.size() == 0 || array[0].size() == 0) {
            return {};
        }

        int rows = array.size(), columns = array[0].size();
        vector<int> order;
        int left = 0, right = columns - 1, top = 0, bottom = rows - 1;
        while (left <= right && top <= bottom) {
            for (int column = left; column <= right; column++) {
                order.push_back(array[top][column]);
            }
            for (int row = top + 1; row <= bottom; row++) {
                order.push_back(array[row][right]);
            }
            if (left < right && top < bottom) {
                for (int column = right - 1; column > left; column--) {
                    order.push_back(array[bottom][column]);
                }
                for (int row = bottom; row > top; row--) {
                    order.push_back(array[row][left]);
                }
            }
            left++;
            right--;
            top++;
            bottom--;
        }
        return order;
    }
};

/***------------------最长回文公共子串------------------------**/
//将字符串 s 反转得到字符串 rev，再求他们的最长公共子串，再判断该最长公共子串是否就是我们要找的最长回文子串。
class Solution5 {
public:
    string longestPalindrome(string s) {
        if(s.length()==1) return s;//大小为1的字符串必为回文串
        string rev=s;//rev存放s反转结果
        string res;//存放结果
        std::reverse(rev.begin(),rev.end());
        if(rev==s) return s;
        int len=0;//存放回文子串的长度
        for(int i=0;i<s.length();i++)//查找s与rev的最长公共子串
        {
            string temp;//存放待验证子串
            for(int j=i;j<s.length();j++)
            {
                temp=temp+s[j];
                if(len>=temp.length())
                    continue;
                else if(rev.find(temp)!=-1)//在rev中找到temp
                {
                    string q=temp;//q用来验证temp是否是回文子串
                    std::reverse(q.begin(),q.end());
                    if(q==temp)
                    {
                        len=temp.length();
                        res=temp;
                    }
                }
                else break;
            }
            temp="";
        }
        return res;
    }
};
/***------------------最长回文公共子串------------------------**/
//给定两个字符串 text1 和 text2，返回这两个字符串的最长 公共子序列 的长度。如果不存在 公共子序列 ，返回 0 。

class Solution114 {
public:
    int longestCommonSubsequence(string text1, string text2) {
        int m = text1.length(), n = text2.length();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        for (int i = 1; i <= m; i++) {
            char c1 = text1.at(i - 1);
            for (int j = 1; j <= n; j++) {
                char c2 = text2.at(j - 1);
                if (c1 == c2) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[m][n];
    }
};
/***------------------最长回文公共子串------------------------**/



int main() {

//    string s23232 = "012345";
//    string s_re = s23232.erase(1,2);
//    cout << "882--------s2 = " << s_re <<endl;


    findMinLength();
    return 0;

    printABC();

    int reee = Gcd(8, 12);
    printf("533--------ree = %d\n", reee);

    std::string s = "pwwkew";
    int re = lengthOfLongestSubstring(s);
    printf("341---------%s  最大不重复子串为 : %d\n", s.c_str(), re);

    char s2[] = "Hello, world!";
    printf("433------sizeof(s2) = %lu   strlen(s2) = %zu\n", sizeof(s2), strlen(s2));

    return 0;


    int a[8] = {3, 2, 4, 1, 7, 8, 5, 6};
    //bubbleSort(a,8);
    quickSort(a, 0, 8);
    printf("排序之后的顺序为：\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    pushQueue(1);
    pushQueue(2);
    pushQueue(3);

    popQueue();
    popQueue();
    popQueue();

    printf("两个队列模拟一个栈：\n");
    pushStack(1);
    //stackPop();
    pushStack(2);
    stackPop();
    pushStack(3);
    stackPop();
    pushStack(4);

//    stackPop();
//    stackPop();
//    stackPop();
//    stackPop();


//    A* b = new A();
//    std::shared_ptr<A> sp = std::make_shared<A>();


    std::string pattern = "0123456789";
    std::string subStr = "678";
    int idx = violentMatch(pattern, subStr);
    printf("300-------查找目标子串的起始位置 idx = %d\n", idx);

    printf("求数组的全排列:\n");
    int list[] = {1, 2, 3, 4};
    perm(list, 0, 3);

    return 0;
}