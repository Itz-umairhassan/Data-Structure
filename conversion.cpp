#include<iostream>
#include<stack>
#include<string>
using namespace std;

bool is_operand(char temp) {
	return (temp >= 'A' && temp <= 'Z') || (temp >= 'a' && temp <= 'z') || (temp >= '0' && temp <= '9');
}

bool is_operator(char temp) {
	return (temp == '+' || temp == '/' || temp == '*' || temp == '-' || temp == '^');
}

bool is_opening(char temp) {
	return (temp == '(' || temp == '{' ||  temp == '['  );
}

bool is_closing(char temp) {
	return (temp == ')' || temp == '}' || temp == ']');
}

bool close_match(char temp1, char temp2) {
	return (temp1 == '(' && temp2 == ')') || (temp1 == '{' && temp2 == '}') || (temp1 == '['
		&& temp2 == ']');
}

int perform_op(int num1, int num2, char op) {
	if (op == '+')
		return num2 + num1;
	else if (op == '*')
		return num2 * num1;
	else if (op == '/')
		return num2 / num1;
	else if (op == '-')
		return num2 - num1;
	return -1;
}
int prec(char temp) {
	if (temp == '+' || temp == '-')
		return 1;
	else if (temp == '*' || temp == '/')
		return 2;
	else if (temp == '^')
		return 3;
	return -1;
}

string conversion(string infix) {
	string postfix = "";
	stack<char>st;

	for (int i = 0;i < infix.length();i++) {
		if (is_operand(infix[i]))
			postfix += infix[i];   // append the operand to the expression...
		else if (is_opening(infix[i]))
			st.push(infix[i]);
		else if (is_closing(infix[i])) {
			while (!st.empty() && !close_match(st.top(), infix[i])) {
				postfix += st.top();
				st.pop();
			}
			st.pop();   // in order to pop closing bracket we don't need it in postfix
		}
		
		else if (is_operator(infix[i])) {
			while (!st.empty() && ( (prec(st.top() ) >= prec(infix[i] ) ) ) ) {
				postfix += st.top();
				st.pop();
			}

			st.push(infix[i]);
		}
	}

	while (!st.empty()) {
		postfix += st.top();
		st.pop();
	}
	return postfix;
}


int evaluate(string postfix) {
	stack<int> s;
	
	for (int i = 0;i < postfix.length();i++) {
		if (is_operand(postfix[i]))
			s.push(postfix[i]-'0');
		else if (is_operator(postfix[i])) {
			int num1 = s.top();
			s.pop();
			int num2 = s.top();
			s.pop();
			int num = perform_op(num1, num2, postfix[i]);
			s.push(num);
			
		}
	}

	return s.top();
}

bool paranthesis_match(string expression) {
	stack<char>s;
	bool flag;
	for (int i = 0;i < expression.length();i++) {
		if (is_opening(expression[i]))
			s.push(expression[i]);
		else if (is_closing(expression[i])) {
			if (s.empty() || !close_match(s.top(), expression[i]))
				return false;
			s.pop();
		}

	}
	if (!s.empty())
		return false;
	return true;
}
int main() {
	string eq;
	cout << "enter equation= ";
	cin >> eq;
	/*cout << conversion(eq) << endl;
	string eq2 = conversion(eq);
	cout << eq2 << endl;
	cout << evaluate(eq2);*/
	cout << paranthesis_match(eq);
	return 0;
}