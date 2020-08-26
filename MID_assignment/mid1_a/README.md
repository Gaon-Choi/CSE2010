# BST with duplicate keys

BST에서 duplicate key를 허용하려고 한다. 아래의 각 경우에 대해 Insert, Delete, Find 함수를 구현하시오.

**Parameter 제약 조건**

- BST의 input parameter K는 int형으로 key값이 입력된다.

- K의 범위는 1 이상, 10000 이하이다.

- K의 개수는 1 이상, 1000 이하이다.


**문제 A**

가장 간단하게는 각 node에 duplicate key의 count를 나타내는 field를 추가할 수 있다. Count field를 유지할 수 있도록 각 함수(Insert, Delete, Find)를 구현하시오.
