#include<iostream>
#include<math.h>
#include<stdlib.h>

using namespace std;

int simulation_time,total_queues,total_iqueues;
float time_counter=0;
float trans_pr[50][50];

float generate_exponential(float rate){
	 float p = ((1.0*rand())/RAND_MAX);
	 return (1/rate)*(log(1/(1-p)));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class customer{
public:
	float entrytime;
	float exittime;
	customer *next;
	customer(){
		next = NULL;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class queue{};

class iqueue :public queue{
	public:
	int type;
	int waiting_status;
	float arrivalrate;
	iqueue(){
		type = 0;
		waiting_status = 0;
	}
};

class ioqueue :public queue{
	public:
		int type;
		int waiting_status;
		float servicerate;
		customer *head;
	ioqueue(){
		type = 1;
		waiting_status =0;
	}
};

class oqueue :public queue{
       public:
       int type;
       customer *head;
       oqueue(){
		type = 3;
	}
};

iqueue iq[50];
ioqueue ioq[50];
oqueue oq;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class event{
	public:
		int qno;
		float tim;
	event(){
	qno = -1;
	tim = -1;
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

event heap[100];

void addtoheap(event x){
	int i=1;
	event temp;
	while(heap[i].tim!=-1) ++i;
	heap[i] = x;
	while(heap[i].tim<heap[i/2].tim){
		temp = heap[i];
		heap[i] = heap[i/2];
		heap[i/2] = temp;
		i = i/2;
	}
}

void deletemin(){
	int i=1;
	event temp;
	while(heap[i].tim!=-1) ++i;
	i-=1;
	heap[1] = heap[i];
	heap[i].tim = -1;
	heap[i].qno = -1;
	i=1;
	while((heap[2*i].tim!=-1 && heap[i].tim > heap[2*i].tim) || (heap[2*i+1].tim!=-1 && heap[i].tim > heap[2*i+1].tim)){
//	cout << "delete" << endl;
		if(heap[2*i+1].tim!=-1 && heap[2*i].tim!=-1 && heap[i].tim > heap[2*i].tim && heap[i].tim > heap[2*i+1].tim){
			if(heap[2*i].tim < heap[2*i+1].tim){
				temp = heap[2*i];
				heap[2*i] = heap[i];
				heap[i] = temp;
				i = 2*i;
			}
			else{
				temp = heap[2*i+1];
				heap[2*i+1] = heap[i];
				heap[i] = temp;
				i = 2*i+1;
			}
		}
		else if (heap[i].tim > heap[2*i].tim && heap[2*i].tim!=-1){
			temp = heap[2*i];
			heap[2*i] = heap[i];
			heap[i] = temp;
			i = 2*i;
		}
		else if (heap[i].tim > heap[2*i+1].tim && heap[2*i+1].tim!=-1){
			temp = heap[2*i+1];
			heap[2*i+1] = heap[i];
			heap[i] = temp;
			i = 2*i+1;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(){
	for(int i=0;i<100;i++) heap[i].tim= -1;
	cin >> simulation_time;
	cin >> total_queues;
	cin >> total_iqueues;
	int rates[total_queues-1];
	for(int i=0;i<total_queues-1;++i){
		cin >> rates[i];
	}
	for(int i=0;i<total_iqueues;++i){
		iq[i].arrivalrate = rates[i];
	}
	for(int i=total_iqueues;i<total_queues-1;++i){
		ioq[i].servicerate = rates[i];
	}
	int m,n;
	for(int i=0;i<total_queues-1;++i){
		cin >> m;
		for(int j=0;j<m;++j){
			cin >> n;
			cin >> trans_pr[i][n];
		}
	}
	for(int i=0; i<total_queues-1;++i){
		for(int j=0;j<total_queues;++j){
			if(trans_pr[i][j]>0){
				for(int k=j-1;k>0;k--){
					trans_pr[i][j]+=trans_pr[i][k];
					if(trans_pr[i][k]>0) break;
				}
				if(trans_pr[i][j]>0.99) trans_pr[i][j]=1;
			}
		}
	}
                                     //////////////////////////////////////////////////////////////////////////
int o=0;
	while(time_counter<simulation_time){
		for(int i=0; i<total_iqueues; ++i){
			if(iq[i].waiting_status==0){
				event ob;
				ob.qno = i;
				float p = generate_exponential(iq[i].arrivalrate);
				ob.tim = p + time_counter;
				addtoheap(ob);
				iq[i].waiting_status = 1;
			}
		}

		for(int i=total_iqueues; i<total_queues-1; ++i){
			if(ioq[i].head != NULL && ioq[i].waiting_status == 0){
				event ob;
				ob.qno = i;
				ob.tim = generate_exponential(ioq[i].servicerate) + time_counter;
				addtoheap(ob);
				ioq[i].waiting_status = 1;
			}
		}
		time_counter = heap[1].tim;

		while(time_counter == heap[1].tim){
			if(heap[1].qno < total_iqueues){
				int i = heap[1].qno;
				customer *ob = new customer;
				ob->entrytime = time_counter;
				float p = (1.0*rand())/RAND_MAX;
				int j=1;
				while(1){
					if(p>trans_pr[heap[1].qno][j-1] && p<=trans_pr[heap[1].qno][j]) break;
					j++;
				}
				if(ioq[j].head == NULL){
					ioq[j].head = ob;
					ioq[j].head->next = NULL;
				}
				else{
					customer *ptr = ioq[j].head->next;
					ioq[j].head->next = ob;
					ioq[j].head->next->next = ptr;
				}
				iq[i].waiting_status = 0;
				deletemin();
			}
			else{
			    int i = heap[1].qno;
				customer *ob = ioq[i].head,*nextt = ioq[i].head->next;
				ioq[i].head = nextt;
				float p = (1.0*rand())/RAND_MAX;
				int j=1;
				while(1){
					if(p>trans_pr[heap[1].qno][j-1] && p<=trans_pr[heap[1].qno][j]) break;
					j++;
				}
				if(j<total_queues-1){
					if(ioq[j].head == NULL) {
						ioq[j].head = ob;
						ioq[j].head->next = NULL;
					}
					else{
					    customer *ptr = ioq[j].head->next;
						ioq[j].head->next = ob;
						ioq[j].head->next->next = ptr;
					}
				}
				if(j==total_queues-1){
					ob->exittime = time_counter;
					if(oq.head == NULL){
						oq.head = ob;
						oq.head->next = NULL;
					}
					else{
						customer *ptr = oq.head->next;
						oq.head->next = ob;
						oq.head->next->next = ptr;
					}
				}
				ioq[i].waiting_status = 0;
				deletemin();
			}
		}
	}
	float avg,sum;
	int count=0;
	customer *ptr;
	ptr = oq.head;
	while(ptr!=NULL){
		sum += (ptr->exittime - ptr->entrytime);
		count+=1;
		ptr = ptr->next;
	}
	avg = sum/(count*1.0);
	cout << avg << endl;
}
