# 迷你正则引擎

一个支持并、或、可莉闭包的迷你正则引擎

## 环境

- gcc
- graphviz

## 功能

- 正则表达式转NFA
- 模拟NFA匹配字符串
- NFA转DFA
- DFA转最小DFA

## 示例

$(a|b)^*abb$

### NFA

![dfa](D:\school_task\compiler\token\extra\reg\pic\nfa.png)

### DFA

![dfa](D:\school_task\compiler\token\extra\reg\pic\dfa.png)

### min_DFA

![min_dfa](D:\school_task\compiler\token\extra\reg\pic\min_dfa.png)