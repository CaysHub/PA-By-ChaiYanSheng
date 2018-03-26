#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;
uint32_t expr(char *e,bool *success);
void init_regex();
void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
  WP *p=free_;
	if(p==NULL){
		printf("Error:watchpoint struct have been all\n");
	  assert(0);
	}else if(p!=NULL){
	  free_=free_->next;
		p->next=NULL;
	}
	return p;
}

void free_wp(WP* wp){
	if(wp==NULL){
	  assert(0);
	}else{
		wp->type=' ';
		wp->enb='n';
	  wp->next=free_;
		free_=wp;
	}
}

void insert_wp(char *args){
  init_regex();
	bool s=true;
	int value=expr(args,&s);
	if(!s){
	  return;
	}
	WP *wp1=new_wp();
	wp1->type='w';
	wp1->enb='y';
	strcpy(wp1->expression,args);
	wp1->value=value;
	if(head==NULL){
		wp1->NO=1;
		head=wp1;
		wp1->next=NULL;
	}else{
	  WP *p=head;
		while(p->next!=NULL)p=p->next;
		wp1->NO=p->NO+1;
		p->next=wp1;
		wp1->next=NULL;
	}
}

void print_all_wp(){
  printf("All the watchpoints with their information:\n");
	if(head==NULL){
		printf("No watchpoint\n");
	  return;
	}
	printf("Num     Type    Value    Enb  What    \n");
	WP *p=head;
	while(p!=NULL){
	  printf("%-8d",p->NO);
		printf("%-8c",p->type);
		printf("0x%-7x",p->value);
		printf("%-5c",p->enb);
		printf("%-8s",p->expression);
		printf("\n");
		p=p->next;
	}
}

void print_one_wp(int no){
  WP *p=head;
	while(p!=NULL&&p->NO!=no){
	  p=p->next;
	}
	if(p==NULL){
	  return;
	}else{
		bool s=true;
	  int new_value=expr(p->expression,&s);
		if(!s)return;
		printf("Watchpoint  %d: %s\n",p->NO,p->expression);
		printf("Old Value   %d\n",p->value);
		printf("New Value   %d\n",new_value);
		printf("W%s\n",p->expression);
	}
}

void delete_wp(int no){
  if(head==NULL){
	  printf("No such Watchpoint\n");
		return;
	}
	WP *p=head,*q;
	q=p->next;
	if(head->NO==no){
	  head=head->next;
		free_wp(p);
		printf("Delete the NO.%d Watchpoint Success\n",p->NO);
		return;
	}
	while(q!=NULL&&q->NO!=no){
	  p=p->next;
		q=p->next;
	}
	if(q==NULL){
	  printf("No such Watchpoint\n");
		return;
	}
	p->next=q->next;
	free_wp(q);
}

bool check_changed_wp(){
  if(head==NULL)return false;
	WP *p=head;
	int flag=0;
	while(p!=NULL){
		bool s=true;
	  int new_value=expr(p->expression,&s);
		if(new_value!=p->value){
		  print_one_wp(p->NO);
			flag=1;
		}
		p->value=new_value;
		p=p->next;
	}
	if(flag==1)return true;
	return false;
}
