#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <ctime>

using namespace std;

typedef vector<int> Clause; // 定义子句类型为整数向量
typedef vector<Clause> Formula; // 定义公式类型为子句向量
unordered_map<int, int> frequency; // 用于跟踪变量的赋值频率

bool DPLL(Formula &formula, vector<int> &assignment, unordered_set<int> &unassigned);
bool unitPropagation(Formula &formula, vector<int> &assignment, unordered_set<int> &unassigned, vector<int> &implicitAssignments);
bool isSatisfied(const Formula &formula, const vector<int> &assignment);
int selectUnassignedVariable(const unordered_set<int> &unassigned);
void printAssignment(const vector<int> &assignment);
void updateFrequency(const vector<int> &assignment);
void undoImplicitAssignments(vector<int> &localImplicitAssignments, vector<int> &assignment, unordered_set<int> &unassigned);
void undoExplicitAssignments(vector<int> &localExplicitAssignments, vector<int> &assignment, unordered_set<int> &unassigned);

int main() {
    // 打开测试数据文件，需要修改为自己的文件路径
    ifstream infile("D:/Desktop/testData5.txt");

    if (!infile.is_open()) { // 检查文件是否成功打开
        cout << "无法打开文件" << endl;
        return 1; // 如果打开失败，返回错误代码
    }

    int n, m; // 定义变量数量和表达式数量
    infile >> n >> m; // 从文件读取变量数和子句数

    Formula formula(m); // 初始化公式，包含m个子句
    unordered_set<int> unassigned; // 用于跟踪未赋值的变量集合
    vector<int> assignment(n + 1, -1); // 初始化赋值向量，-1表示未赋值

    // 从文件中读取每个子句
    for (int i = 0; i < m; i++) {
        int a, b, c; // 子句中的三个文字
        infile >> a >> b >> c; // 读取子句
        formula[i] = {a, b, c}; // 存储子句到公式中
        // 将每个文字的绝对值添加到未赋值集合中
        unassigned.insert(abs(a));
        unassigned.insert(abs(b));
        unassigned.insert(abs(c));
    }

    infile.close(); // 关闭文件

    // 记录程序开始时间
    clock_t start = clock();

    // 调用DPLL算法尝试求解
    if (DPLL(formula, assignment, unassigned)) {
        // 如果找到解，输出结果
        cout << "找到一个解为：";
        for (int i = 1; i <= n; ++i) {
            cout <<" "<< (assignment[i] == 1 ? "1" : "0") ; // 输出每个变量的值
        }
        cout << endl;
    } else {
        printAssignment(assignment); // 输出当前赋值状态
        cout << "无解" << endl; // 如果没有解，输出无解信息
    }

    // 记录程序结束时间
    clock_t end = clock();
    // 计算并输出程序运行时间
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "程序运行时间为：" << duration << "秒" << std::endl;
    return 0; // 返回成功
}

// DPLL算法的实现
bool DPLL(Formula &formula, vector<int> &assignment, unordered_set<int> &unassigned) {
    // 局部隐式赋值栈和显式赋值栈
    vector<int> localImplicitAssignments; // 用于存储隐式赋值
    vector<int> localExplicitAssignments; // 用于存储显式赋值

    // 执行单位传播以简化公式
    if (!unitPropagation(formula, assignment, unassigned, localImplicitAssignments)) {
        // 如果单位传播失败，撤销隐式赋值并返回无解
        undoImplicitAssignments(localImplicitAssignments, assignment, unassigned);
        return false;
    }

    // 检查当前赋值是否满足公式
    if (isSatisfied(formula, assignment)) {
        return true; // 如果满足，返回成功
    }

    // 检查是否还有未赋值的变量
    if (unassigned.empty()) {
        return false; // 如果没有未赋值的变量且未满足公式，返回无解
    }

    // 选择一个未赋值的变量进行赋值
    int var = selectUnassignedVariable(unassigned);
    localExplicitAssignments.push_back(var); // 记录显式赋值
    unassigned.erase(var); // 从未赋值集合中移除该变量

    // 尝试将变量赋值为1
    assignment[var] = 1;
    updateFrequency(assignment); // 更新变量频率
    // cout << "尝试赋值: " << var << " = 1" << endl; // 可选调试信息

    // 递归调用DPLL检查该赋值是否有效
    if (DPLL(formula, assignment, unassigned)) {
        return true; // 如果递归调用成功，返回成功
    }

    // 回溯，撤销当前层的隐式赋值
    undoImplicitAssignments(localImplicitAssignments, assignment, unassigned);

    // 尝试将变量赋值为0
    assignment[var] = 0;
    updateFrequency(assignment); // 再次更新频率
    // cout << "尝试赋值: " << var << " = 0" << endl; // 可选调试信息

    // 再次递归调用DPLL
    if (DPLL(formula, assignment, unassigned)) {
        return true; // 如果调用成功，返回成功
    }

    // 如果两次赋值均失败，撤销当前变量的所有赋值
    undoImplicitAssignments(localImplicitAssignments, assignment, unassigned);
    undoExplicitAssignments(localExplicitAssignments, assignment, unassigned);

    return false; // 如果找不到解，返回无解
}

// 撤销隐式赋值的函数
void undoImplicitAssignments(vector<int> &localImplicitAssignments, vector<int> &assignment, unordered_set<int> &unassigned) {
    for (int var : localImplicitAssignments) {
        assignment[var] = -1; // 撤销隐式赋值
        unassigned.insert(var); // 恢复为未赋值状态
    }
    localImplicitAssignments.clear(); // 清空当前层次的隐式赋值栈
}

// 撤销显式赋值的函数
void undoExplicitAssignments(vector<int> &localExplicitAssignments, vector<int> &assignment, unordered_set<int> &unassigned) {
    for (int var : localExplicitAssignments) {
        assignment[var] = -1; // 撤销显式赋值
        unassigned.insert(var); // 恢复为未赋值状态
    }
    localExplicitAssignments.clear(); // 清空当前层次的显式赋值栈
}

// 单位传播函数
bool unitPropagation(Formula &formula, vector<int> &assignment, unordered_set<int> &unassigned, vector<int> &localImplicitAssignments) {
    bool changed = true; // 标志，指示是否发生变化
    while (changed) {
        changed = false; // 重置标志
        for (const auto &clause : formula) {
            int unassignedCount = 0; // 统计未赋值变量的数量
            int lastUnassignedVar = 0; // 记录最后一个未赋值变量的文字
            bool clauseSatisfied = false; // 标志，指示子句是否满足

            for (int lit : clause) {
                int var = abs(lit); // 获取变量的绝对值
                int value = (lit > 0) ? 1 : 0; // 确定当前文字的期望值

                if (assignment[var] == -1) { // 如果变量未赋值
                    unassignedCount++; // 增加未赋值计数
                    lastUnassignedVar = lit; // 更新最后一个未赋值的文字
                } else if (assignment[var] == value) { // 如果当前文字满足子句
                    clauseSatisfied = true; // 标记子句为满足
                    break; // 不再检查其他文字
                }
            }

            // 如果子句不满足且只有一个未赋值的变量
            if (!clauseSatisfied && unassignedCount == 1) {
                // 强制将该变量赋值
                int var = abs(lastUnassignedVar); // 获取变量
                assignment[var] = (lastUnassignedVar > 0) ? 1 : 0; // 根据文字的正负赋值
                unassigned.erase(var); // 从未赋值集合中移除该变量
                localImplicitAssignments.push_back(var); // 记录隐式赋值
                updateFrequency(assignment); // 更新变量频率
                // cout << "单位传播: 赋值 " << var << " = " << (lastUnassignedVar > 0 ? "1" : "0") << endl; // 可选调试信息
                changed = true; // 标记发生了变化
            }

            // 如果子句不满足且没有未赋值变量，返回冲突
            if (!clauseSatisfied && unassignedCount == 0) {
                // cout << "子句不满足且无未赋值变量，存在冲突"<< endl; // 可选调试信息
                return false; // 返回冲突
            }
        }
    }
    return true; // 返回成功
}

// 检查公式是否满足的函数
bool isSatisfied(const Formula &formula, const vector<int> &assignment) {
    for (const auto &clause : formula) {
        bool clauseSatisfied = false; // 标记子句是否满足
        for (int lit : clause) {
            int var = abs(lit); // 获取变量的绝对值
            int value = (lit > 0) ? 1 : 0; // 确定当前文字的期望值
            if (assignment[var] == value) { // 检查该文字是否满足子句
                clauseSatisfied = true; // 标记子句为满足
                break; // 不再检查其他文字
            }
        }
        // 如果有任何一个子句不满足，返回false
        if (!clauseSatisfied) return false;
    }
    return true; // 所有子句均满足，返回true
}

// 选择未赋值变量的函数
int selectUnassignedVariable(const unordered_set<int> &unassigned) {
    int bestVar = -1; // 存储最佳变量
    int maxFreq = -1; // 存储最大频率

    // 遍历未赋值变量集合，选择频率最高的变量
    for (int var : unassigned) {
        if (frequency[var] > maxFreq) {
            maxFreq = frequency[var]; // 更新最大频率
            bestVar = var; // 更新最佳变量
        }
    }

    return bestVar; // 返回频率最高的未赋值变量
}

// 更新变量频率的函数
void updateFrequency(const vector<int> &assignment) {
    for (size_t i = 1; i < assignment.size(); ++i) {
        if (assignment[i] != -1) { // 如果变量已赋值
            frequency[i]++; // 增加其频率
        }
    }
}

// 打印当前赋值状态的函数
void printAssignment(const vector<int> &assignment) {
    cout << "当前赋值状态: ";
    for (size_t i = 1; i < assignment.size(); ++i) {
        // 输出变量的状态，未赋值、1或0
        cout << (assignment[i] == -1 ? "未赋值" : (assignment[i] == 1 ? "1" : "0")) << " ";
    }
    cout << endl; // 换行
}
