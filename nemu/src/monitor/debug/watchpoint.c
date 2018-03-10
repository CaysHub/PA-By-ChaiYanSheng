#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

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
  WP *p=free_,*q,*s;
	q=head;s=head->next;
	if(p==NULL){
		printf("Error:watchpoint struct have been all\n");
	  assert(0);
	}else if(p!=NULL){
		if(head==NULL){
		  free_==free_->next;
			head=p;
			p->next=NULL;
		}else if(head!=NULL){
			if(q->NO>p->NO){
			  free_=p->next;
				p->next=head;
				head=p;
			}else{
		    while(s!=NULL&&q->NO<p->NO){
			    q=q->next;
				  s=q->next;
			  }
			  q->next=p;
			  free_=p->next;
			  p->next=s;
			}
		}
	}
	return p;
}

void free_wp(WP* wp){
	WP *p,*q,*s,*r;
	p=head;q=p->next;
	r=free_;s=r->next;
	if(head==NULL){
	  printf("Error:No such watchpoint\n");
		assert(0);
	}else if(head!=NULL){
	  if(head==wp){
			q=head;
		  head=head->next;
		}else{
		  while(q!=NULL&&q!=wp){
			  p=p->next;
				q=p->next;
			}
			if(q==NULL){
			  printf("Error:No such watchoint\n");
				assert(0);
			}
			p->next=q->next;
		}
		if(free_==NULL||q->NO<free_->NO){
		  q->next=free_;
			free_=q;
		}else{
		  while(r!=NULL&&s->NO<q->NO){
			  s=s->next;
				r=s->next;
			}
			s->next=q;
			q->next=r;
		}
	}
  return;
}

