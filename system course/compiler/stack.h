#pragma once
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <Windows.h>

struct node {
	int val;
	struct node* next;
}*top = NULL;

void push(int val) {
	struct node* temp = (struct node*)malloc(sizeof(struct node));
	temp->val = val;
	temp->next = top;
	top = temp;
}

int is_empty() {
	return ((top == NULL) ? 1 : 0);
}

int pop() {
	if (is_empty()) {
		printf("Stack Underflow.");
		return INT_MIN;
	}
	struct node* temp = top;
	int buff = temp->val;
	top = top->next;
	temp->next = NULL;
	free(temp);
	return buff;
}

int peek() {
	if (is_empty()) {
		printf("Stack Underflow.");
		return INT_MIN;
	}
	int buff = top->val;
	return buff;
}

void clear_stack() {
	while (pop() != INT_MIN);
}
