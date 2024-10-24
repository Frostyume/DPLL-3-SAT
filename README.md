# 基于DPLL算法解决3-SAT问题

### 问题描述 
给定m个涉及到n个boolean变量的表达式，找出如何对这几个变量赋值，让这m个boolean表达式同时为true。这种问题的答案不一定唯一
例子:给定下列4个boolean表达式，如何去变量的值同时使这些表达式的答案为 true:
$$X_1 \lor \neg X_2 \lor \neg X_4$$
$$X_2 \lor \neg X_3 \lor X_4$$
$$X_2 \lor X_3 \lor X_4$$
$$X_2 \lor X_3 \lor \neg X_4$$
