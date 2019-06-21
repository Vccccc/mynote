# LeetCode原题
### Single Number
Description:
Given an array of integers, every element appears twice except for one. Find that single one.
Note: Your algorithm should have a linear runtime complexity. Could you implement it without using
extra memory?

分析：
元素出现偶数次，表示某个bit出现偶数次。比如数字2出现偶数次，表示在1位和0位，10出现偶数次。那么异或运算下来