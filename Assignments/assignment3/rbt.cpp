#include<iostream>

using namespace std;

class TreeNode{
//friend class rbt;
public:
float x;
TreeNode *left,*right,*parent;
int leftsize;
int bh;
bool color;

TreeNode(){
	left = right = parent = NULL;
	leftsize = 0;
	bh = 0;
	color = 0;
}
};

class rbt{
public:
TreeNode *root;

rbt(){
	root = NULL;
}

TreeNode* search(float k){
	TreeNode* head;
	head = root;
	while(head->right != NULL  || head->left != NULL){
		if(head->x == k ){ 
			break;
		}
		if(k < head->x){
			if(head->left == NULL) break;
			head = head->left;
		}
		if(k > head->x){
			if(head->right == NULL) break;
			head = head->right;
		}
	}
	return head;
}

void rotate(int a, TreeNode *node){
	if(a == 0){
		if(node->parent->left == node){    // right
			TreeNode *tmp;
			tmp = node->parent;
			if(node->parent == root) root = node;
			tmp->left = node->right;
			if(node->right!=NULL) node->right->parent = tmp;
			node->right = tmp;
			node->parent = tmp->parent;
			tmp->parent = node;
			tmp->leftsize -= (node->leftsize + 1);
			if(node->parent!=NULL && node->parent->left == tmp) node->parent->left = node;
			if(node->parent!=NULL && node->parent->right == tmp) node->parent->right = node;
		}	
	}
	if(a == 1){
		if(node->parent->right == node){
			TreeNode *tmp;
			tmp = node->parent;
			if(node->parent == root) root = node;
			tmp->right = node->left;
			if(node->left!=NULL) node->left->parent = tmp;
			node->left = tmp;
			node->parent = tmp->parent;
			tmp->parent = node;
			node->leftsize += (tmp->leftsize + 1);
			if(node->parent!=NULL && node->parent->left == tmp) node->parent->left = node;
			if(node->parent!=NULL && node->parent->right == tmp) node->parent->right = node;
		}	
	}
}

void insert(float k){
	if(root == NULL){
		root = new TreeNode;
		root->x = k;
		root->left = NULL;
		root->right = NULL;
		root->parent = NULL;
		root->leftsize = 0;
		root->bh = 0;
		root->color = 1;
	}
	else{	
		TreeNode *flag;
		flag = search(k);
		if(flag->x != k){
			TreeNode *nw = new TreeNode;
			nw->x = k;
			nw->left = NULL;
			nw->right = NULL;
			nw->parent = flag;
			nw->leftsize = 0;
			nw->bh = 0;
			nw->color = 0;
			if(k > flag->x) flag->right = nw;
			if(k < flag->x) flag->left = nw;
			flag = root;
			while(1){
				if(k > flag->x) flag = flag->right;
				if(k < flag->x) {
					flag->leftsize +=1;
					flag = flag->left;
				}
				if(k == flag->x) break;
			}
			insertbal(nw);
		}
	}
}

void insertbal(TreeNode *node){
	if(node->parent->color == 0){
		if(node->parent->parent->left == node->parent){
			if(node->parent->parent->right!=NULL && node->parent->parent->right->color == 0){
				node->parent->color = 1;
				node->parent->parent->right->color = 1;
				node->parent->parent->bh += 1;
				if(node->parent->parent != root){
					node->parent->parent->color = 0;
					insertbal(node->parent->parent);
				}
			}
			else if(node->parent->parent->right==NULL || node->parent->parent->right->color == 1){
				if(node->parent->left == node){
					node->parent->color = 1;
					node->parent->parent->color = 0;
					rotate(0,node->parent);
				}
				else if(node->parent->right == node){
					node->color = 1;
					node->parent->parent->color = 0;
					rotate(1,node);
					rotate(0,node);
				}
			}
		}
		else if(node->parent->parent->right == node->parent){
			if(node->parent->parent->left!=NULL && node->parent->parent->left->color == 0){
				node->parent->color = 1;
				node->parent->parent->left->color = 1;
				node->parent->parent->bh += 1;
				if(node->parent->parent != root){
					node->parent->parent->color = 0;
					insertbal(node->parent->parent);
				}
			}
			else if(node->parent->parent->left==NULL || node->parent->parent->left->color == 1){
				if(node->parent->right == node){
					node->parent->color = 1;
					node->parent->parent->color = 0;
					rotate(1,node->parent);
				}
				else if(node->parent->left == node){
					node->color = 1;
					node->parent->parent->color = 0;
					rotate(0,node);
					rotate(1,node);
				}
			}
		}
		
	}
}

void select(int k,TreeNode *node){
	while(1){
		if(node->leftsize+1 == k){
			cout << node->x << endl;
			break;
		}
		else if (k <= node->leftsize){
			node = node->left;
		}
		else if (k > (node->leftsize+1)){
			k = k-node->leftsize-1;
			node = node->right;
		}
	}
}

void del(float k){
	TreeNode *flag = search(k);
	TreeNode *trv = flag;
	int col;
	string w;
	if(flag->x == k){
		while(flag->right!=NULL || (flag->left == NULL && flag->right != NULL)){
			trv = trv->right;
			while(trv->left!=NULL){
				trv = trv->left;
			}
			flag->x = trv->x;
			flag = trv;
		}
		while(flag->left!=NULL && flag->right == NULL){
			trv = trv->left;
			while(trv->right!=NULL){
				trv = trv->right;
			}
			flag->x = trv->x;
			flag = trv;
		}
		col = flag->color;
		if(flag->parent->right == flag){
			flag = flag->parent;
			flag->right = NULL;
			w = "r";
		}
		else if(flag->parent->left == flag){
			flag = flag->parent;
			flag->leftsize -= 1; 
			flag->left = NULL;
			w = "l";
		}
		trv = flag;
		while(trv->parent!=NULL){
			if(trv->parent->left == trv){
				trv = trv->parent;
				trv->leftsize -= 1;
			}
			else if(trv->parent->right == trv){
				trv = trv->parent;
			}
		}	
		if(col == 1){
			deletebal(w,flag);
		}
	}
}

void deletebal(string w,TreeNode *node){
	if(w == "l"){
		if(node->right->color == 1){
			if((node->right->left != NULL && node->right->left->color==0) && (node->right->right == NULL || node->right->right->color ==1))		{	
				node->right->left->color = 1;
				node->right->color = node->color;
				node->right->left->bh += node->color;
				node->bh -= 1;
				rotate(0,node->right->left);
				rotate(1,node->right);
			}
			else if ((node->right->left == NULL || node->right->left->color ==1) && (node->right->right !=NULL && node->right->right->color == 0))	{	
				node->right->right->color = node->color;
				node->bh -= 1;
				node->right->bh += node->color;
				rotate(1,node->right);
			}
			else if((node->right->left == NULL || node->right->left->color == 1) && (node->right->right == NULL || node->right->right->color ==1))	{
				if(node->color == 0){
					node->color = 1;
					node->right->color = 0;
					node->bh-=1;
				}
				else if(node->color == 1){
					node->right->color = 0;
					node->bh -= 1;
					if(node!=root){
						if(node->parent->left == node){
							deletebal("l",node->parent);
						}
						if(node->parent->right == node){
							deletebal("r",node->parent);
						}
					}
				}
			}
			else if((node->right->left != NULL && node->right->right->color == 0) && (node->right->right != NULL && node->right->right->color ==0))	{
				node->right->right->color = 1;
				node->right->color = node->color;
				node->color = 1;
				node->bh -= 1;
				node->right->bh += 1;
				rotate(1,node->right);
			}
		}
		else if(node->right->color == 0){
			node->color = 0;
			node->right->color =1;
			rotate(1,node->right);
			deletebal("l",node);
		}
	}
	else if(w == "r"){
		if(node->left->color==1){
			if((node->left->left != NULL && node->left->left->color==0) && (node->left->right==NULL || node->left->right->color==1))		{	
				node->left->left->color = node->color;
				node->bh -= 1;
				node->left->bh += node->color;
				rotate(0,node->left);
			}
			else if((node->left->left==NULL || node->left->left->color==1) && (node->left->right!=NULL && node->left->right->color==0))		{
				node->left->right->color = 1;
				node->left->color = node->color;
				node->left->right->bh += node->color;
				node->bh -= 1;
				rotate(1,node->left->right);
				rotate(0,node->left);
			}
			else if((node->left->left==NULL || node->left->left->color==1) && (node->left->right==NULL || node->left->right->color==1))		{
				if(node->color == 0){
					node->color = 1;
					node->left->color = 0;
					node->bh-=1;
				}
				else if(node->color == 1){
					node->left->color = 0;
					node->bh -= 1;
					if(node!=root){
						if(node->parent->left == node){
							deletebal("l",node->parent);
						}
						if(node->parent->right == node){
							deletebal("r",node->parent);
						}
					}
				}
			}
			else if((node->left->left!=NULL && node->left->left->color==0) && (node->left->right!=NULL && node->left->right->color==0))		{
				node->left->left->color = 1;
				node->left->color = node->color;
				node->color = 1;
				node->bh -=1;
				node->left->bh +=1;
				rotate(0,node->left);
			}		
		}
		else if(node->left->color==0){
			node->color = 0;
			node->left->color = 1;
			rotate(0,node->left);
			deletebal("r",node);
		}	
	}
}

void join(float k,TreeNode *node1,TreeNode *node2){
	if(node1==NULL && node2==NULL){
		TreeNode *nw = new TreeNode;
		nw->x = k;
		nw->left = nw->right = nw->parent = NULL;
		nw->bh = nw->leftsize = 0;
		nw->color = 1;
		root = nw;
	}
	else if(node1==NULL){
		root = node2;
		insert(k);
		root->parent = NULL;
	}
	else if(node2==NULL){
		root = node1;
		insert(k);
		root->parent = NULL;
	}
	else if(node1->bh == node2->bh){
		TreeNode *nw = new TreeNode;
		TreeNode *tmp;
		nw->x = k;
		nw->color = 1;
		nw->bh = node1->bh + 1;
		nw->parent = NULL;
		nw->left = node1;
		nw->right = node2;
		node1->parent = nw;
		node2->parent = nw;
		nw->leftsize=0;
		tmp = nw->left;
		while(tmp!=NULL){
			nw->leftsize += tmp->leftsize +1;
			tmp = tmp->right;
		}
		root = nw;
	}
	else if(node1->bh > node2->bh){
		while(node1->bh!=node2->bh){
			node1 = node1->right;
		}
		TreeNode *nw = new TreeNode;
		TreeNode *tmp;
		nw->x = k;
		nw->color = 0;
		nw->bh = node1->bh + 1;
		nw->parent = node1->parent;
		nw->parent->right = nw;
		nw->left = node1;
		nw->right = node2;
		node1->parent = nw;
		if(node2!=NULL) node2->parent = nw;
		nw->leftsize = 0;
		tmp = nw->left;
		while(tmp!=NULL){
			nw->leftsize+=(tmp->leftsize+1);
			tmp = tmp->right;
		}
		tmp = nw;
		while(tmp->parent != NULL) tmp = tmp->parent;
		root = tmp;
		if(nw->parent->color == 0){
			insertbal(nw);
		}
	}
	else if(node1->bh < node2->bh){
		while(node1->bh!=node2->bh){
			node2 = node2->left;
		}
		TreeNode *nw = new TreeNode;
		TreeNode *tmp;
		nw->x = k;
		nw->color = 0;
		nw->bh = node1->bh + 1;
		nw->parent = node2->parent;
		nw->parent->left = nw;
		nw->left = node1;
		nw->right = node2;
		if(node1!=NULL) node1->parent = nw;
		node2->parent = nw;
		nw->leftsize = 0;
		tmp = nw->left;
		while(tmp!=NULL){
			nw->leftsize+=(tmp->leftsize+1);
			tmp = tmp->right;
		}
		tmp = nw;
		while(tmp->parent != NULL){
			tmp = tmp->parent;
			tmp->leftsize += nw->leftsize+1;
		}
		tmp = nw;
		while(tmp->parent != NULL) tmp = tmp->parent;
		root = tmp;
		if(nw->parent->color == 0){
			insertbal(nw);
		}
	}
}

TreeNode* split(float k,rbt ob){
	TreeNode *flag,*node,*rt;
	rbt tl,tr;
	flag = ob.search(k);
	if(flag->x == k){
		tl.root = flag->left;
		if(tl.root!=NULL){
			tl.root->parent = NULL;
			tl.root->color = 1;
		}
		tr.root = flag->right;
		if(tr.root!=NULL){
			tr.root->parent = NULL;
			tr.root->color = 1;
		}
		node = flag->parent;
		while(node != NULL){
			if(node->left == flag){
				if(node->right!=NULL){
					node->right->parent = NULL;
					node->right->color = 1;
				}
				rt = node->right;
				tr.join(node->x,tr.root,node->right);
				flag = node;
				node = node->parent;
			}
			else if(node->right == flag){
				if(node->left!=NULL){ 
					node->left->parent = NULL;
					node->left->color = 1;
				}
				tl.join(node->x,node->left,tl.root);
				flag = node;
				node = node->parent;
			}
		}
	root = tr.root;
	return tl.root;
	}
}

};



int main(){
	rbt t[10001];
	int ni,tn,ne;
	float key;
	string s;
	cin >> ni;
	for(int i=1 ; i<=ni ; i++){
		cin >> s;
		if(s == "insert"){
			cin >> tn;
			cin >> ne;
			for(int j=0 ; j<ne ; j++){
				cin >> key;
				t[tn].insert(key);
			}
		}
		if(s == "select"){
			cin >> tn;
			cin >> key;
			t[tn].select(key,t[tn].root);
		}
		if(s == "delete"){
			cin >> tn;
			cin >> ne;
			for(int j=0 ; j<ne ;j++){
				cin >> key;
				t[tn].del(key);
			}
		}
		if(s == "join"){
			int tl,tr,td;
			cin >> tl;
			cin >> key;
			cin >> tr;
			cin >> td;
			t[td].join(key,t[tl].root,t[tr].root);
		}
		if(s == "split"){
			int tl,tr,tt;
			cin >> tt;
			cin >> key;
			cin >> tl;
			cin >> tr;
			TreeNode *flag = t[tt].search(key);
			if(flag->x != key) t[tt].insert(key);
			t[tl].root = t[tr].split(key,t[tt]);
		}
	}
}
