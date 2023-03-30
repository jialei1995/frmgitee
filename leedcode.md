### 层次遍历树：

#### c++

```c++
vector<int> levelOrder(TreeNode* root) {
	queue<TreeNode*> queue;
	vector<int> res;
	if(root==NULL)return res;//若传入NULL，直接返回空vector
	queue.push(root);
	while(!queue.empty()){
		TreeNode *node=queue.front();//临时变量储存当前需要处理的节点
		queue.pop();				//出队列 的同时，将当前出队列的元素的左右孩子都入队列
		res.push_back(node->val);	//结果保存在vector
		if(node->left){
			queue.push(node->left);
		}
		if(node->right){
			queue.push(node->right);
		}
	}
	return res;
}
```

#### python实现：

```py
def levelOrder( root: TreeNode) -> List[int]:     
	if not root: return []
	res = []
	queue = []
	queue.append(root)
	while queue:
		node = queue.pop(0)    #python中的list无法从头部删除元素，错误。python中的pop可以控制从左边删除还是右边删除。pop(0)就是从左边删除
		res.append(node.val)
		if node.left: queue.append(node.left)
		if node.right: queue.append(node.right)
	return res
	'''以下才是可以成功的代码，用到了collections.deque()。
	if not root: return []
	res, queue = [], collections.deque()
	queue.append(root)
	while queue:
		node = queue.popleft()
		res.append(node.val)
		if node.left: queue.append(node.left)
		if node.right: queue.append(node.right)
	return res
	'''
```


​	
###  寻找相交链表的第一个焦点的位置：
太他妈巧妙了，两个指针一个a,一个b。过程就是a将heada遍历一边然后遍历headb，b将headb遍历一边遍历heada。若有焦点就会退出循环，若没有焦点
最终a,b都会指向null。因为a与b走的路程是一样的。每次循环2者都在移动。

这个有个bug，若给定的不是相交链表，两个长度也不一样，程序就会死循环出不来？？  错，程序没bug，遍历走的总路程一样，若没有焦点，最终肯定指向NULL

```python
def getIntersectionNode(self, headA: ListNode, headB: ListNode) -> ListNode:  太他妈巧妙了
	a = headA
	b = headB
	while(a != b):
		a = a.next if a else headB
		b = b.next if b else headA
	return a
```

### 自制的升序排序好的栈：

```c
#include <stack>
class SortedStack {
public:利用一个辅助栈
    stack<int> stack1;
    stack<int> stack2;//临时用一下  最终的数据往stack1 放
    
void push(int val) {
    while(!stack1.empty() && val > stack1.top()){   若栈1不是空的，看新加的val是不是最大值，不是最大值，先将栈1的大于val的数字
        stack2.push(stack1.top());					放到栈2，再放val到栈1.  val放进栈1后再将栈2里面的数据放回栈1时用一下
        stack1.pop();								
     }
    stack1.push(val);
    while(!stack2.empty()){     //弹出临时栈里面的所有值 放回栈1
        stack1.push(stack2.top());
        stack2.pop();
     }
}
```



### 删除未排序链表中的重复节点，保留前一个节点组成新链表,在原来的链表上改变指针指向。

```c++
ListNode* removeDuplicateNodes(ListNode* head) {
    if(head == NULL) return head;
    set<int> set;
    ListNode*tail = head;
    set.insert(head->val);
    ListNode*tmp = head->next;
    while(tmp){
        if(set.find(tmp->val) == set.end()){如果遍历到的tmp点不在set中，则这点是需要的。
            set.insert(tmp->val);
            tail->next = tmp;
            tail = tmp;
            tmp = tmp->next;  先让tmp指向下个节点
            tail->next = NULL;	再将准备返回的节点尾部指向空，不然会丢失原链表的后续节点
         }else{
            tmp = tmp->next;  若当前tmp已经在set中，则继续向后遍历
        }
    }
    return head;
}
```



### 深度优先搜索：dfs
解题模板
深度优先搜索(DFS)和广度优先搜索(BFS)解决，BFS较DFS繁琐
#### 自顶而下：dfs
##### 一般路径：

```c+
vector<vector<int>>res;
void dfs(TreeNode*root,vector<int> & path)    path最终返回的数据就是本函数执行深度优先查找的过程。
{
    if(!root) return;  //根节点为空直接返回
    path.push_back(root->val);  //作出选择
    if(!root->left && !root->right) //如果到叶节点  
    {
        res.push_back(path);
        return;
    }
    dfs(root->left,path);  //继续递归
    dfs(root->right,path);
}
```

##### 给定和的路径：到叶子节点和刚好是sum

```c
void dfs(TreeNode*root, int sum, vector<int> path)
{
    if (!root)
        return;
    sum -= root->val;
    path.push_back(root->val);
    if (!root->left && !root->right && sum == 0)
    {
        res.push_back(path);
        return;
    }
    dfs(root->left, sum, path);
    dfs(root->right, sum, path);
}
```



#### 非自顶而下：
这类题目一般解题思路如下：
设计一个辅助函数maxpath，调用自身求出以一个节点为根节点的左侧最长路径left和右侧最长路径right，那么经过该节点的最长路径就是left+right
接着只需要从根节点开始dfs,不断比较更新全局变量即可

int res=0;
int maxPath(TreeNode *root) //以root为路径起始点的最长路径
{
    if (!root)
        return 0;
    int left=maxPath(root->left);
    int right=maxPath(root->right);
    res = max(res, left + right + root->val); //更新全局变量  
    return max(left, right);   //返回左右路径较长者
}
这类题型DFS注意点：
1、left,right代表的含义要根据题目所求设置，比如最长路径、最大路径和等等

2、全局变量res的初值设置是0还是INT_MIN要看题目节点是否存在负值,如果存在就用INT_MIN，否则就是0

3、注意两点之间路径为1，因此一个点是不能构成路径的

题目分析
下面是对具体题目的分析和代码呈现
一、自顶向下
257. 二叉树的所有路径
直接套用模板1即可，注意把"->"放在递归调用中  res保存所有的dfs的路径
void dfs(TreeNode*root, string path)
{

    if (!root)
        return;
    path += to_string(root->val);
    if (!root->left && !root->right)
    {
        res.push_back(path);
        return;
    }
    dfs(root->left, path+"->");
    dfs(root->right, path+"->");
}
113. 路径总和 II
直接套用模板2

vector<vector<int>> res;
vector<vector<int>> pathSum(TreeNode *root, int targetSum)
{
    vector<int> path;
    dfs(root, targetSum, path);
    return res;
}

void dfs(TreeNode*root, int sum, vector<int> path)
{
    if (!root)
        return;
    sum -= root->val;
    path.push_back(root->val);
    if (!root->left && !root->right && sum == 0)
    {
        res.push_back(path);
        return;
    }
    dfs(root->left, sum, path);
    dfs(root->right, sum, path);
}
437. 路径总和 III
双重递归：先调用dfs函数从root开始查找路径，再调用pathsum函数到root左右子树开始查找
套用模板2


int count = 0;
int pathSum(TreeNode *root, int targetSum)
{
    if (!root)
        return 0;
    dfs1(root, targetSum);            //以root为起始点查找路径
    pathSum(root->left, targetSum);  //左子树递归
    pathSum(root->right, targetSum); //右子树递归
    return count;
}

void dfs(TreeNode *root, int sum)
{
    if (!root)
        return;
    sum -= root->val;
    if (sum == 0) //注意不要return,因为不要求到叶节点结束,所以一条路径下面还可能有另一条
        count++;  //如果找到了一个路径全局变量就+1
    dfs1(root->left, sum);
    dfs1(root->right, sum);
}
988. 从叶结点开始的最小字符串
换汤不换药，套用模板1


vector<string> path;
string smallestFromLeaf(TreeNode *root)
{
    dfs(root, "");
    sort(path.begin(), path.end()); //升序排序
    return path[0];
}

void dfs(TreeNode *root, string s)
{
    if (!root)
        return;
    s += 'a' + root->val;
    if (!root->left && !root->right)
    {
        reverse(s.begin(), s.end()); //题目要求从根节点到叶节点，因此反转
        path.push_back(s);
        return;
    }
    dfs(root->left, s);
    dfs(root->right, s);
}
二、非自顶向下
124. 二叉树中的最大路径和
/left,right分别为根节点左右子树最大路径和,注意：如果最大路径和<0,意味着该路径和对总路径和做负贡献，因此不要计入到总路径中，将它设置为0


int res = INT_MIN; //注意节点值可能为负数，因此要设置为最小值
int maxPathSum(TreeNode *root)
{
    maxPath(root);
    return res;
}

int maxPath(TreeNode *root) //以root为路径起始点的最长路径
{
    if (!root)
        return 0;
    int left = max(maxPath(root->left), 0);
    int right = max(maxPath(root->right), 0);
    res = max(res, left + right + root->val);  //比较当前最大路径和与左右子树最长路径加上根节点值的较大值，更新全局变量
    return max(left + root->val, right + root->val); //返回左右子树较长的路径加上根节点值
}
687. 最长同值路径


int longestUnivaluePath(TreeNode *root)
{
    if (!root)
        return 0;
    longestPath(root);
    return res;
}

int longestPath(TreeNode *root)
{
    if (!root)
        return 0;
    int left = longestPath(root->left), right = longestPath(root->right);
    // 如果存在左子节点和根节点同值，更新左最长路径;否则左最长路径为0
    if (root->left && root->val == root->left->val)
        left++;
    else
        left = 0;
    if (root->right && root->val == root->right->val)
        right++;
    else
        right = 0;
    res = max(res, left + right);
    return max(left, right);
}
543. 二叉树的直径


int res1 = 0;  
int diameterOfBinaryTree(TreeNode *root)
{
    maxPath(root);
    return res1;
}

int maxPath(TreeNode *root)
{
// 这里递归结束条件要特别注意：不能是!root(而且不需要判断root为空,因为只有非空才会进入递归)，因为单个节点路径长也是0
    if (!root->left && !root->right)  
        return 0;
    int left = root->left ? maxPath(root->left) + 1 : 0;  //判断左子节点是否为空，从而更新左边最长路径
    int right = root->right ? maxPath(root->right) + 1 : 0;
    res1 = max(res, left + right); //更新全局变量
    return max(left, right);  //返回左右路径较大者
}


生成对应数量的左右括号：python解法
def generateParenthesis(self, n: int) -> List[str]:
	res=[]
	def dfs(  left,  right,  curStr):
		if (left == 0 and  right == 0):  #左右括号都不剩余了，说明找到了有效的括号
			res.append(curStr)
			return

		if (left < 0): #左括号只有剩余的时候才可以选，如果左括号的数量已经选完了，是不能再选左括号了。
			return
		if (right < left):  #说明右括号选的比左括号多了，这个函数中选哪个括号则哪个括号的入参就会减小1
			return
		#选择左括号
		dfs( left - 1, right, curStr + "(")
		#选择右括号
		dfs( left, right - 1, curStr + ")")
	dfs(n,n,"")  #内部定义的dfs函数在generateParenthesis函数中得调用一次 在dfs中可以访问外部函数定义的变量，这个方法可以修改原函数入参。
					# fun([],"")空列表，空字符串也可以直接当作入参当作临时变量去调用
	return res

class Solution { c++写法
public:
    vector<string> res;
    void dfs(int left,int right,string curStr){
        if (left == 0 &&  right == 0){
            res.push_back(curStr);
			return;
        }
		if (left < 0)return;
		if (right < left)return;
		dfs( left - 1, right, curStr + "(");
		dfs( left, right - 1, curStr + ")");
    }
    vector<string> generateParenthesis(int n) {
        dfs(n,n,"");
        return res;
    }
};

判断括号字符串是否有效：
bool ismatch(char left,char right){  判断是否匹配函数
    switch(right){
        case ')':
            return left=='(';
        case ']':
            return left=='[';
        case '}':
            return left=='{';
        default:
            return false;
    }
}

bool isvalid(string s){
    if(s.length()==0)return true;
    stack<char> stack;
    for(auto ch :s){   遍历传入的string
        if(ch==')' || ch==']'||ch=='}'){   如果当前char是右括号
            if(stack.empty() || !ismatch(stack.top(),ch) ){  判断栈定是否有匹配的左括号
                return false;
            }else{
                stack.pop();
            }
        }else{					如果当前char是左括号直接入栈
            stack.push(ch);
        }
    }
    return stack.empty();
}


-------------分治法求最长子序列
int MaxSubseqSum(int a[],int left,int right)//传入数组，头位置，尾位置
{
    int maxLeftSum,maxRightSum,maxMidSum;
    int maxLeftBorderSum,LeftBorderSum;
    int maxRightBorderSum,RightBorderSum;
    int mid;
    int i;
    if(left==right) //递归出口，子序列只有一个元素时
        return a[left];
    mid=(left+right)/2; //求中间位置
    maxLeftSum=MaxSubseqSum(a,left,mid);    //求左边序列的最大子序列和
    maxRightSum=MaxSubseqSum(a,mid+1,right);    //求右边序列的最大子序列和
    /*求中间跨左边与右边时的最大序列和，maxMidSum*/
    maxLeftBorderSum=0;
    LeftBorderSum=0;
    for(i=mid;i>=left;i--)  //从中间位置向左找靠边界的最大子序列，初始化maxLeftBorderSum=0，如果左边加起来都小于0，则maxLeftBorderSum不变还是0
    {
        LeftBorderSum+=a[i];
        if(LeftBorderSum>maxLeftBorderSum)
            maxLeftBorderSum=LeftBorderSum;
    }
    maxRightBorderSum=0;
    RightBorderSum=0;
    for(i=mid+1;i<=right;i++)   //从中间位置向右找靠边界的最大子序列，初始化maxRightBorderSum=0，如果右边加起来都小于0，则maxRightBorderSum不变还是0
    {
        RightBorderSum+=a[i];
        if(RightBorderSum>maxRightBorderSum)
            maxRightBorderSum=RightBorderSum;
    }
    maxMidSum=maxLeftBorderSum+maxRightBorderSum;   //得到处在中间位置上的最大子序列和
    return max3(maxLeftSum,maxRightSum,maxMidSum);
}


int maxbuf(int *buf,int len)
{
    int summax=0;
    int curmax=0;//当前子序列总和
    for(int i=0;i<len;i++)
    {
        curmax += buf[i];
        if(curmax>summax)//更新最大子序列和
        {
            summax = curmax;
        }else if(curmax<0)//<0，则重新开始累加
        {
            curmax=0;
        }
    }
    return summax;
}
----------------------------------递归转动态 规划----用已知计算未知
1.	青蛙跳台阶问题：
	int numWays(int n) {//递归解法
		if(n==1)
		{
			return 1;
		}else if(n==2)
		{
			return 2;
		}else
		{
			return numWays(n-1)+numWays(n-2);
		}
	}
	优化动态规划：新建哈希map，循环给hash表赋值。
	int numWays(int n) {
	   f[0]=f[1]=1;//设计初始状态
	   for(int i=2;i<=n;i++)
	   {
		   f[i]=(f[i-1]+f[i-2]);//确定状态方程
	   }
	   return f[n];//返回最终状态
	}
2.  给你一个非负整数 num ，请你返回将它变成 0 所需要的步数。 如果当前数字是偶数，你需要把它除以 2 ；否则，减去 1 。
	int numberOfSteps(int num) {
        f[0]=0;
        f[1]=1;
        for(int i=2;i<=num;i++)
        {
            if(i%2==0)//偶数
            {
                f[i]=f[i/2]+1;
            }else
            {
                f[i]=f[i-1]+1;
            }
        }
        return f[num];
    }
3.  爬楼梯的最少成本
	int minCostClimbingStairs(vector<int>& cost) {
		f[0]=f[1]=0;//要上第0或者第1阶台阶不需要花费体力
		int n=cost.size();//最后肯定是要爬上第N阶的
	    for(int i=2;i<=n;i++)
	    {
		    f[i]=min(f[i-1]+cost[i-1],f[i-2]+cost[i-2]);//确定状态方程，要上第2阶要么
																					从第0阶跳+0阶跳需要的体力
																					从第1阶跳+1阶跳需要的体力
	    }
	    return f[n];
    }
4.	打家劫舍------

	贪心思想做这个题：[5 2 6 3 1 7]---先挑最大的7 ，不能挑1 在其他位置挑最大的6，不能挑2 3. 再去其他位置挑最大的5 不能挑2了。所有的位置都不能挑了。返回相加结果
							这种方法是错的，贪心的思想就是每次求最大的值---再去其他值中找次大的值---每次都是找最的。
							
	int rob(vector<int>& nums) {
        f[0] = nums[0];//第0位置，能偷最大数就是nums0
        int n=nums.size();
        for(int i=1;i<n;i++)
        {
            if(i==1)//第一位置能偷的最大数是nums0与nums1的最大值
            {
                f[i]=max(f[i-1],nums[i]);
            }else
            {
                f[i]=max(f[i-2]+nums[i],f[i-1]);//第i位置能偷的最大值
            }
        }
        return f[n-1];
    }
5.	哈希表，求数组中只出现一次的数字的和   给定的数组范围小就可以用hash表
	int qiusum(int * arr,int size)
	{
		int sum=0;
		for(int i=0;i<size;i++)//遍历所给数组，将对应的hash表对应下标的数值++
		{
			hash[arr[i]]++;
		}
		for(int j=0;j<size;j++)//遍历hash，如果数值为1，累加到sum，否则跳过
		{
			if(hash[j]==1)
			{
				sum+=j;
			}
		}
		return sum;
	}
6.	N皇后问题
	class Solution {
    int hash[10][10];//>0表示该位置不能放置皇后
    int cnt;

    void doAdd(int r,int c,int n,int val)
    {
        if(r<0 || r>=n)return;
        if(c<0||c>=n)return;
        hash[r][c] += val;
    }
    void add(int r,int c,int n,int val)//val=1放置  val=-1取出，取出的时候将对应位置标记的数字都-1
    {
        //放置一个皇后需要将她所在的行列对角线标记
        int i=0;
        for(i=0;i<n;i++)
        {
            doAdd(r,i,n,val);//标记行
            doAdd(i,c,n,val);//标记列
        }
        for(i=0;i<n;i++)
        {
            doAdd(r+i,c+i,n,val);
            doAdd(r-i,c-i,n,val);//标记主对角线
            doAdd(r+i,c-i,n,val);
            doAdd(r-i,c+i,n,val);//标记副对角线
        }
    }

    void dfs(int depth,int maxDepth)//当前要放的行，总共要放的行
    {
        int i;
        if(depth==maxDepth)//最后一行都可以放，就成功了一种方法
        {
            ++cnt;
            return;
        }
        for(i=0;i<maxDepth;++i)//第一行每个位置都能直接放 i表示往哪一列放
        {
            if(hash[depth][i]==0)//没皇后能攻击到，第一行可放的位置0,0  0,1  0,2  0...
            {
                add(depth,i,maxDepth,1);//放置皇后
                dfs(depth+1,maxDepth);//放下一行往哪里放
                add(depth,i,maxDepth,-1);//取出来
            }
        }
    }
public:
    int totalNQueens(int n) {
        cnt=0;
        memset(hash,0,sizeof(hash));
        dfs(0,n);
        return cnt;
    }
};

7.求数组中连续子序列的最大的和---递推，动态规划问题
设计dp[i]表示以arr[i]结尾的最大子序列和，dp[i]并不一定是前i个数中的最大子序列和，只是为了让dp[i]--->dp[i+1]之间产生联系。
实际返回结果的时候返回的是什么呢？实际返回值
int maxsubarr()
{
    int maxres=dp[0];
    int size=sizeof(array)/sizeof(*array);
    for(int i=1;i<size;i++)
    {
            if(dp[i-1]>=0)
            {
                    dp[i] = dp[i-1]+array[i];
            }else
            {
                    dp[i] = array[i];
            }
            if(maxres<dp[i])//实际返回值
            {
                    maxres = dp[i];
            }
            printf("dp[%d]=%d,maxres=%d\n",i,dp[i],maxres);
    }
    return maxres;
}

113. 路径总和 II
给你二叉树的根节点 root 和一个整数目标和 targetSum ，找出所有 从根节点到叶子节点 路径总和等于给定目标和的路径。
程序运行的过程中path中的数据会一直变化   ret中的数据只会一直增加 不会渐少
方法1：
class Solution {
public:

    void dfs(TreeNode* root, int targetSum,vector<vector<int>>&ret,vector<int> &path) {
        if (root == nullptr)    return;
        path.push_back(root->val);
        targetSum -= root->val;
        //如果当前targetsum=0 并且当前节点是叶子节点  吧path中的数据全放到re中
        if (root->left == nullptr && root->right == nullptr && targetSum == 0) {
            ret.push_back(path);
        }
        //继续往target逼近 检查左右节点
        dfs(root->left, targetSum,ret,path);
        dfs(root->right, targetSum,ret,path);      
        path.pop_back();//弹出
    }
    
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        vector<vector<int>> ret;
        vector<int> path;//深度遍历时的路径
        dfs(root, targetSum,ret, path);//把返回值当作入参传入
        return ret;
    }
};

方法2：
class Solution {
public:
    vector<vector<int>> ret;//作为全局var去 使用
    vector<int> path;
    void dfs(TreeNode* root, int targetSum) {
        if (root == nullptr)   return;
        path.push_back(root->val);
        targetSum -= root->val;
        //如果当前targetsum=0 并且当前节点是叶子节点  吧path中的数据全放到re中
        if (root->left == nullptr && root->right == nullptr && targetSum == 0) {
            ret.push_back(path);
        }
        
            dfs(root->left, targetSum);
            dfs(root->right, targetSum);      
        path.pop_back();
    }
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        dfs(root, targetSum);
        return ret;
    }
};

回溯，深度优先遍历：46 全排列
https://leetcode.cn/problems/permutations/solutions/9914/hui-su-suan-fa-python-dai-ma-java-dai-ma-by-liweiw/
vector<vector<int>> res;
vector<int> path;

//要遍历的数组  大小  深度   哪些位置已使用 哪些位置未使用
void dfs(vector<int> nums,int size,int depth,vector<bool> used)
{
	if(depth==size)
	{
		res.push_back(path);
		return;
	}
	for(int i=0;i<size;i++)//压入path的数可以是nums的任意一个
	{
		if(used[i]==false)
		{
			used[i]=true;//一旦压入 则记个标记  下次不许在压入此位置数据
			path.push_back(nums[i]);
			dfs(nums,size,depth+1,used);//遍历的深度++
			used[i]=false;
			path.pop_back();
		}
	}
}
vector<vector<int>> permute(vector<int>& nums) {
	int size=nums.size();
	if(size==0)return res;
	vector<bool> used(size,false);//判断当前位置的数据有没有使用 初始化都没使用
	dfs(nums,size,0,used);
	return res;
}

40. 组合总和 II
class Solution {
private:
    vector<int> candidates;
    vector<vector<int>> res;
    vector<int> path;
public:
    void DFS(int start, int target) {
        if (target == 0) {
            res.push_back(path);
            return;
        }
        //所有数组中的元素 都要往path中压一次 
        for (int i = start; i < candidates.size() && target - candidates[i] >= 0; i++) {
            //path中压入的元素 不能重复 使用----是数组中的下标为n的元素只能选择一次
            if (i > start && candidates[i] == candidates[i - 1])
                continue;
            path.push_back(candidates[i]);
            // 元素不可重复利用，使用下一个即i+1
            DFS(i + 1, target - candidates[i]);
            path.pop_back();
        }
    }

    vector<vector<int>> combinationSum2(vector<int> &candidates, int target) {
        sort(candidates.begin(), candidates.end());
        this->candidates = candidates;//排序后的入参赋值给类内变量
        DFS(0, target);
        return res;
    }
};


循环打印矩阵：
const int dir[4][2]={{0,1},{1,0},{0,-1},{-1,0}};//表示4个方向dir[x] x表示方向右下左上
vector<int> spiralOrder(vector<vector<int>>& matrix) {
	
	if(matrix.size()==0 || matrix[0].size()==0)
		return {};
	int rownum=matrix.size();
	int colnum=matrix[0].size();
	vector<int> retorder(rownum*colnum);
	vector<vector<bool>> used(rownum,vector<bool>(colnum,false));//为false表示当前元素还未访问  可以访问
	int row=0,col=0;
	int i=0;
	int dirinex=0;
	for(i=0;i<rownum*colnum;i++)
	{
		retorder[i]=(matrix[row][col]);//访问过的元素used对应位置复制true表示之后不能访问
		used[row][col]=true;
		int nextrow=row+dir[dirinex][0],nextcol=col+dir[dirinex][1];//临时变量计算将要访问的位置能不能访问
		if(nextrow<0||nextrow>=rownum||nextcol<0||nextcol>=colnum||used[nextrow][nextcol]==true)
		{
			dirinex=(dirinex+1)%4;//若不能访问，说明方向需要
		}
		row=row+dir[dirinex][0];
		col=col+dir[dirinex][1];
	}
	
	return retorder;
}
	
返回空vector数组方法：return {}；
若传进来空二维数组怎么判断：
不能int row=arr.size();  int col=arr[0].size() ---因为数组为空  没有[0]号元素
在访问0号元素之前就需要判空：
if(matrix.size()==0 || matrix[0].size()==0)    ||之前如果arr为空，则不会走到||之后  就不会出错
            return {};

如果stack是空的，则不能通过stack.top() 去访问栈顶元素
要访问栈顶元素，需要(!stack.empty())  ---才能访问

队列里面存的一对一对的<int,int>  怎么直接接收一组元素：auto [x, y] = Q.front(); 直接给x y 赋值


面试题13. 机器人的运动范围
class Solution {
// 计算 x 的数位之和
    int get(int x) {
        int res=0;
        for (; x; x /= 10) {
            res += x % 10;
        }
        return res;
    }
public:
    int movingCount(int m, int n, int k) {
        if (!k) return 1;
        queue<pair<int,int> > Q;
        // 向右和向下的方向数组
        int dx[2] = {0, 1};
        int dy[2] = {1, 0};
        vector<vector<int> > vis(m, vector<int>(n, 0));//是否访问过
        Q.push(make_pair(0, 0));//压入原点坐标
        vis[0][0] = 1;//标记远点被访问
        int ans = 1;//只有远点能被访问 则初始化为1
        //弹出一个点  压入弹出点 右边或下面的点
        while (!Q.empty()) {
            auto [x, y] = Q.front();//直接获取 某组元素的横纵坐标
            Q.pop();
            for (int i = 0; i < 2; ++i) {
                int tx = dx[i] + x;//i=0 表示分析下面的点  i=1表示分析右边的点
                int ty = dy[i] + y;
                //准备访问的点是否可以访问 不能越界  访问过的不能访问  相加之和<k 才能 push到队列，否则contoinue
                if (tx < 0 || tx >= m || ty < 0 || ty >= n || vis[tx][ty] || get(tx) + get(ty) > k) 
                    continue;
                Q.push(make_pair(tx, ty));
                vis[tx][ty] = 1;
                ans++;//可以访问的点++
            }
        }
        return ans;
    }
};
python实现
class Solution:
    def movingCount(self, m: int, n: int, k: int) -> int:
        def get(x):
            ret=0;
			while(x):
				ret=ret+x%10
				x=x//10     x/10不能获取到十位部分，需要//两个/才可以 取整数
			return ret
        Q=[]
        dx=[0,1]
        dy=[1,0]
        vis=[[0 for i in range(n)]for i in range(m)]
        ans=1;
        Q.append([0,0])

        while(len(Q)!=0):
            x,y=Q[0]
            Q.pop(0) #python中无队列,栈的容器，用list都可以实现  队列pop的时候需要pop(0) 栈pop的时候直接pop()即可 
            for i in range(2):
                tx=dx[i]+x  #计算要到达的点 坐标
                ty=dy[i]+y
                if(tx<0 or tx>=m or ty<0 or ty>=n or vis[tx][ty] or get(tx)+get(ty)>k ): 无法到达直接continue
                    continue
                Q.append([tx,ty])  #可以达到 先压入队列  再更新flag
                vis[tx][ty]=1
                ans=ans+1
        return ans   

面试题14- I. 剪绳子  纯数学问题 代码不难


合并两个排序的链表：
 ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode * l3=new ListNode(-1);//必须给头节点申请空间，否则不能直接给它赋值 这个是头节点 不是首节点
        ListNode * tmp;
        tmp=l3;
        while(l1 && l2)
        {
            if(l1->val <= l2->val)
            {
                tmp->next=l1;//这里就是直接给自己申请的头节点赋值 的操作，若头节点为空 是不能直接赋值的
                    l1=l1->next;
            }else
            {
                tmp->next=l2;
                    l2=l2->next;
            }
            tmp=tmp->next;
        }
        if(l1)
            tmp->next=l1;
        else if(l2)
            tmp->next=l2;
        return l3->next;
    }
	//递归实现  开始必须判空 否则不能直接对比l1的val与l2的val 会出错的
	 public ListNode mergeTwoLists(ListNode l1, ListNode l2) {
        if(l1 == null) return l2;
        if(l2 == null) return l1;
        if(l1.val <= l2.val) {
            l1.next = mergeTwoLists(l1.next, l2);
            return l1;
        } else {
            l2.next = mergeTwoLists(l1, l2.next);
            return l2;
        }
    }
	
在排序数组中查找元素的第一个和最后一个位置:不知道怎么判断选取边界条件时：可以用flag的方法去判断
vector<int> searchRange(vector<int>& nums, int target) {
	int size=nums.size();
	int flag=0;
			vector<int> ret(2);
			if(size==0)
				return {-1,-1};
			if(size==1)
			{
				if(target==nums[0])
				{
					return {0,0};
				}
			}
			int head=0,tail=size-1;
			int mid=0;
			while(head<=tail)
			{
				mid=(head+tail)/2;
				if(target > nums[mid])
				{
					head=mid+1;
				}else if(target<nums[mid])
				{
					tail=mid-1;
				}else
				{
					flag=1;//找到了
					break;
				}
			}//找一下当前数组中是否有target元素
			if(flag==0)//没找到 
			{
				return {-1,-1};
			}
			head=mid;tail=mid;
			while(head>=0&&nums[head]==target)
			{
				head--;
			}
			while(tail<size && nums[tail]==target)
			{
				tail++;
			}
			ret[0]=head+1;
			ret[1]=tail-1;
			return ret;
}


322. 零钱兑换  动态规划求解
int dp[1000];
int coinChange(vector<int>& coins, int amount) {
	int Max = amount + 1;
	vector<int> dp(amount + 1, Max);
	dp[0] = 0;
	//动态规划求解
	//依次求  dp[0---amount] 后面的数都由前面的数而来
	for (int i = 1; i <= amount; ++i) {
		for (int j = 0; j < (int)coins.size(); ++j) {
			if (coins[j] <= i) {  //当前要计算的总金额 i 必须大于coins[j]才去计算，否则就会越界报错
				dp[i] = min(dp[i], dp[i - coins[j]] + 1);//dp[i]为 dp[i-coins[j(0~coin.size)]] 之间的最小值
			}
		}
	}
	return dp[amount] > amount ? -1 : dp[amount];
}



剑指 Offer 45. 把数组排成最小的数（自定义排序，清晰图解）
def minNumber(self, nums: List[int]) -> str:
	def quick_sort(l , r):
		if l >= r: return
		i, j = l, r
		while i < j:
			while strs[j] + strs[l] >= strs[l] + strs[j] and i < j: j -= 1
			while strs[i] + strs[l] <= strs[l] + strs[i] and i < j: i += 1
			strs[i], strs[j] = strs[j], strs[i] #交换strs的i j 位置的数据
		strs[i], strs[l] = strs[l], strs[i]
		quick_sort(l, i - 1)
		quick_sort(i + 1, r)
	
	strs = [str(num) for num in nums]  将数字数组转化成字符串数组，对字符串数组按照特定的排序规则排序
	quick_sort(0, len(strs) - 1)
	return ''.join(strs)   拼接排序后的数组

函数中定义的局部strs在子函数中可以直接使用，不需要传参就可以


岛屿问题：
岛屿的最大面积：
class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int res = 0;
        for (int r = 0; r < grid.size(); r++) {
            for (int c = 0; c < grid[0].size(); c++) {
                if (grid[r][c] == 1) {//若当前位置为岛屿 则求面积
                    int a = area(grid, r, c);
                    res = max(res, a);
                }
            }
        }
        return res;
    }
    //深度优先遍历
    int area(vector<vector<int>>& grid, int r, int c) {
        if (!inArea(grid, r, c)) { //当前位置超出数组接线  返回0  无新增面积
            return 0;
        }
        if (grid[r][c] != 1) { //当前位置为海 或者 当前位置已经遍历过  无新增面积
            return 0;
        }
        grid[r][c] = 2;//加标志 表示当前位置已经遍历过了
        return 1 
            + area(grid, r - 1, c)
            + area(grid, r + 1, c)
            + area(grid, r, c - 1)
            + area(grid, r, c + 1);//返回当前面积1+上下左右岛面积
    }

    bool inArea(vector<vector<int>>& grid, int r, int c) {
        return 0 <= r && r < grid.size() 
                && 0 <= c && c < grid[0].size();
    }

};

岛屿的周长：
class Solution {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        int res=0;
        for (int r = 0; r < grid.size(); r++) {
            for (int c = 0; c < grid[0].size(); c++) {
                if (grid[r][c] == 1) {//若当前位置为岛屿 则求周长
                    return  area(grid, r,c);
                }
            }
        }
        return res;
    }
    //深度优先遍历
    int area(vector<vector<int>>& grid, int r, int c) {
        if (!inArea(grid, r, c)) { //当前位置超出数组接线  返回1  临界边 新增周长为1 
            return 1;
        }
        if (grid[r][c] ==0)//当前方向是水 新增周长1
        {
           return 1;
        }

        if (grid[r][c] != 1) { //已经遍历过
            return 0;
        }
        //后面的表示当前位置为1  可以上下左右扩散
        grid[r][c] = 2;//加标志 表示当前位置已经遍历过了
        return area(grid, r - 1, c)
            + area(grid, r + 1, c)
            + area(grid, r, c - 1)
            + area(grid, r, c + 1);//返回当前面积1+上下左右岛面积
    }
    
    bool inArea(vector<vector<int>>& grid, int r, int c) {
        return 0 <= r && r < grid.size() 
                && 0 <= c && c < grid[0].size();
    }

};


岛屿数量：
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int res = 0;
        for (int r = 0; r < grid.size(); r++) {
            for (int c = 0; c < grid[0].size(); c++) {
                if (grid[r][c] == '1') {//若当前位置为岛屿 则求面积
                    res+=1;
                    area(grid, r, c);
                }
            }
        }
        return res;
    }
    //深度优先遍历  把遍历过的岛屿置'0'，否则害怕把同一个岛屿上多个1重新计算
    void area(vector<vector<char>>& grid, int r, int c) {
        if (!inArea(grid, r, c)) { //当前位置超出数组接线  返回0  无新增面积
            return;
        }
        if(grid[r][c] == '0')
            return;
        grid[r][c] = '0';//加标志 表示当前位置已经遍历过了
        
        area(grid, r - 1, c);
        area(grid, r + 1, c);
        area(grid, r, c - 1);
        area(grid, r, c + 1);//返回当前面积1+上下左右岛面积
        return;
    }
    
    bool inArea(vector<vector<char>>& grid, int r, int c) {
        return 0 <= r && r < grid.size() 
                && 0 <= c && c < grid[0].size();
    }
};

class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int res = 0;
        for (int r = 0; r < grid.size(); r++) {
            for (int c = 0; c < grid[0].size(); c++) {
                if (grid[r][c] == '1') {//若当前位置为岛屿 则求面积
                    res+=1;
                    area(grid, r, c);
                }
            }
        }
        return res;
    }
    //深度优先遍历
    void area(vector<vector<char>>& grid, int r, int c) {
        if (!inArea(grid, r, c)) { //当前位置超出数组接线  返回0  无新增面积
            return;
        }
        if(grid[r][c] == '0')
            return;
        if(grid[r][c] == '2')
        {
            return;
        }
        grid[r][c] = '2';//加标志 表示当前位置已经遍历过了
        
        area(grid, r - 1, c);
        area(grid, r + 1, c);
        area(grid, r, c - 1);
        area(grid, r, c + 1);//返回当前面积1+上下左右岛面积
        return;
    }
    
    bool inArea(vector<vector<char>>& grid, int r, int c) {
        return 0 <= r && r < grid.size() 
                && 0 <= c && c < grid[0].size();
    }
};

38. 外观数列
string countAndSay(int n) { //用前一个值推理下一个值
	vector<string> dfs(n+1);
	dfs[1] = "1";
	for (int i = 2; i <= n; ++i) {
		int start = 0;
		int pos = 0;
		while (pos < dfs[i-1].size()) {
			while (pos < dfs[i-1].size() && dfs[i-1][pos] == dfs[i-1][start]) {
				pos++;
			}
			dfs[i] += to_string(pos - start) + dfs[i-1][start];
			start = pos;
		}
	}
	
	return dfs[n];
}

while(条件1 && 条件2) 与 while(条件2 && 条件1) 不一样，因为&&符号的特殊性，前者不成立后者就不会执行
while(条件1 || 条件2) 与 while(条件2 || 条件1) 不一样，因为&&符号的特殊性，前者成立后者就不会执行

对于需要访问数组下标的条件：
while(pos<len(dfs[i-1]) and dfs[i-1][start]==dfs[i-1][pos] ): 若数组越界就不会访问下标
while(dfs[i-1][start]==dfs[i-1][pos] and pos<len(dfs[i-1])):  程序会异常，因为会先访问越界下标 才会

79. 单词搜索
class Solution {
public:
    bool check(vector<vector<char>>& board, vector<vector<int>>& visited, int i, int j, string& s, int k) {
        if (board[i][j] != s[k]) {//当前位置不符 直接退出
            return false;
        } else if (k == s.length() - 1) { //word遍历到最后一位了说明包含此单词
            return true;
        }
        visited[i][j] = true;//遍历过某个位置  置true
        vector<pair<int, int>> directions{{0, 1}, {0, -1}, {1, 0}, {-1, 0}};//变量方向
        bool result = false;
		//4个方向都要遍历
        for (const auto& dir: directions) {
            int newi = i + dir.first, newj = j + dir.second;//计划要遍历的点坐标
			//若点坐标在board范围内 并且没有被遍历过  则递归处理新的点坐标 否则不做处理
            if (newi >= 0 && newi < board.size() && newj >= 0 && newj < board[0].size()) {
                if (!visited[newi][newj]) {//新点没被遍历过
                    bool flag = check(board, visited, newi, newj, s, k + 1);
                    if (flag) {
                        result = true;  //若存在直接break;
                        break;
                    }
                }
            }
        }
        visited[i][j] = false;//回溯法精髓  --- 用完恢复
        return result;
    }

    bool exist(vector<vector<char>>& board, string word) { //一个一个点作为初始点 去遍历 board
        int h = board.size(), w = board[0].size();
        vector<vector<int>> visited(h, vector<int>(w));
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                bool flag = check(board, visited, i, j, word, 0);//从word的0位置开始  board的i j位置开始
                if (flag) {
                    return true;
                }
            }
        }
        return false;
    }
};

def exist(self, board: List[List[str]], word: str) -> bool:
	def check(board,i,j,word,pos,visited):
		if(board[i][j]!=word[pos]):
			return False
		elif(pos==len(word)-1):
			return True
		dirs=[[0,1],[0,-1],[1,0],[-1,0]]
		visited[i][j]=True
		for dir in dirs:
			newi=i+dir[0]
			newj=j+dir[1]
			if(newi>=0 and newi<len(board) and newj>=0 and newj<len(board[0]) and visited[newi][newj]==False):
				ret=check(board,newi,newj,word,pos+1,visited)
				if(ret==True):
					return True
		visited[i][j]=False #for循环完了在恢复--原因：4个方向都不对，当前节点要退回去  从下一个节点去当作头处理
		return False
			

	visited = [[False for i in range(len(board[0]))] for i in range(len(board))]  
	for i in range(len(board)):
		for j in range(len(board[0])):
			ret=check(board,i,j,word,0,visited)
			if(ret==True):
				return True;
	return False


​	
108. 将有序数组转换为二叉搜索树
class Solution {
public:

    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return helper(nums, 0, nums.size() - 1);
    }
    
    TreeNode* helper(vector<int>& nums, int left, int right) {
        if (left > right) {
            return nullptr;
        }
        //总是选择中间位置左边的数字作为根节点
        int mid = (left + right) / 2;
    
        TreeNode* root = new TreeNode(nums[mid]);  //构造根节点
        root->left = helper(nums, left, mid - 1);	//构造左子树
        root->right = helper(nums, mid + 1, right); //构造右子树
        return root;
    }
};


128. 最长连续序列
int longestConsecutive(vector<int>& nums) {
	set<int> num_set;
	for (int num : nums) {
		num_set.insert(num);
	}//set 去重

	int maxlen = 0;
	for (int num : num_set) {
		//count  返回 num的个数 因为set会去重 只会返回0/1
		if (!num_set.count(num - 1)) 当前位置数字 前面的数字找不到 ，则以当前位置为头 开始往后找最长序列
		{
			int currentNum = num;
			int currentStreak = 1;

			while (num_set.count(currentNum + 1)) {//当前位置数字的 +1能找到
				currentNum += 1;
				currentStreak += 1;
			}
		
			maxlen = max(maxlen, currentStreak);//更新
		}else
		{
		//当前位置不属于头 没必要去计算最长长度
		}
	}

	return maxlen;   
	

def longestConsecutive(self, nums: List[int]) -> int:
	num_set = set(nums)  python中list转set 方法，直接set(list)即可直接转化

	maxlen=0
	for num in num_set:  
		if(num-1 not in num_set):  判断某个元素不在set中
			currentNum=num
			currentlen=1
			while(currentNum+1 in num_set): 判断某个元素是否在set中
				currentNum+=1
				currentlen+=1
			maxlen=max(currentlen,maxlen)  
	return maxlen





### 多项式加 乘计算
//设计存储结构体

```c
typedef struct{
    int cnt;//多项式有几项
    int array[100];//每项两个数字--系数+幂
}Polynomial;
```



//读入多项式

```c
Polynomial ReadPoly()
{
    Polynomial input;
    memset(input.array,0,100);
    int cnt;
    int xishu,mi;
    int i=0;
    cin>>cnt;//多项式中分式个数
    input.cnt = cnt;
    while(cnt--)
    {
        cin>>xishu>>mi;
        input.array[i] = xishu;
        input.array[i+1] = mi;
        i+=2;
    }
    return input;
}
```


//多项式相乘

```c
Polynomial Mult(Polynomial p1,Polynomial p2)
{
    Polynomial mul;
    memset(mul.array,0,100);
    mul.cnt=0;
    Polynomial tmpmul;//存储当前p1中此项*p2中所有项的结果
    int index=0;
    for(int p1head=0;p1head<p1.cnt;p1head++)
    {
        index=0;
        tmpmul.cnt=0;
        memset(tmpmul.array,0,100);
        for(int p2head=0;p2head<p2.cnt;p2head++)
        {
            tmpmul.array[index++]=p1.array[(p1head*2)+0]* p2.array[(p2head*2)+0];//系数相乘
            tmpmul.array[index++]=p1.array[(p1head*2)+1]+ p2.array[(p2head*2)+1];//指数相加
            tmpmul.cnt++;
        }
        mul = Add(mul,tmpmul);//每次算完将结果ADD到返回值中
    }
    return mul;
}
```



多项式相加

```c
Polynomial Add(Polynomial p1,Polynomial p2)
{
    int p1head=0,p2head=0;
    Polynomial sum;
    memset(sum.array,0,100);
    sum.cnt=0;
    int sumindex=0;
    while(p1head<p1.cnt && p2head<p2.cnt)//相加的时候按照幂从高到低扫描
    {
        if(p1.array[(p1head*2)+1] > p2.array[(p2head*2)+1])
        {
            sum.array[sumindex++]=p1.array[(p1head*2)+0];
            sum.array[sumindex++]=p1.array[(p1head*2)+1];
            p1head++;
        }else if(p1.array[(p1head*2)+1] < p2.array[(p2head*2)+1])
        {
            sum.array[sumindex++]=p2.array[(p2head*2)+0];
            sum.array[sumindex++]=p2.array[(p2head*2)+1];
            p2head++;
        }else//相同幂的系数相加，指针都向后偏移
        {
            sum.array[sumindex++]=p2.array[(p2head*2)+0]+p1.array[(p1head*2)+0];
            sum.array[sumindex++]=p2.array[(p2head*2)+1];
            p2head++;
            p1head++;
        }
        sum.cnt++;
    }
    if(p1head == p1.cnt)//说明p2还有数据
    {
        while(p2head<p2.cnt)
        {
            sum.array[sumindex++]=p2.array[(p2head*2)+0];
            sum.array[sumindex++]=p2.array[(p2head*2)+1];
            p2head++;
            sum.cnt++;
        }
    }
    if(p2head == p2.cnt)//说明p1还有数据
    {
        while(p1head<p1.cnt)
        {
            sum.array[sumindex++]=p1.array[(p1head*2)+0];
            sum.array[sumindex++]=p1.array[(p1head*2)+1];
            p1head++;
            sum.cnt++;
        }
    }
    return sum;
}
```



遍历多项式

```c
void PrintPoly(Polynomial pp)
{
    cout<<"sum cnt="<<pp.cnt<<endl;
    for(int i=0;i<pp.cnt;i++)
    {
        cout<<pp.array[i*2+0]<<" "<<pp.array[i*2+1]<<"---";
    }
    cout<<endl;
}
```



```c
int main()
{
    Polynomial P1,P2,PP,PS;
    P1=ReadPoly();
    P2=ReadPoly();
    PS=Add(P1,P2);
    PrintPoly(PS);

    PP=Mult(P1,P2);
    PrintPoly(PP);
    return 0;

}
```


### 哨兵

在数组边界建立哨兵：
例如：在数组中查询是否存在K
正常的做法:
for(i=0;i<size;i++)
{
	是否当前位置==K;
	返回当前位置；
}
建立哨兵的做法：
给数组0位置赋值K
array[0]=K;
for(i=size-1;array[i]!=K;i--)
{
	return i;
}
这样做肯定有返回值，如果返回值是0，说明数组1~size-1位置都没有K



### 利用栈中序遍历二叉树：由两种遍历序列确定二叉树的时候必须有中序序列才可以。

```c
vector<int> inorderTraversal(TreeNode* root) {
    vector<int> res;
    stack<TreeNode*> stk;
    while (root != nullptr || !stk.empty()) {
        while (root != nullptr) {  //将所有的左子节点压栈
            stk.push(root);
            root = root->left;
        }
        root = stk.top();  //出栈最上面的节点 即最左边 的节点
        stk.pop();
        res.push_back(root->val);
        root = root->right;//当前的遍历节点指向 右子树，看右面的节点是否需要压栈
    }
    return res;
}
```




### 剑指 Offer 50. 第一个只出现一次的字符

```c
#include<map>
class Solution {
public:
    char firstUniqChar(string s) {
        map<int, int> frequency;
        for (char ch: s) {
            ++frequency[ch];//先遍历str将每个字母的数量记下来
        }
        for (int i = 0; i < s.size(); ++i) {//按照str中字母的顺序去遍历map
            if (frequency[s[i]] == 1) {
                return s[i];
            }
        }
		for(char m :s)
        {
            if(frequency[m]==1)   return m;
        }
		return ' ';
	}
};
```



1. 两数之和
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> hashtable;
        for (int i = 0; i < nums.size(); ++i) {
            auto it = hashtable.find(target - nums[i]);
            if (it != hashtable.end()) {
                return {it->second, i};
            }
            hashtable[nums[i]] = i;
        }
        return {};
    }
};

func twoSum(nums []int, target int) []int {
    //hashTable := map[int]int{}  这样初始化也可以
    hashTable := make(map[int]int)
    for i, x := range nums {
        //if p, ok := hashTable[target-x]; ok {  查找是否有key为 target-x 的节点 相当于
        p,ok:=mymap[target-val]
        if ok{
            return []int{p, i} //
        }
        hashTable[x] = i  //存的数据为数组当前位置的值 ： 当前位置索引
    }
    return nil
}


9. 回文数
 func isPalindrome(x int) bool {
    tmp:=x
    cur:=0
    if(tmp<0){  负数不可能是回文数
        return false
    }
    for{
        if(tmp!=0){   一位一位的获取最低位的数字 放到cur中，最后用cur与给的x对比是否一样  
            cur=cur*10+tmp%10
            tmp/=10
        }else{
            break
        }
    }
    return cur==x
}



14. 最长公共前缀   
将两个字符串的字串与第三个字符串再求字串，最后的字串再与第4个求子串
```go
func min(x,y int)int{
    if x<y {
        return x
    }
    return y
}

func issub(str1,str2 string)string{
    leng:=min(len(str1),len(str2))
    index:=0
    for(index<leng && str1[index]==str2[index]){
        index++
    }
    return str1[:index] //获取两个字符串的 最长字串
}

func longestCommonPrefix(strs []string) string {
    size:=len(strs)
    prefix:=strs[0]
    for i:=1;i<size;i++ {  //不能带括号 带括号就会出错
        prefix=issub(prefix,strs[i])
        if(len(prefix)==0){
            return ""
        }
    }
    return prefix
}
```

20. 有效的括号
```go
func isValid(s string) bool {
mymap := map[int]int{//通过右括号  可以匹配到左括号，因为压入的左括号
        ')':'(',        
        ']':'[',
        '}':'{',
    }
    stack := []byte{}
    for i:=0;i<len(s);i++ {
        if s[i]==byte('(') || s[i]==byte('[') || s[i]==byte('{'){
            stack=append(stack,s[i])
        }else{
            if len(stack)==0 {
                return false
            }
            if(stack[len(stack)-1]==byte(mymap[int(s[i])])){ //计算是否相等时 判断 栈顶元素 是否与遍历到的元素作为键对应的值相等
                stack=stack[:(len(stack)-1)]
            }else{
                return false
            }
        }
    }
    if(len(stack)==0){  //最后需要保证stack是空的才可以
        return true
    }else{
        return false
    }
}
```
或者把map改成先
```go
func isValid(s string) bool {
    mymap := map[int]int{
        '(':')',
        '[':']',
        '{':'}',
    }
    stack := []byte{}
    for i:=0;i<len(s);i++ {
        if s[i]==byte('(') || s[i]==byte('[') || s[i]==byte('{'){
            stack=append(stack,s[i])
        }else{
            if len(stack)==0 {
                return false
            }
            if(mymap[int(stack[len(stack)-1])]==int(s[i])){ // 计算是否相等时 判断 栈顶元素作为键 对应的值是否与遍历到的元素一致
                stack=stack[:(len(stack)-1)]
            }else{
                return false
            }
        }
    }
    if(len(stack)==0){
        return true
    }else{
        return false
    }
}
```