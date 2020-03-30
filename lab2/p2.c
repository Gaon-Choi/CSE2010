#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	int id;
	char* name;
} student;

typedef struct Node_ {
	student stu;
	struct Node_* next;
} Node;

Node* make_node() {
	Node* node = malloc(sizeof(Node));
	node->stu.id = 0;
	node->stu.name = NULL;
	node->next = NULL;
	return node;
}

int is_blank(Node* list) {
	return list->next == NULL;
}

int check_exist(Node* list, int id_) {
	if (!is_blank(list)) {
		list = list->next;
		while (list != NULL) {
			if (list->stu.id == id_)
				return 1;
			list = list->next;
		}
		return 0;
	}
	else
		return 0;
}

void insert_student(Node* list, student stu_) {
	Node* node_ = make_node();
	node_->stu = stu_;
	node_->next = list->next;
	list->next = node_;
}

Node* find_student(Node* list, int student_id) {
	while (list != NULL) {
		if (list->stu.id == student_id)
			return list;
		list = list->next;
	}
	return NULL;
}

Node* find_previous(Node* list, int student_id) {
	Node* pos = list;
	while (pos->next != NULL && pos->next->stu.id != student_id)
		pos = pos->next;
	return pos;
}

void print_students(Node* list, FILE* output) {
	fprintf(output, "-----LIST-----\n");
	while (list != NULL) {
		fprintf(output, "%d %s\n", list->stu.id, list->stu.name);
		list = list->next;
	}
	fprintf(output, "--------------\n");
}

void print_students_2(Node* list, FILE* output)
{
	fprintf(output, "Current List > ");
	while (list != NULL) {
		fprintf(output, "%d %s", list->stu.id, list->stu.name);
		if (list->next != NULL)
			fprintf(output, "-");
		list = list->next;
	}
	fprintf(output, "\n");
}

Node* inserting_point(Node* list, int id_) {
	if (list == NULL || list->stu.id > id_)
		return NULL;

	Node* prev = list;
	list = list->next;

	while (list != NULL) {
		if (list->stu.id > id_)
			break;
		prev = list;
		list = list->next;
	}
	return prev;
}


void insert(Node* list, FILE* input, FILE* output) {
	int id_;
	char first_[100] = { 0, };
	char second_[100] = { 0, };
	fscanf(input, "%d %s %s", &id_, first_, second_);
	if (check_exist(list, id_) == 1) {
		fprintf(output, "Insertion Failed. ID %d already exists.\n", id_);
		return;
	}

	strcat(first_, " ");
	strcat(first_, second_);
	int size = strlen(first_);
	student newbee;
	newbee.id = id_;
	newbee.name = malloc(sizeof(char) * size);
	strcpy(newbee.name, first_);
	Node* position = inserting_point(list->next, id_);
	if (position == NULL)
		insert_student(list, newbee);
	else
		insert_student(position, newbee);
	fprintf(output, "Insertion Success : %d\n", id_);
	print_students_2(list->next, output);
}

void delete (Node* list, FILE* input, FILE* output) {
	int id_;
	fscanf(input, "%d", &id_);
	if (find_student(list->next, id_) == NULL)
		fprintf(output, "Deletion Failed : Student ID %d is not in the list.\n", id_);
	else {
		Node* p = find_previous(list, id_);
		if (p->next != NULL) {
			Node* tmp = p->next;
			p->next = tmp->next;
			free(tmp);
		}
		fprintf(output, "Deletion Success : %d\n", id_);
		print_students_2(list->next, output);
	}
}

void find(Node* list, FILE* input, FILE* output) {
	list = list->next; // assuming head
	int id_;
	fscanf(input, "%d", &id_);
	Node* found = find_student(list, id_);
	if (found == NULL)
		fprintf(output, "Find %d Failed. There is no student ID\n", id_);
	else
		fprintf(output, "Find Success : %d %s\n", found->stu.id, found->stu.name);
}

int main(void) {
	char opt;
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");
	Node* student_list = make_node();

	while (fscanf(input, "%c", &opt) == 1) {
		switch (opt) {
		case 'i':
			insert(student_list, input, output);
			break;
		case 'd':
			delete (student_list, input, output);
			break;
		case 'f':
			find(student_list, input, output);
			break;
		case 'p':
			print_students(student_list->next, output);
			break;
		default:
			break;
		}
	}
	Node* node = student_list->next;
	while (node != NULL) {
		free(node->stu.name);
		node = node->next;
	}
	while (student_list != NULL) {
		Node* node = student_list->next;
		free(student_list);
		student_list = node;
	}

	fclose(input);
	fclose(output);
	return 0;
}
