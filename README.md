# 基于DPLL算法解决3-SAT问题

### 问题描述 
给定m个涉及到n个boolean变量的表达式，找出如何对这几个变量赋值，让这m个boolean表达式同时为true。这种问题的答案不一定唯一
例子:给定下列4个boolean表达式，如何去变量的值同时使这些表达式的答案为 true:

$$X_1 \cup \overline{X_2} \cup \overline{X_4}$$ 
$$X_2 \cup \overline{X_3} \cup X_4$$
$$X_2 \cup X_3 \cup X_4$$
$$X_2 \cup X_3 \cup \overline{X_4}$$

答案一: $$X_1$$, $$X_2$$ 为true, $$X_3$$, $$X_4$$ 自由选取。
答案二: $$X_1$$, $$X_3$$ 任意, $$X_2$$ 为true, $$X_4$$ 为false。
