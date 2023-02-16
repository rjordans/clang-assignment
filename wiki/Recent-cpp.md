## Fluency in a reasonably recent version of C++.
- I suggest reading about the Standard Template Library: 
>[Cpp STL Tutorial](https://www.geeksforgeeks.org/cpp-stl-tutorial/)
- Read about smart pointers
>[Smart Pointers in C++ and How to Use Them](https://www.geeksforgeeks.org/smart-pointers-cpp/)
- You will need to know basic concepts such as:
    - polymorphism (virtual methods and virtual methods override)
    - design patterns and, in particular, the visitor pattern
    - functors
    - lambda functions
    - templates
        - specialization, tag dispatch and SFINAE
        - Curiously Recurring Template Pattern (CRTP)

The series of videos [here](https://www.youtube.com/playlist?list=PLAe2BCBg8rkIicrIjA6LyjiMn8GFaS6pJ) gives a tremendous hands-on explanation of these concepts and is interesting also for an already knowledgeable audience. 

The code below gives an example of code using basic templates, functors, and lambda functions: 
 ```cpp 
struct OrderByFirstValue {
	template<typename T, typename U>
	bool operator()(T const& lhs, U const& rhs) const {
		return lhs.first < rhs.first;
	}
};

template<typename T, typename U>
void print(string header, vector<T,U> vs) {
	cout << header << endl;
	for (auto v: vs) {
		cout << v.first << ":" << v.second << endl;
	}
}

int main() {
	vector<pair<int, bool>> v = { make_pair(3, true), make_pair(2, false), make_pair(0, true), make_pair(1,true) };
	print("initial", v);
	sort(begin(v), end(v), OrderByFirstValue());
	print("OrderByFirstValue", v);
	sort(begin(v), end(v), 
		[](auto const& lhs, auto const & rhs) 
		{
			return rhs.first < lhs.first;
		});
	print("Lambda", v);
	return 0;
}
```

The code below gives an example of code using the Curiously Recurring Template Pattern; this pattern is used for two reasons: 
1. it provides a compile-time polymorphism through a template, which is more efficient than the run-time polymorphism achieved via inheritance and
2. it allows for less copy-paste in the definition of the classes and their methods.

```cpp
class Shape {
public:
	virtual Shape* clone() = 0;
	virtual ~Shape() = default; 
};

template<typename T>
class ShapeCRTP : public Shape {
	virtual Shape* clone() override { return new T(*static_cast<T*>(this)); }
};

class Square : public ShapeCRTP<Square> {
	int x = 1;
};
class Rectangle : public ShapeCRTP<Rectangle> {
	int x = 1;
	int y = 2;
};

int main() {
	vector<Shape*> v;
	v.push_back(new Square);
	v.push_back(new Rectangle);
	for (auto s: v) {
		s->clone();
	}
	cout << "Done!" << endl;
	return 0; 
}
```

A conceptual example for the visitor pattern

```cpp
// visitors.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>

using namespace std;
//forward declaration of AST node classes neede dby the visitor

class Stmt;
class Expr;

//Visitor interface
class Visitor {
public:
	virtual void visit(const Stmt* stmt) = 0;
	virtual void visit(const Expr* expr) = 0;
};

class ASTNode {public:
	virtual ~ASTNode() {}
	virtual void acccept(Visitor* visitor) const = 0;
};

class Stmt: public ASTNode {
public:
	void acccept(Visitor* visitor) const override {
		visitor->visit(this);
	}
	static string whoAmI() { return "this is a stmt"; }
	static string onlyForStmt() { return "only for stmt"; }
};

class Expr : public ASTNode {
public:
	void acccept(Visitor* visitor) const override {
		visitor->visit(this);
	}
	static string whoAmI() { return "this is an expr"; }
	static string onlyForExpr() { return "only for expr"; }
};

//visitors you can create without chnaging Stmt and Expr calsses
class WhoAmI : public Visitor {
	void visit(const Stmt* stmt) override {
		cout << stmt->whoAmI() << endl;
	}
	void visit(const Expr* expr) override {
		cout << expr->whoAmI() << endl;
	}
};

class OnlyForMe : public Visitor {
	void visit(const Stmt* stmt) override {
		cout << stmt->onlyForStmt() << endl;
	}
	void visit(const Expr* expr) override {
		cout << expr->onlyForExpr() << endl;
	}
};

int main()
{
	vector<ASTNode*> nodes = { new Stmt, new Expr };
	WhoAmI* whoAmI = new WhoAmI;
	OnlyForMe* onlyForMe = new OnlyForMe;

	for (ASTNode* node:nodes) {
		node->acccept(whoAmI);
		node->acccept(onlyForMe);
	}
	cout << "Done!" << endl;
	return 0;
}
```