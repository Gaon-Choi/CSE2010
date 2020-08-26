# BST with duplicate keys

BST에서 duplicate key를 허용하려고 한다. 아래의 각 경우에 대해 Insert, Delete, Find 함수를 구현하시오.

**Parameter 제약 조건**

- BST의 input parameter K는 int형으로 key값이 입력된다.

- K의 범위는 1 이상, 10000 이하이다.

- K의 개수는 1 이상, 1000 이하이다.


**문제 B_b**

BST에 사용되는 key는 같아도 각각 서로 다른 노드로 구별되어야 하는 경우가 있을 수 있다. 이럴 경우,

(b) Duplicate Key가 insert되면 무조건 left subtree에 추가하는 것으로 약속한다고 할 때 각 함수(Insert, Delete, Find)를 구현하시오.
