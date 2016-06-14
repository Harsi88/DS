#include<iostream>
using namespace std;
class node{
public:
	int edge,weight;
	node* next;
	node(){
		edge = weight = 0;
		next = NULL;
	}
};
float findmin(int a[],int size,bool isvisit[]){
	int mini,j=0;
	while(isvisit[j]==1) j++;
	mini = j;
	for(int i=0;i<size;i++){
		if(!isvisit[i] && a[i]<a[mini]) mini=i;
	}
	return mini;
}
int main(){
	int tv,sv,dv,nbv,noe;
	cin >> tv;
	cin >> dv;
	cin >> sv;
	int bv[tv];
	node* ptr[tv],*pt;
	for (int i=0; i<tv; i++) ptr[i]=NULL;
	for(int i=0; i<tv; i++){
		cin >> noe;
		for(int j=0; j<noe; j++){
			pt = ptr[i];
			if(pt==NULL){
				ptr[i] = new node;
				cin >> ptr[i]->edge;
				cin >> ptr[i]->weight;
				ptr[i]->next = NULL;
			}
			else{
				while(pt->next!=NULL){
					pt = pt->next;
				}
				pt->next = new node;
				cin >> pt->next->edge;
				cin >> pt->next->weight;
				pt->next->next = NULL;
			}
		}
	}
	cin >> nbv;
	int n;
	for(int i=0; i<tv; i++) bv[i]=0;
	for(int i=0; i<nbv; i++){
		cin >> n;
		bv[n] = 1;
	}
	int vrtx[tv];
	bool isvisit[tv],blk_fnd[tv];
	int pi[tv];
	int c,cv;
	bool end_reached=false;
	for(int i=0; i<tv; i++) blk_fnd[i] = false;
while(!end_reached){
	end_reached = true;
	for(int i=0; i<tv; i++){
		pi[i]=0;
		isvisit[i]=false;
		if(blk_fnd[i]) isvisit[i]=true;
		if(i==sv) vrtx[i]=0;
		else vrtx[i]=999999;
	}
	cv = findmin(vrtx,tv,isvisit);
	while(cv != dv){
		if(vrtx[cv] == 999999) break;
		pt = ptr[cv];
		while(pt!=NULL){
			if(!isvisit[pt->edge]){
			if(vrtx[pt->edge] > (vrtx[cv]+pt->weight))  pi[pt->edge] = cv;
			vrtx[pt->edge] = ((vrtx[pt->edge] < (vrtx[cv]+pt->weight)) ? vrtx[pt->edge] : (vrtx[cv]+pt->weight));
			}
			pt = pt->next;
		}
		isvisit[cv] = true;
	 	cv = findmin(vrtx,tv,isvisit);
	}
	if(vrtx[cv] == 999999){
		cout << "No path to goal exists" << endl;
		break;
	}
	cout << "Found new path from "<< dv << " of length " << vrtx[dv] << " :" << endl;
	c=dv;
	while(c!=sv){
		cout << c << " ";
		c  = pi[c];
	}
	cout << c << endl;
	c=dv;
	while(c!=sv){
		if(bv[pi[c]]==1) break;
		c = pi[c];
	}
	if(c==sv){
		cout << "Reached goal" << endl;
		break;
	}
	else{
		end_reached = false;
		cout << "Found blocked vertex at " << pi[c] << endl;
		dv = c;
		blk_fnd[pi[c]] = true;
	}
}
}
