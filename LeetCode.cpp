//
// Created by Rambo.Liu on 2025/5/16.
//
#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <set>
#include <queue>

using namespace std;

/**
 * 1. 两数之和
 * 给定一个整数数组 nums 和一个整数目标值 target，请你在该数组中找出 和为目标值 target  的那 两个 整数，并返回它们的数组下标
 * @param arr
 * @param len
 * @param target
 */
void findTargetInArray(int *arr, int len, int target) {
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (target == *(arr + i) + (*(arr + j))) {
                printf("19-------找到了这2个数 为 a[%d] = %d  a[%d]  = %d\n", i, *(arr + i), j, *(arr + j));
            }
        }
    }
}

/**
 * 给定一个未排序的整数数组 nums ，找出数字连续的最长序列（不要求序列元素在原数组中连续）的长度。
 * 核心思路：对于 nums 中的元素 x，以 x 为起点，不断查找下一个数 x+1,x+2,⋯ 是否在 nums 中，并统计序列的长度。
为了做到 O(n) 的时间复杂度，需要两个关键优化：
把 nums 中的数都放入一个哈希集合中，这样可以 O(1) 判断数字是否在 nums 中。
如果 x−1 在哈希集合中，则不以 x 为起点。为什么？因为以 x−1 为起点计算出的序列长度，一定比以 x 为起点计算出的序列长度要长！这样可以避免大量重复计算。
 比如 nums=[3,2,4,5]，从 3 开始，我们可以找到 3,4,5 这个连续序列；而从 2 开始，我们可以找到 2,3,4,5 这个连续序列，一定比从 3 开始的序列更长。
 */
int longestConsecutive(vector<int> &nums) {
    int ans = 0;
    unordered_set<int> st(nums.begin(), nums.end()); // 把 nums 转成哈希集合
    for (int x: st) { // 遍历哈希集合
        if (st.find(x - 1) != st.end()) {
            continue;
        }
        // x 是序列的起点
        int y = x + 1;
        while (st.find(y) != st.end()) { // 不断查找下一个数是否在哈希集合中
            y++;
        }
        // 循环结束后，y-1 是最后一个在哈希集合中的数
        ans = max(ans, y - x); // 从 x 到 y-1 一共 y-x 个数
    }
    return ans;
}

/**
 * 给定一个长度为 n 的整数数组 height 。有 n 条垂线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i]) 。
 * 找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水
 * [1,8,6,2,5,4,8,3,7] =  最大装水量 49
 * 面积公式： S(i,j)=min(h[i],h[j])×(j−i)
 * @param height
 * @return
 */
int maxArea(vector<int> &height) {
    int i = 0, j = height.size() - 1, res = 0;
    while (i < j) {
        res = height[i] < height[j] ?
              max(res, (j - i) * height[i++]) :
              max(res, (j - i) * height[j--]);
    }
    printf("70----------最大装水量为：sum = %d\n", res);
    return res;
}

int maxArea2(vector<int> &height) {
    int sum = 0;
    for (int i = 0; i < height.size() - 1; i++) {
        for (int j = i; j < height.size(); j++) {
            sum = max(sum, min(height[j], height[i]) * (j - i));
        }
    }
    printf("81----------最大装水量为：sum = %d\n", sum);
    return sum;
}

/**
 * 给你一个整数数组 nums 和一个整数 k ，请你统计并返回 该数组中和为 k 的子数组的个数 。子数组是数组中元素的连续非空序列。
 * @param nums
 * @param k
 * @return
 */
int subarraySum(vector<int> &nums, int k) {
    unordered_map<int, int> psCounts = {{0, 1}};    // 统计前缀和出现次数，初始ps[0] = 0出现一次
    int res = 0;                                    // 满足条件的子数组个数
    int ps = 0;                                     // 当前前缀和ps[i]
    for (auto &num: nums) {
        ps += num;                  // 更新前缀和，[0, i+1)
        res += psCounts[ps - k];    // 累加以元素nums[i]为结尾，满足子数组条件个数
        psCounts[ps] += 1;          // 将当前前缀和加入哈希表
    }
    return res;
}

/**
 * 给定一个整数数组 nums，将数组中的元素向右轮转 k 个位置，其中 k 是非负数。
示例 1:
输入: nums = [1,2,3,4,5,6,7], k = 3
输出: [5,6,7,1,2,3,4]
解释:
向右轮转 1 步: [7,1,2,3,4,5,6]
向右轮转 2 步: [6,7,1,2,3,4,5]
向右轮转 3 步: [5,6,7,1,2,3,4]
 * @param nums
 * @param k
 */
//void rotate(vector<int>& nums, int k) {
//    k %= nums.size(); // 轮转 k 次等于轮转 k % n 次
//    ranges::reverse(nums);   //[7,6,5,4,3,2,1]  这行代码需要  在C++ 20 生效  ，数组逆转
//    reverse(nums.begin(), nums.begin() + k);  //[5,6,7,4,3,2,1]
//    reverse(nums.begin() + k, nums.end()); //[5,6,7,1,2,3,4]
//}


//给你两个 非空 的链表，表示两个非负的整数。它们每位数字都是按照 逆序 的方式存储的，并且每个节点只能存储 一位 数字。
//
//请你将两个数相加，并以相同形式返回一个表示和的链表
// leetcoode 2 数相加
struct ListNode {
    int val;
    ListNode *next;

    ListNode(int x) : val(x), next(NULL) {}
};

ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
    ListNode *dummy = new ListNode(0); // 辅助节点
    ListNode *pre = dummy;
    int carry = 0;
    while (l1 || l2) {
        int n1 = l1 ? l1->val : 0;
        int n2 = l2 ? l2->val : 0;
        int sum = n1 + n2 + carry;
        pre->next = new ListNode(sum % 10);
        carry = sum / 10;

        pre = pre->next;
        if (l1) l1 = l1->next;
        if (l2) l2 = l2->next;
    }
    if (carry > 0) pre->next = new ListNode(carry);

    return dummy->next;
}

//链表排序
ListNode *sortList(ListNode *head) {
    if (!head) {
        return nullptr;
    }

    //使用直接插入法进行排序
    ListNode *dummy = new ListNode(0); // 辅助节点
    ListNode *cur = head;

    while (cur) {
        //从新链表中查询第一个  比当前结点  大的结点，然后将当前结点  前插入  新链表中
        ListNode *newPre = dummy;
        ListNode *newCur = newPre;

        while (newCur && newCur->val < cur->val) {
            newPre = newCur;
            newCur = newCur->next;
        }
        //找到了第一个  比当前结点  大的结点
        ListNode *tmp = new ListNode(cur->val); // 辅助节点

        tmp->next = newCur;
        newPre->next = tmp;

        cur = cur->next;
    }

    //打印
    ListNode *tmp = dummy;
    while (tmp) {
        printf("185----  %d\n", tmp->val);
        tmp = tmp->next;
    }
    return dummy;

}

/**
 * Definition for a binary tree node.
 */
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {}

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}

    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

/**
 * 给你二叉树的根结点 root ，请你将它展开为一个单链表：
 * 展开后的单链表应该同样使用 TreeNode ，其中 right 子指针指向链表中下一个结点，而左子指针始终为 null 。
 * 展开后的单链表应该与二叉树 先序遍历 顺序相同。
 leet code 114
 */
class Solution {
public:
    void flatten(TreeNode *root) {
        // 时间复杂度O(N)，空间复杂度O(1)
        TreeNode *node;
        while (root) {
            if (root->left) {
                node = root->left;
                while (node->right) {
                    node = node->right;
                }
                node->right = root->right;
                root->right = root->left;
                root->left = nullptr;
            }
            root = root->right;
        }
    }
};


/**
 * 判断字符是否为回文
 * @param s
 * @param left
 * @param right
 * @return
 */
bool isPalindrome(string &s, int left, int right) {
    while (left < right) {
        if (s[left++] != s[right--]) {
            return false;
        }
    }
    return true;
}

/**
 * 分割回文子串：将一个字符串的回文子串分割出来 ，存放到一个数组中并返回
 * 示例 1：

输入：s = "aab"
输出：[["a","a","b"],["aa","b"]]
示例 2：

输入：s = "a"
输出：[["a"]]
 *
 * @param s
 * @return
 */
vector<vector<string>> partition(string s) {
    vector<vector<string>> ans;
    vector<string> path;
    int n = s.length();
    auto dfs = [&](auto &&dfs, int i) {
        if (i == n) { // s 分割完毕
            ans.emplace_back(path);
            return;
        }
        for (int j = i; j < n; j++) { // 枚举子串的结束位置
            if (isPalindrome(s, i, j)) {
                path.push_back(s.substr(i, j - i + 1)); // 分割！
                // 考虑剩余的 s[j+1] ~ s[n-1] 怎么分割
                dfs(dfs, j + 1);
                path.pop_back(); // 恢复现场
            }
        }
    };
    dfs(dfs, 0);
    return ans;

}

/**
 * 数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且 有效的 括号组合
 * 示例 1：

输入：n = 3
输出：["((()))","(()())","(())()","()(())","()()()"]
示例 2：

输入：n = 1
输出：["()"]
 */
class Solution22 {
public:
    vector<string> generateParenthesis(int n) {
        dfs("", n, n);
        return res;
    }

private:
    vector<string> res;

    void dfs(const string &str, int left, int right) {
        if (left < 0 || left > right)  // 出现类似 ()) )) 这种格式都是错误的不用再继续了
            return;
        if (left == 0 && right == 0) {
            res.push_back(str);
            return;
        }
        dfs(str + '(', left - 1, right);
        dfs(str + ')', left, right - 1);
    }
};

/**
 * 给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量。
    岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。
    此外，你可以假设该网格的四条边均被水包围
 */
class Solution200 {
public:
    int numIslands(vector<vector<char>> &grid) {
        int m = grid.size(), n = grid[0].size();
        auto dfs = [&](auto &&dfs, int i, int j) -> void {
            // 出界，或者不是 '1'，就不再往下递归
            if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] != '1') {
                return;
            }
            grid[i][j] = '2'; // 插旗！避免来回横跳无限递归
            dfs(dfs, i, j - 1); // 往左走
            dfs(dfs, i, j + 1); // 往右走
            dfs(dfs, i - 1, j); // 往上走
            dfs(dfs, i + 1, j); // 往下走
        };

        int ans = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '1') { // 找到了一个新的岛
                    dfs(dfs, i, j); // 把这个岛插满旗子，这样后面遍历到的 '1' 一定是新的岛
                    ans++;
                }
            }
        }
        return ans;
    }
};

/**
 * 找到给定的二维数组中最大的岛屿面积
 */
class Solution105 {
public:
    int maxAreaOfIsland(vector<vector<int>> &grid) {
        int ans = 0;
        for (int i = 0; i < grid.size(); i++)
            for (int j = 0; j < grid[0].size(); j++)
                ans = max(ans, dfs(i, j, grid));
        return ans;
    }

    int dfs(int x, int y, vector<vector<int>> &grid) {
        if (x < 0 || y < 0 || x >= grid.size() || y >= grid[0].size() || grid[x][y] == 0)
            return 0;
        grid[x][y] = 0;
        return 1 + dfs(x - 1, y, grid) + dfs(x, y + 1, grid) + dfs(x + 1, y, grid) + dfs(x, y - 1, grid);
    }
};

/**
 * 最大的正方形 leetcode 221
 * @param matrix
 * @return
 */
int maximalSquare(vector<vector<char>> &matrix) {
    int row = matrix.size();
    int col = matrix[0].size();
    int maxSide = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            //找到第一个元素为1 将他作为正方形的左上角
            if (matrix[i][j] == '1') {
                // 遇到一个 1 作为正方形的左上角
                maxSide = max(maxSide, 1);
                // 计算可能的最大正方形边长
                int currentMaxSide = min(row - i, col - j);
                for (int k = 1; k < currentMaxSide; k++) {
                    // 判断新增的一行一列是否均为 1
                    bool flag = true;
                    if (matrix[i + k][j + k] == '0') {
                        break;
                    }
                    for (int m = 0; m < k; m++) {
                        if (matrix[i + k][j + m] == '0' || matrix[i + m][j + k] == '0') {
                            flag = false;
                            break;
                        }
                    }
                    if (flag) {
                        maxSide = max(maxSide, k + 1);
                    } else {
                        break;
                    }

                }

            }
        }
    }

    return maxSide * maxSide;


}

/**
 * 岛屿的周长
 */
class Solution463 {
public:
    int islandPerimeter(vector<vector<int>> &grid) {
        int m = grid.size();
        if (m == 0) return 0;
        int n = grid[0].size();
        int ans = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    ans += 4;
                    if (i < m - 1 && grid[i + 1][j] == 1) ans -= 2;
                    if (j < n - 1 && grid[i][j + 1] == 1) ans -= 2;
                }
            }
        }
        return ans;
    }
};

/**
 * 去掉字符串中重复的字符，并保证原始顺序
 * ['a','b','c','b','a','e'] 结果为 ：['a','b','c','e']
 * @param s
 */
void removeDuplicat() {
    char arr[] = {'a', 'b', 'c', 'a', 'd', 'b', 'e'};
    int n = sizeof(arr) / sizeof(arr[0]);

    // 使用 std::set 去重
    std::set<char> uniqueChars(arr, arr + n);

    // 将 set 转换回 vector 保持顺序
    std::vector<char> result(uniqueChars.begin(), uniqueChars.end());

    // 输出结果
    printf("418-------去重后的结果为:\n");
    for (char c: result) {
        std::cout << c << ' ';
    }
    std::cout << std::endl;
}

/**
 * 给你一个字符串 s ，请你去除字符串中重复的字母，使得每个字母只出现一次。需保证 返回结果的字典序最小
 */
class Solution316 {
public:
    string removeDuplicateLetters(string s) {
        vector<int> left(26), in_ans(26);
        for (char c: s) {
            left[c - 'a']++; // 统计每个字母的出现次数
        }
        string ans;
        for (char c: s) {
            left[c - 'a']--;
            if (in_ans[c - 'a']) { // ans 中不能有重复字母
                continue;
            }
            while (!ans.empty() && c < ans.back() && left[ans.back() - 'a']) {
                // (设 x=ans.back()) 如果 c < x，且右边还有 x，那么可以把 x 去掉，
                // 因为后面可以重新把 x 加到 ans 中
                in_ans[ans.back() - 'a'] = false; // 标记 x 不在 ans 中
                ans.pop_back();
            }
            ans += c; // 把 c 加到 ans 的末尾
            in_ans[c - 'a'] = true; // 标记 c 在 ans 中
        }
        return ans;
    }
};

/**
 * 按层遍历2叉树
 */
class Solution102 {
public:
    vector<vector<int>> levelOrder(TreeNode *root) {
        if (root == nullptr) {
            return {};
        }
        vector<vector<int>> ans;
        queue<TreeNode *> q;
        q.push(root);
        while (!q.empty()) {
            vector<int> vals;
            //只遍历之前 q 中已经有的数据
            for (int n = q.size(); n--;) {
                auto node = q.front();
                q.pop();
                vals.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            ans.emplace_back(vals);
        }
        return ans;
    }
};

/**
 * 给你两个单链表的头节点 headA 和 headB ，请你找出并返回两个单链表相交的起始节点。如果两个链表不存在相交节点，返回 null
 * @param headA
 * @param headB
 * @return
 */
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {

    if (!headA || !headB) {
        return nullptr;
    }

    //找出链表的长度
    int a_length = 0;
    int b_length = 0;
    ListNode *tempA = headA;
    while (tempA) {
        a_length++;
        tempA = tempA->next;
    }
    ListNode *tempB = headB;
    while (tempB) {
        b_length++;
        tempB = tempB->next;
    }

    if (a_length > b_length) {
        //得到差值
        int diff = a_length - b_length;
        //对齐两个端链表
        ListNode *tmpHeadA = headA;
        for (int i = 0; i < diff; i++) {
            tmpHeadA = tmpHeadA->next;
        }
        ListNode *tmpHeadB = headB;
        ListNode *resultNode = nullptr;
        while (tmpHeadA && tmpHeadB) {
            if (tmpHeadA == tmpHeadB) {
                //找到了结点
                resultNode = tmpHeadA;
                break;
            }
            tmpHeadA = tmpHeadA->next;
            tmpHeadB = tmpHeadB->next;
        }

        return resultNode;
    } else if (a_length < b_length) {
        //得到差值
        int diff = b_length - a_length;
        //对齐两个端链表
        ListNode *tmpHeadB = headB;
        for (int i = 0; i < diff; i++) {
            tmpHeadB = tmpHeadB->next;
        }
        ListNode *tmpHeadA = headA;
        ListNode *resultNode = nullptr;
        while (tmpHeadA && tmpHeadB) {
            if (tmpHeadA == tmpHeadB) {
                //找到了结点
                resultNode = tmpHeadA;
                break;
            }
            tmpHeadA = tmpHeadA->next;
            tmpHeadB = tmpHeadB->next;
        }

        return resultNode;
    } else {
        ListNode *tmpHeadA = headA;
        ListNode *tmpHeadB = headB;
        ListNode *resultNode = nullptr;
        while (tmpHeadA && tmpHeadB) {
            if (tmpHeadA == tmpHeadB) {
                //找到了结点
                resultNode = tmpHeadA;
                break;
            }
            tmpHeadA = tmpHeadA->next;
            tmpHeadB = tmpHeadB->next;
        }

        return resultNode;
    }

}

//求一颗树的根结点到任意结点的路径
//助栈结构来保存路径上的结点，首先从根结点开始，一直往左找，如果左边找到就返回true；
// 否则，如果左边找不到并且右子树不为空的情况下再继续往右子树找。如果左右子树都找不到，
// 就弹出栈顶结点并返回false。方法运行完毕后，栈中保存的元素就是一条从根到给定结点的路径
bool findPath(TreeNode *root, int target, vector<int> &path) {
    if (root == nullptr) return false;

    // 将当前节点的值加入路径
    path.push_back(root->val);

    // 如果当前节点是目标节点，返回true
    if (root->val == target) return true;

    // 递归地在左子树和右子树中查找
    if (root->left && findPath(root->left, target, path)) return true;
    if (root->right && findPath(root->right, target, path)) return true;

    // 如果当前节点不是目标节点，并且在其子树中未找到目标节点，移除当前节点的值（回溯）
    path.pop_back();
    return false;

}

//查找给定两个结点的最近公共结点
int lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {

    //查找 根结点到p 结点的路径
    vector<int> p_path;
    findPath(root, p->val, p_path);

    //查找 根结点到q 结点的路径
    vector<int> q_path;
    findPath(root, q->val, q_path);

    //得到两条路径之后  判断第一个出现相同的结点
    int lca = -1;
    for (int i = 0; i < p_path.size() && i < q_path.size(); i++) {
        if (p_path[i] == q_path[i]) {
            lca = q_path[i];
        } else {
            break;
        }
    }
    return lca;
}

//判断链表是否为回文链表
bool isPalindrome(ListNode *head) {
    if (!head) {
        return false;
    }
    ListNode *tmp = head;
    int sum = 0;
    while (tmp) {
        sum++;
        tmp = tmp->next;
    }
    //取前半部分的结点入栈
    int middle = sum / 2;
    int jiOrOu = sum % 2;
    //如果是偶数个
    if (!jiOrOu) {
        stack<ListNode *> result;
        ListNode *tmp2 = head;
        for (int i = 0; i < middle; i++) {
            result.push(tmp2);
            tmp2 = tmp2->next;
        }
        bool t = true;
        while (!result.empty() && tmp2) {
            ListNode *tmp = result.top();
            result.pop();
            if (tmp->val != tmp2->val) {
                t = false;
                break;
            }
            tmp2 = tmp2->next;
        }

        return t;
    } else {  //如果是奇数个
        stack<ListNode *> result;
        ListNode *tmp2 = head;
        for (int i = 0; i < middle; i++) {
            result.push(tmp2);
            tmp2 = tmp2->next;
        }
        bool t = true;
        tmp2 = tmp2->next; //再往后移一个
        while (!result.empty() && tmp2) {
            ListNode *tmp = result.top();
            result.pop();
            if (tmp->val != tmp2->val) {
                t = false;
                break;
            }
            tmp2 = tmp2->next;
        }

        return t;
    }


}


/**
 * 给定一个整数数组 temperatures ，表示每天的温度，返回一个数组 answer ，
 * 其中 answer[i] 是指对于第 i 天，下一个更高温度出现在几天后。如果气温在这之后都不会升高，请在该位置用 0 来代替
 * 输入: temperatures = [73,74,75,71,69,72,76,73]
 * 输出: [1,1,4,2,1,1,0,0]
 * @param temperatures
 * @return
 */
vector<int> dailyTemperatures(vector<int> &temperatures) {
    vector<int> result(temperatures.size());
    //找每一个元素的第一个比它大的数
    //此方法时间复杂度为 o(n2)  可能存在超时的可能
//    for(int i =0; i < temperatures.size() - 1; i ++) {
//        for(int j = i; j < temperatures.size(); j ++) {
//            if (temperatures[j] > temperatures[i]) {
//                result[i] = j -i;
//                break;
//            } else {
//                result[i] = 0;
//            }
//        }
//    }
//    result[temperatures.size() - 1] = 0;


    //方法二
    stack<int> st;  //暂存 每个元素的索引
    for (int i = 0; i < temperatures.size(); i++) {
        int t = temperatures[i];
        //找到第一比 i 位置元素大的数 就停止
        while (!st.empty() && t > temperatures[st.top()]) {
            result[st.top()] = i - st.top();
            st.pop();
        }
        st.push(i);
    }


    for (int i = 0; i < result.size(); i++) {
        printf("700----------- 每日温度值：%d\n", result[i]);
    }


    return result;
}

/**
 * 翻转二叉树 leetcode 226
 * 将每一个结点的  左孩子  与 右孩子  互换
 *
 * @param root
 * @return
 */
TreeNode *invertTree(TreeNode *root) {
    if (!root) {
        return nullptr;
    }
    queue<TreeNode *> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode *tmp = q.front();
        //方法一
//        if(tmp->left) {
//            q.push(tmp->left);
//        }
//        if(tmp->right) {
//            q.push(tmp->right);
//        }
//        TreeNode* tmpleft = tmp->left;
//        tmp->left = tmp->right;
//        tmp->right = tmpleft;
//
//方法二
        if (tmp->left && tmp->right) {

            q.push(tmp->left);
            q.push(tmp->right);
            //将左孩子  摘下
            TreeNode *tmpleft = tmp->left;
            tmp->left = tmp->right;
            tmp->right = tmpleft;

        } else if (tmp->left && !tmp->right) {
            q.push(tmp->left);

            //将左孩子  摘下
            TreeNode *tmpleft = tmp->left;
            tmp->left = nullptr;
            tmp->right = tmpleft;

        } else if (!tmp->left && tmp->right) {
            q.push(tmp->right);

            //将右孩子  摘下
            TreeNode *tmpright = tmp->right;
            tmp->left = tmpright;
            tmp->right = nullptr;
        }
        q.pop();

    }

    printf("翻转完毕\n");
    return root;
}

/**
 * 动态规划 leet code 198
 * 你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，
 * 影响你偷窃的唯一制约因素就是相邻的房屋装有相互连通的防盗系统，如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警。
 * 给定一个代表每个房屋存放金额的非负整数数组，计算你 不触动警报装置的情况下 ，一夜之内能够偷窃到的最高金额。
 * [2,1,1,2]  结果：4
 * @param nums
 * @return
 */
int rob(vector<int> &nums) {
    if (nums.size() == 0) {
        return 0;
    }
    // 子问题：
    // f(k) = 偷 [0..k) 房间中的最大金额

    // f(0) = 0
    // f(1) = nums[0]
    // f(k) = max{ rob(k-1), nums[k-1] + rob(k-2) }

    int N = nums.size();
    vector<int> dp2(N + 1, 0);
    dp2[0] = 0;
    dp2[1] = nums[0];
    for (int k = 2; k <= N; k++) {
        dp2[k] = max(dp2[k - 1], nums[k - 1] + dp2[k - 2]);
    }
    return dp2[N];
}

//动态规划求 台阶问题
void testDP1(int n) {
    // f(n) = f(n-1) + f(n-2);
    vector<int> dp(n + 1, 0);
    if (n == 0) {
        printf("输入台阶数必须大于0\n");
        return;
    }
    if (n == 1) {
        printf("总共有1种走法\n");
        dp[0] = 1;
        return;
    }
    if (n == 2) {
        printf("总共有2种走法\n");
        dp[1] = 2;
        return;
    }
    dp[0] = 0;
    dp[1] = 1;
    dp[2] = 2;
    for (int k = 3; k < n + 1; k++) {
        dp[k] = dp[k - 1] + dp[k - 2];
    }
    printf("878-------总共有 %d 走法\n", dp[n]);
}

/**
 * 给定n阶台阶 ，一次只能走1/2 打印所有种走法
 */
int printStage(int n) {
    if (n < 1) {
        printf("参数报错\n");
        return 0;
    }
    if (n == 1) {
        printf("走法只有1种\n");
        return 1;
    }
    if (n == 2) {
        printf("走法只有1种\n");
        return 2;
    }
    return printStage(n - 1) + printStage(n - 2);
}

/**
 * 动态规划二
 * 给你一个整数数组 nums ，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。
 * 子数组是数组中的一个连续部分
 * @param nums
 * @return
 */
int maxSubArray(vector<int> &nums) {

    vector<int> dp(nums.size());
    dp[0] = nums[0];

    // 状态转移方程
    //dp[i] = max(dp[i-1] + nums[i], nums[i])，即当前位置的最大和要么是之前的最大和加上当前元素，要么是当前元素本身
    for (int i = 1; i < nums.size(); i++) {
        dp[i] = max(dp[i - 1] + nums[i], nums[i]);
    }
    int sum = nums[0];
    for (int i = 0; i < nums.size(); i++) {
        if (sum < dp[i]) {
            sum = dp[i];
        }
    }
    return sum;
}

/**
 * 动态规划 例子
 * 给你一个整数数组 nums ，找到其中最长严格递增子序列的长度
 * 输入：nums = [10,9,2,5,3,7,101,18]
    输出：4
    解释：最长递增子序列是 [2,3,7,101]，因此长度为 4
 * @param nums
 * @return
 */
int lengthOfLIS(vector<int> &nums) {

    int maxLength = 0;
    vector<int> dp(nums.size(), 1);
    for (int i = 0; i < nums.size(); i++) {
        for (int j = 0; j < i; j++) {
            if (nums[j] < nums[i]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
    }
    int sum = nums[0];
    for (int i = 0; i < nums.size(); i++) {
        if (sum < dp[i]) {
            sum = dp[i];
        }
    }
    return sum;
}

/**
 * 动态规划
 * 给你一个字符串 s ，找出其中最长的回文子序列，并返回该序列的长度。
   子序列定义为：不改变剩余字符顺序的情况下，删除某些字符或者不删除任何字符形成的一个序列
   输入：s = "bbbab"
    输出：4
    解释：一个可能的最长回文子序列为 "bbbb" 。
 * @param s
 * @return
 */
int longestPalindromeSubseq(string s) {

    vector<int> dp(s.length(), 0); //记录 回文子串的长度
    //dp[i] 代表 s 中 0-i 子字符串  最大回文串的 长度
    //第一步找出最长的回文子串
    //判断s[i] 中 是否 存在回文子串，先判断本身，再判断s[i-1]

}

/**
 * 746. 使用最小花费爬楼梯
 * 动态规划
 * [1,100,1,1,1,100,1,1,100,1]  结果为 6
 * - 支付 1 ，向上爬两个台阶，到达下标为 2 的台阶。
- 支付 1 ，向上爬两个台阶，到达下标为 4 的台阶。
- 支付 1 ，向上爬两个台阶，到达下标为 6 的台阶。
- 支付 1 ，向上爬一个台阶，到达下标为 7 的台阶。
- 支付 1 ，向上爬两个台阶，到达下标为 9 的台阶。
- 支付 1 ，向上爬一个台阶，到达楼梯顶部。
 * @param cost
 * @return
 */
int minCostClimbingStairs(vector<int> &cost) {

    vector<int> dp(cost.size() + 1, 0);
    //往上走1步  还是2步  取决于 min(cost[i+1], cost[i+2])
    //dp[i] 表示爬第i 层所需要的花销
    //dp[i] = min(dp[i-1],dp[i-2])
    for(int i =2; i < cost.size(); i ++) {
        dp[i] = min(dp[i-1] + cost[i-1],dp[i-2] + cost[i-2]);
    }

    return dp[cost.size()];


}

/**
 * leetcode 62. 不同路径
 * 一个机器人位于一个 m x n 网格的左上角 （起始点在下图中标记为 “Start” ）。

机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为 “Finish” ）。

问总共有多少条不同的路径
 * 动态规划
 * @param m
 * @param n
 * @return
 */
int uniquePaths(int m, int n) {

    //设定1个 二维数组
    vector<vector<int>> dp(m, vector<int>(n, 0));
    //初始值
    for (int i = 0; i < m; i++) { dp[i][0] = 1; }
    for (int j = 0; j < n; j++) { dp[0][j] = 1; }

    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }
    return dp[m - 1][n - 1];

}
/**
 * 63. 不同路径 II
 * 动态规划
 * @param obstacleGrid
 * @return
 */
int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
    int m = obstacleGrid.size();
    int n = obstacleGrid[0].size();
    //设定1个 二维数组
    vector<vector<int>> dp(m, vector<int>(n, 0));
    //初始值
    for (int i = 0; i < m && obstacleGrid[i][0] == 0; i++) dp[i][0] = 1;
    for (int j = 0; j < n && obstacleGrid[0][j] == 0; j++) dp[0][j] = 1;

    if(obstacleGrid[0][0] == 1 || obstacleGrid[m-1][n-1] == 1) {
        return 0;
    }

    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            if (obstacleGrid[i - 1][j] == 0 && obstacleGrid[i][j - 1] == 0) {
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            } else if (obstacleGrid[i - 1][j] == 0) {
                dp[i][j] = dp[i - 1][j];
            } else if (obstacleGrid[i][j - 1] == 0) {
                dp[i][j] = dp[i][j - 1];
            } else {
                continue;
            }
        }
    }
    return dp[m - 1][n - 1];

}

/**
 * 343. 整数拆分
 * 给定一个正整数 n ，将其拆分为 k 个 正整数 的和（ k >= 2 ），并使这些整数的乘积最大化。

返回 你可以获得的最大乘积
 * @param n
 * @return
 */
int integerBreak(int n) {

    vector<int> dp(n+1,0); //初始化 1 表示本身也是一种：1+ 0；
//    5
//    4+1
//    3+2
//    3+1+1
//    2+2+1
//    2+1+1+1
//    1+1+1+1+1
//
//    4
//    3+1
//    2+2
//    2+1+1
//    1+1+1+1
//    dp[i] = max(dp[i], max((i - j) * j, dp[i - j] * j));  对于每个数字i，尝试所有可能的拆分方式j和i-j

    for (int i = 3; i <= n ; i++) {
        for (int j = 1; j < i - 1; j++) {
            dp[i] = max(dp[i], max((i - j) * j, dp[i - j] * j));
        }
    }
    printf("\n1099------------%d  最大的乘积 = %d ",n,dp[n]);
    return dp[n];
}


/**
 * 递归打印 输出正整数的所有分割
 * @param partition
 */
void printPartition(const std::vector<int>& partition) {
    for (size_t i = 0; i < partition.size(); ++i) {
        std::cout << partition[i];
        if (i < partition.size() - 1) {
            std::cout << " + ";
        }
    }
    std::cout << std::endl;
}

/**
 * 递归打印 输出正整数的所有分割
 * @param n
 * @param start
 * @param partition
 */
void findPartitions(int n, int start, std::vector<int>& partition) {
    if (n == 0) { // 当n为0时，找到一种分割方式，输出它
        printPartition(partition);
        return;
    }
    for (int i = start; i >= 1; --i) { // 从start开始递减到1，这样可以避免重复的分割方式（例如，不输出5+1和1+5）
        if (n >= i) { // 如果当前的i不大于n，则可以尝试使用i作为分割的一部分
            partition.push_back(i); // 将i加入到当前分割中
            findPartitions(n - i, i, partition); // 递归寻找剩余部分的分割方式，注意start变为i以避免重复的分割方式（如3+3不被视为3+3和3+3）
            partition.pop_back(); // 回溯，移除最后加入的元素，尝试下一个可能的分割值
        }
    }
}

/**
 * 给定一个只包含正整数的非空数组。是否可以将这个数组分割成两个子集，使得两个子集的元素和相等
 * 输入: [1, 5, 11, 5]
    输出: true
    解释: 数组可以分割成 [1, 5, 5] 和 [11].
 * @param nums
 * @return
 */
bool canPartition(vector<int>& nums) {

    return false;
}

/**
 * 给定一个数组，数组内的元素每次可以选择无限次，求选择的元素累加和为target 的次数
 * 与硬币兑换一样
 * 动态规划
 * @param nums
 * @param target
 * @return
 */
int partitionNums(vector<int>& nums,int target) {

    //每个元素可以选择无限次
//    vector<vector<int>> dp(nums.size(), vector<int>(target + 1, 0));
//    for (int j = 0; j <= target; j++) {
//        if (j % nums[0] == 0) {
//            dp[0][j] = 1;
//        } else {
//            dp[0][j] = 0;
//        }
//    }
//
//    for(int i =0; i < nums.size(); i ++) {
//        dp[i][0] = 1;
//    }
//
//    for(int i = 1; i < nums.size(); i++) { // 遍历科研物品
//        for(int j = 0; j <= target; j++) { // 遍历行李箱容量
//            if (j < nums[i]) dp[i][j] = dp[i - 1][j]; // 如果装不下这个物品,那么就继承dp[i - 1][j]的值
//            else {
//                dp[i][j] = dp[i-1][j] +  dp[i][j - nums[i]];  //不选 + 选
//            }
//        }
//    }
//    return dp[nums.size() - 1][target];

    //每个元素只可以选择1次，或者不选 01背包
    vector<vector<int>> dp(nums.size() + 1, vector<int>(target + 1, 0));
    dp[0][0] = 1;
    printf("\n1201---------初始值\n");
    for (int i = 0; i < nums.size(); i++) {
        for (int j = 0; j <= target; j++) {
            printf("%d ",dp[i][j]);
        }
        printf("\n");
    }
    printf("1201---------初始值 end \n");

    for(int i = 1; i <= nums.size(); i++) { // 遍历科研物品
        for(int j = 0; j <= target; j++) { // 遍历行李箱容量
            if (j >= nums[i-1]) {
                //dp[i - 1][j]表示该物品不放入背包，如果在 [0, i - 1] 这个子区间内已经有一部分元素，使得它们的和为 j ，那么 dp[i][j] = true；
                //dp[i - 1][j - nums[i]]表示该物品放入背包。如果在 [0, i - 1] 这个子区间内就得找到一部分元素，使得它们的和为 j - nums[i]。
                dp[i][j] = dp[i-1][j] + dp[i-1][j - nums[i-1]];
            } else {
                dp[i][j] = dp[i-1][j];
            }
        }
    }
    printf("1220---------每个硬币只能选择一次使用二维数组 总和 = %d  组合数：%d\n",target,dp[nums.size()][target]);
    //return dp[nums.size()][target];



    //一维数组做法
    vector<int> dp2(target + 1, 0);
    dp2[0]  =1;
    for (int coin : nums) {
        // 从后向前遍历，避免重复计算
        for (int j = target; j >= coin; j--) {
            dp2[j] += dp2[j - coin];
        }
    }
    printf("1223---------每个硬币只能选择一次 使用一维数组 总和 = %d  组合数：%d\n",target,dp2[target]);

    return dp2[target];
}

/**
 * 给你一个由 不同 整数组成的数组 nums ，和一个目标整数 target 。请你从 nums 中找出并返回总和为 target 的元素组合的个数
 * 动态规划  完全背包问题
 * @param nums
 * @param target
 * @return
 */
int combinationSum4(vector<int> &nums, int target) {
    int n = nums.size();
    vector<vector<unsigned int>> dp(n, vector<unsigned int>(target + 1, 0));

    // 初始化：用前 i+1 个数字凑出 0 的排列数为 1（空组合）
    for (int i = 0; i < n; ++i) {
        dp[i][0] = 1;
    }

    for (int j = 1; j <= target; ++j) { //先遍历 背包  得到的是排列数
        for (int i = 0; i < n; ++i) {
            if (i > 0) {
                dp[i][j] = dp[i-1][j]; // 不使用 nums[i]
            }
            if (nums[i] <= j) {
                dp[i][j] += dp[n-1][j - nums[i]]; // 使用 nums[i]
            }
        }
    }

    return dp[n-1][target];
}




int main() {

//    removeDuplicat();
//
//
//    queue<int> q2;
//    q2.push(0);
//    q2.push(1);
//    q2.push(2);
//    q2.push(3);
//    for (int n = q2.size(); n--;) {
//        auto node = q2.front();
//        printf("497----------n = %d\n",node);
//        q2.pop();
//
//        q2.push(4);
//        q2.push(5);
//    }
//
//
//    return 0;

//    ListNode *l1 = new ListNode(8);
//    ListNode *l2 = new ListNode(4);
//    ListNode *l3 = new ListNode(3);
//    ListNode *l4 = new ListNode(2);
//    ListNode *l5 = new ListNode(1);
//    l1->next = l2;
//    l2->next = l3;
//    l3->next = l4;
//    l4->next = l5;
//    sortList(l1);



//    ListNode *l12 = new ListNode(1);
//
//    ListNode *l1 = new ListNode(1);
//    ListNode *l2 = new ListNode(2);
//    ListNode *l3 = new ListNode(3);
//    ListNode *l4 = new ListNode(4);
//    ListNode *l5 = new ListNode(5);
//    ListNode *l6 = new ListNode(6);
//    ListNode *l7 = new ListNode(7);
//    l1->next = l2;
//    l2->next = l3;
//    l3->next = l4;
//    l4->next = l5;
//    l5->next = l6;
//    l6->next = l7;
//
//    l12->next = l2;
//
//    ListNode* cha = getIntersectionNode(l1,l12);
//    if(cha) {
//        printf("626--------交叉结点为 %d\n",cha->val);
//    }


//    TreeNode* root = new TreeNode();
//    TreeNode* n1 = new TreeNode(1);
//    TreeNode* n2 = new TreeNode(2);
//    TreeNode* n3 = new TreeNode(3);
//    TreeNode* n4 = new TreeNode(4);
//    TreeNode* n5 = new TreeNode(5);
//    TreeNode* n6 = new TreeNode(6);
//
//    root->left = n1;
//    root->right = n2;
//
//    n1->left = n3;
//    n1->right = n4;
//
//    n2->left = n5;
//    n2->right = n6;
//
//    vector<int> paths;
//    findPath(root,6,paths);
//    for(int i = 0; i < paths.size(); i ++) {
//        printf("683-------va = %d\n",paths[i]);
//    }
//
//    int commone = lowestCommonAncestor(root,n3,n4);
//    printf("706--------%d 与  %d 最近公共 结点为 ：%d\n",n3->val,n4->val,commone);




//    ListNode *l1 = new ListNode(1);
//    ListNode *l2 = new ListNode(2);
//    ListNode *l3 = new ListNode(1);
//    ListNode *l4 = new ListNode(1);
//    l1->next = l2;
//    l2->next = l3;
//    //l3->next = l4;
//
//   bool isP =  isPalindrome(l1);
//    printf("752------是否回文 = %d\n",isP);


//    vector<int> temp = {73,74,75,71,69,72,76,73};
//    dailyTemperatures(temp);


    //翻转2叉树
//    TreeNode *root = new TreeNode();
//    TreeNode *n1 = new TreeNode(1);
//    TreeNode *n2 = new TreeNode(2);
//    TreeNode *n3 = new TreeNode(3);
//    TreeNode *n4 = new TreeNode(4);
//    TreeNode *n5 = new TreeNode(5);
//    TreeNode *n6 = new TreeNode(6);
//
//    root->left = n1;
//    root->right = n2;
//
//    n1->left = n3;
//    n1->right = n4;
//
//    n2->left = n5;
//    n2->right = n6;
//
//    invertTree(root);
//
//
//    string s = "aabaa";
//    partition(s);



//    int array[6] = {1,4,6,2,9,10};
//    findTargetInArray(array,6,10);

//    vector<int> nums = {100, 4, 200, 3, 2, 5};
//    int re = longestConsecutive(nums);
//    printf("60---------re = %d\n", re);
//
//    vector<int> waters = {1, 8, 6, 2, 5, 4, 8, 3, 7};
//    maxArea2(waters);
//    maxArea(waters);

//    vector<int> coins = {2,7,9,3,1};
//    vector<int> coins2 = {2,1,1,2};
//    int maxCoins = rob(coins2);
//    printf("-------最大金额为: %d\n",maxCoins);
//
//    int ss = 6;
//    testDP1(ss);
//    int steps = printStage(ss);
//    printf("1058----总共有 %d 走法\n",steps);


//    vector<int> costs = {0, 0, 0, 1};
//    minCostClimbingStairs(costs);

    std::vector<int> partition; // 用于存储当前的分割结果
    findPartitions(5, 5, partition); // 从n开始递归寻找所有分割方式

    integerBreak(5);
    vector<int> nums = {1,2,3};
    vector<int> nums2 = {1};
    vector<int> nums3 = {1,2,3,4};
    vector<int> nums4 = {1,2,3,4,5,6};  // [1 2 3 5] ,[2,4,5],[1,4,6],[2,3,6],[5,6]
    int p_nums = partitionNums(nums4,11);
    printf("\n1356--------组合的和 %d 总次数 = %d\n",5,p_nums);

    combinationSum4(nums,4);


    return 0;
}