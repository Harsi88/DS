#include<iostream>
#include<string>
#include<stdlib.h>
#include<sstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GenListNode
{
public:
	int tab;
	string s;
	GenListNode *down,*next;
	GenListNode()
	{
		down = NULL;
		next = NULL;
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class value
{
public:
	int tag;
	float val;
	GenListNode *link;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class environment
{
public:
	value* table[26];
	environment *parent;
	environment()
	{
		for(int i=0 ; i<26 ; ++i) table[i] = NULL;
		parent = NULL;
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void add(char x, value valnew, environment &e)
	{
		value *p = new value;
		p->tag = valnew.tag;
		p->link = valnew.link;
		p->val = valnew.val;
		e.table[x-'a'] = p;
	}


value find(char x, environment& e)
{
	if (e.table[x-'a'] != NULL)
	{
		return *(e.table[x-'a']);
	}
	else
	{
		if (e.parent == NULL)
		{
			cout << "Entry " << x << " not found in symbol table " << endl;
		}
		else
		{
		//	cout << " Looking in parent environment " << endl;
			return find(x, *(e.parent));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int bracket = 0;

GenListNode* makeList()
{
	string str;
	cin >> str;
//	if(str.compare("!")==0) return NULL;
	GenListNode *ob = new GenListNode();
	if(str.compare("(")==0)
	{
		bracket++;
		ob->tab = 1;
		ob->down = makeList();
		if( bracket != 0 )
		ob->next = makeList();
		return ob;
	}
	else if(str.compare(")")==0)
	{
		bracket--;
		delete ob;
		return NULL;
	}
	else
	{
		ob->tab = 0;
		ob->s = str;
		ob->down = NULL;
		ob->next = makeList();
		return ob;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printList(GenListNode* ob)
{
	if(ob->tab==1)
	{
		if(ob->down != NULL ) printList(ob->down);
		if(ob->next != NULL ) printList(ob->next);
		return;
	}
	else
	{
		cout << ob->s << endl;
		if(ob->next != NULL ) printList(ob->next);
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

value evallist(GenListNode *ob, environment &e);

value evalnode(GenListNode *ob, environment &e)
{
	const char *p = ob->s.c_str();
	if(ob->tab == 1)
	{
		return evallist(ob->down,e);
	}
	if( *p-'a'>=0 && *p-'a'<=25)
	{
		char q = *p;
		return find(q,e);
	}
	if( *p>=48 && *p<=57)
	{
		value *vl;
		vl = new value;
		vl->tag = 0;
		stringstream cvt(ob->s);
		cvt>>vl->val;
		vl->link = NULL;
		return *vl;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

value evallist(GenListNode *ob, environment &e)
{
	if(ob->s.compare("+")==0 || ob->s.compare("-")==0 || ob->s.compare("*")==0 || ob->s.compare("/")==0 || ob->s.compare("%")==0 || ob->s.compare("==")==0 || ob->s.compare("<=")==0 || ob->s.compare(">=")==0 || ob->s.compare("!=")==0 || ob->s.compare("<")==0 || ob->s.compare(">")==0)
	{
		value v1 = evalnode(ob->next , e);
		value v2 = evalnode(ob->next->next , e);
		value *v;
		v = new value;
		v->tag = 0;
		if(ob->s.compare("+")==0) v->val = v1.val + v2.val;
		if(ob->s.compare("-")==0) v->val = v1.val - v2.val;
		if(ob->s.compare("*")==0) v->val = v1.val * v2.val;
		if(ob->s.compare("/")==0) v->val = v1.val / v2.val;
		if(ob->s.compare("==")==0)
		{
		 	if(v1.val == v2.val) v->val = 1;
			else v->val = 0;
		}
		if(ob->s.compare("<=")==0)
		{
		 	if(v1.val <= v2.val) v->val = 1;
			else v->val = 0;
		}
		if(ob->s.compare(">=")==0)
		{
		 	if(v1.val >= v2.val) v->val = 1;
			else v->val = 0;
		}
		if(ob->s.compare("!=")==0)
		{
		 	if(v1.val != v2.val) v->val = 1;
			else v->val = 0;
		}
		if(ob->s.compare("<")==0)
		{
		 	if(v1.val < v2.val) v->val = 1;
			else v->val = 0;
		}
		if(ob->s.compare(">")==0)
		{
		 	if(v1.val > v2.val) v->val = 1;
			else v->val = 0;
		}
		if(ob->s.compare("%")==0) v->val = (int)v1.val % (int)v2.val;
		return *v;
	}
	if(ob->s.compare("begin")==0)
	{
		ob = ob->next;
		while (ob->next != NULL)
		{
			evalnode(ob,e);
			ob = ob->next;
		}
		return evalnode(ob,e);
	}
	if(ob->s.compare("if")==0)
	{
		value v = evalnode(ob->next , e);
		if( v.val == 1) return evalnode(ob->next->next,e);
		else return evalnode(ob->next->next->next,e);
	}
	if(ob->s.compare("while")==0)
	{
		value v = evalnode(ob->next , e);
		value ret;
		while(v.val == 1)
		{
			ret = evalnode(ob->next->next,e);
			v = evalnode(ob->next,e);
		}
		return ret;
	}
	if(ob->s=="define")
	{
		value v = evalnode(ob->next->next,e);
		add(ob->next->s[0] , v , e);
		return v;
	}
	if(ob->s=="set")
	{
		value v = evalnode(ob->next->next,e);
		add(ob->next->s[0] , v , e);
		return v;
	}
	if(ob->s=="lambda")
	{
		value v;
		v.tag = 1;
		v.link = ob;
		return v;
	}
	const char *p = ob->s.c_str();
	if( *p-'a'>=0 && *p-'a'<=25)
	{

		value v = find(*p,e),v1;
		environment e1;
		e1.parent = &e;
		GenListNode *ob1 = v.link->next->down;
		GenListNode *ob2=ob1;
		while(ob1->next != NULL )
		{
			add(ob1->s[0] , evalnode(ob->next,e) , e1 );
			ob1=ob1->next;
			ob = ob->next;
		}
		add(ob1->s[0] , evalnode(ob->next,e) , e1 );
		return evalnode (v.link->next->next , e1);
	}
	if(ob->tab == 1)
	{
		environment e1;
		e1.parent = &e;
		GenListNode *ob1 = ob->down->next->down, *link = ob->down->next;
		while(ob1->next != NULL )
		{
			add(ob1->s[0] , evalnode(ob->next,e) , e1 );
			ob1=ob1->next;
			ob = ob->next;
		}
		add(ob1->s[0] , evalnode(ob->next,e) , e1 );
		return evalnode (link->next , e1);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	cout << "enter : ";
//	printList(makeList());
	environment e;
	GenListNode *ob = makeList();
	value v = evallist(ob->down,e);
	cout << v.val<<endl;
}
