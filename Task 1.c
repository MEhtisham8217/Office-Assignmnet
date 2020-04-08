#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct employee {
  char* name;
  struct employee* supervisor;
  struct employee* subordinates;
  size_t n_subordinates;
};


struct office {
  struct employee* department_head;
};

struct employee** queue;
int size;
int front,rear;
void initQueue(){
queue=malloc(sizeof(struct employee*)*100);
size=0;
front=0;
rear=-1;
}


void insert(struct employee* data) {
   queue[++rear] = data;
   size++;
}

struct employee* removeData() {
   size--;
   return queue[front++]; 
}

int isQueueEmpty() {
   if(size == 0){
		return 0;	
   }
   return 1;
}


/**
 * Places an employee within the office, if the supervisor field is NULL
 *  it is assumed the employee will be placed under the next employee that is
 * not superivising any other employee (top-down, left-to-right).
 * 
 * If the supervisor is not NULL, it will be added to the supervisor's list
 *  of employees (make sure the supervisor exists in hierarchy).
 * if the office or employee are null, the function not do anything.
 */
void office_employee_place(struct office* off, struct employee* supervisor,
  struct employee* emp){
  	if(off==NULL || emp==NULL){
  		return;
	}
	else{
		if(supervisor==NULL){
			initQueue();
			int check=0;
			struct employee* head=off->department_head;
			insert(head);
			while((isQueueEmpty()==1)&&check==0){
				struct employee* curr=removeData();
				if(curr->n_subordinates==0){
					curr->n_subordinates++;
					strcpy(curr->subordinates[0].name,emp->name);
					curr->subordinates=emp->subordinates;
					curr->supervisor=emp->supervisor;
				}
				else{
					int i=0;
					for(i=0;i<curr->n_subordinates&&check==0;i++){
						if(curr->subordinates[i].n_subordinates==0){
							curr->subordinates[i].n_subordinates++;
							strcpy(curr->subordinates[i].subordinates[0].name,emp->name);
							curr->subordinates[i].subordinates=emp->subordinates;
							curr->subordinates[i].supervisor=curr;
							check=1;
						}
						else{
							insert(&curr->subordinates[i]);
						}
					}
				}
			}
		}
		else{
			int check=0;
			initQueue();
			struct employee* head=off->department_head;
			insert(head);
			while((isQueueEmpty()==1)&&check==0){
				struct employee* curr=removeData();
				int i=0;
				for(i=0;i<curr->n_subordinates&&check==0;i++){
					if(!strcmp(curr->subordinates[i].name,supervisor->name)){
						check=1;
					}
					else{
						insert(&curr->subordinates[i]);
					}
				}
			}
			if(check==1){
				strcpy(supervisor->subordinates[supervisor->n_subordinates].name,emp->name);
				supervisor->subordinates[supervisor->n_subordinates].n_subordinates=emp->n_subordinates;
				supervisor->subordinates[supervisor->n_subordinates].subordinates=emp->subordinates;
				supervisor->subordinates[supervisor->n_subordinates].supervisor=supervisor;
			}
		}
	}
	  
}


/**
 * Fires an employee, removing from the office
 * If employee is null, nothing should occur
 * If the employee does not supervise anyone, they will just be removed
 * If the employee is supervising other employees, the first member of that 
 *  team will replace him.
 */
void office_fire_employee(struct employee* employee){
	if(employee==NULL){
		return;
	}
	else{
		if(employee->subordinates==NULL){
			struct employee* curr = employee->supervisor;
			int i=0,j=0;
			for(i=0;i<curr->n_subordinates;i++){
				if(!strcmp(curr->subordinates[i].name,employee->name)){
					j=i;
					break;
				}
			}
			if(i>curr->n_subordinates&&j==0){
				return;
			}
			else{
				curr->n_subordinates--;
				for(i=j;i<curr->n_subordinates;i++){
					strcpy(curr->subordinates[i].name,curr->subordinates[i+1].name);
					curr->subordinates[i].subordinates=curr->subordinates[i+1].subordinates;
					curr->subordinates[i].supervisor=curr->subordinates[i+1].supervisor;
					curr->subordinates[i].n_subordinates=curr->subordinates[i+1].n_subordinates;
				}
			}
		}
		else{
			struct employee* temp;
			temp->name=employee->subordinates[0].name;
			struct employee* curr=employee;
			curr->n_subordinates--;
			int i=0;
			for(i=0;i<curr->n_subordinates;i++){
				strcpy(curr->subordinates[i].name,curr->subordinates[i+1].name);
				curr->subordinates[i].subordinates=curr->subordinates[i+1].subordinates;
				curr->subordinates[i].supervisor=curr->subordinates[i+1].supervisor;
				curr->subordinates[i].n_subordinates=curr->subordinates[i+1].n_subordinates;
			}
			strcpy(employee->name,temp->name);
		}
	}
}


/**
 * Retrieves the first encounter where the employee's name is matched to one in the office
 * If the employee does not exist, it must return NULL
 * if office or name are NULL, your function must do nothing
 */ 
struct employee* office_get_first_employee_with_name(struct office* office,
  const char* name){
  	if(office==NULL || name==NULL){
		return NULL;
	}
	else{
	  	initQueue();
		struct employee* head=office->department_head;
		insert(head);
		while((isQueueEmpty()==1)){
			struct employee* curr=removeData();
			int i=0;
			for(i=0;i<curr->n_subordinates;i++){
				if(!strcmp(curr->subordinates[i].name,name)){
					return &curr->subordinates[i];
				}
				else{
					insert(&curr->subordinates[i]);
				}
			}
		}
		return NULL;
	}
}

/**
 * Retrieves the last encounter where the employee's name is matched to one in the office
 * If the employee does not exist, it must return NULL
 * if office or name are NULL, your function must do nothing
 */ 
struct employee* office_get_last_employee_with_name(struct office* office,
  const char* name){
  	if(office==NULL||name==NULL){
  		return NULL;
	}
	else{
		int ind=-1;
		initQueue();
		struct employee* head=office->department_head;
		insert(head);
		struct employee* res=NULL;
		while((isQueueEmpty()==1)){
			struct employee* curr=removeData();
			int i=0;
			for(i=0;i<curr->n_subordinates;i++){
				if(!strcmp(curr->subordinates[i].name,name)){
					res=&curr->subordinates[i];
				}
				else{
					insert(&curr->subordinates[i]);
				}
			}
		}
		return res;
	}
}

/**
 * This function will need to retrieve all employees at a level.
 * A level is defined as distance away from the boss. For example, all 
 * subordinates of the boss are 1 level away, subordinates of the boss's subordinates
 * are 2 levels away.
 * 
 * if office, n_employees or emplys are NULL, your function must do nothing
 * You will need to provide an allocation for emplys and specify the
 * correct number of employees found in your query.
 */

void getemp(struct employee* curr,int level,int rlevel){
	if(level==rlevel){
		insert(curr);
	}
	else{
		int i=0;
		for(i=0;i<curr->n_subordinates;i++){
			getemp(&curr->subordinates[i],level+1,rlevel);
		}
	}
} 

void office_get_employees_at_level(struct office* office, size_t level,
  struct employee** emplys, size_t* n_employees){
  	if(office==NULL||emplys==NULL||n_employees==NULL){
  		return;
	}
	else{
		struct employee* head=office->department_head;
		initQueue();
		getemp(head,0,level);
		emplys=malloc(sizeof(struct employee*)*size);
		*n_employees=size;
		int i=0;
		while(isQueueEmpty()==1){
			emplys[i]=removeData();
		}
	}
}

/**
 * Will retrieve a list of employees that match the name given
 * If office, name, emplys or n_employees is NULL, this function should do nothing
 * 
 * if office, n_employees, name or emplys are NULL, your function must do nothing
 * You will need to provide an allocation to emplys and specify the
 * correct number of employees found in your query.
 */
void getname(struct employee* curr,const char* name){
	if(!strcmp(curr->name,name)){
		insert(curr);
	}
	else{
		int i=0;
		for(i=0;i<curr->n_subordinates;i++){
			getname(&curr->subordinates[i],name);
		}
	}
} 
void office_get_employees_by_name(struct office* office, const char* name,
  struct employee** emplys, size_t* n_employees){
  	if(office==NULL||name==NULL||emplys==NULL||n_employees==NULL){
  		return;
	}
	else{
	  	struct employee* head=office->department_head;
		initQueue();
		getname(head,name);
		emplys=malloc(sizeof(struct employee*)*size);
		*n_employees=size;
		int i=0;
		while(isQueueEmpty()==1){
			emplys[i]=removeData();
		}
	}
}

/**
 * You will traverse the office and retrieve employees using a postorder traversal
 * If off, emplys or n_employees is NULL, this function should do nothing
 *
 * You will need to provide an allocation to emplys and specify the
 * correct number of employees found in your query.
 */

void postorder(struct employee* curr){
	if(curr->n_subordinates==0){
		return;
	}
	else{
		int i=0;
		for(i=0;curr->n_subordinates;i++){
			postorder(&curr->subordinates[i]);
		}
		insert(curr);
	}
} 

void office_get_employees_postorder(struct office* off, struct employee** emplys,
  size_t* n_employees){
  	if(off==NULL||emplys==NULL||n_employees==NULL){
  		return;	
	}
	else{
	  	initQueue();
	  	struct employee* head=off->department_head;
	  	postorder(head);
	  	emplys=malloc(sizeof(struct employee*)*size);
	  	*n_employees=size;
	  	int i=0;
	  	while(isQueueEmpty()==1){
	  		emplys[i++]=removeData();	
		}
	}
}
/*
 * The employee will be promoted to the same level as their supervisor and will join
 *  their supervisor's team.
 * If the employee has subordinates, the first subordinate will
 *  be promoted to manage that team.
 * if emp is NULL, this function will do nothing
 * if the employee is at level 0 or level 1, they cannot be promoted
 */
void office_promote_employee(struct employee* emp){
	if(emp==NULL||emp->supervisor->supervisor==NULL||emp->supervisor==NULL){
		return;
	}
	else{
		struct employee* curr=emp->supervisor->supervisor;
		strcpy(curr->subordinates[curr->n_subordinates].name,emp->name);
		curr->subordinates[curr->n_subordinates].subordinates=NULL;
		curr->subordinates[curr->n_subordinates].supervisor=curr;
		curr->subordinates[curr->n_subordinates++].n_subordinates=0;
		struct employee* temp;
		temp->name=emp->subordinates[0].name;
		curr=emp;
		int i=0;
		for(i=0;i<curr->n_subordinates-1;i++){
			strcpy(curr->subordinates[i].name,curr->subordinates[i+1].name);
			curr->subordinates[i].subordinates=curr->subordinates[i+1].subordinates;
			curr->subordinates[i].supervisor=curr->subordinates[i+1].supervisor;
			curr->subordinates[i].n_subordinates=curr->subordinates[i+1].n_subordinates;
		}
		strcpy(emp->name,temp->name);
	}
}

/**
 * Demotes an employee, placing them under the supervision of another employee.
 * If supervisor or emp is null, nothing should occur
 * If the employee does not supervise anyone, they will not be demoted as they are
 *  already at the lowest position
 * If an employee is to be demoted but their new position level is less than their current one
 *   nothing will happen. (Attempting to promote through demotion)
 * Otherwise, the employee should be assigned at the end the supervisor's team and
 *  the first employee from the previously managed team will be promoted.
 *
 * Edge case:
 * if the supervisor use to be an subordinate to the demoted employee (and they will get promoted)
 * the demoted employee will be attached to subordinate's new subordinates list
 * not their previous list.
 */
void office_demote_employee(struct employee* supervisor, struct employee* emp){
	if(supervisor==NULL||emp==NULL){
		return;
	}
	else{
		if(emp->n_subordinates==0){
			strcpy(supervisor->subordinates[supervisor->n_subordinates].name,emp->name);
			supervisor->subordinates[supervisor->n_subordinates].n_subordinates=0;
			supervisor->subordinates[supervisor->n_subordinates].subordinates=NULL;
			supervisor->subordinates[supervisor->n_subordinates++].supervisor=supervisor;
		}
		else{
			int i=0,check=0;
			for(i=0;i<emp->n_subordinates;i++){
				if(!strcmp(emp->subordinates[i].name,supervisor->name)){
					check=1;
					break;
				}
			}
			if(check==1){
				struct employee* temp;
				strcpy(temp->name,supervisor->name);
				temp->n_subordinates=supervisor->n_subordinates;
				temp->subordinates=supervisor->subordinates;
				temp->supervisor=supervisor->supervisor;
				if(supervisor->n_subordinates==0){
					strcpy(supervisor->name,emp->name);
					supervisor->supervisor=emp->supervisor;
					supervisor->subordinates=emp->subordinates;
					supervisor->n_subordinates=emp->n_subordinates;
					strcpy(emp->name,temp->name);
					emp->n_subordinates=0;
					emp->subordinates=temp->subordinates;
					emp->supervisor=supervisor;
				}
				else{
					emp->n_subordinates--;
					char curr[50];
					strcpy(curr,emp->name);
					char temp1[50];
					strcpy(temp1,supervisor->name);
					strcpy(supervisor->name,supervisor->subordinates[0].name);
					strcpy(emp->name,temp1);
					strcpy(emp->subordinates[emp->n_subordinates].name,curr);
					emp->subordinates[emp->n_subordinates].n_subordinates=0;
					emp->subordinates[emp->n_subordinates++].subordinates=NULL;
					emp->supervisor=supervisor;
				}
			}
			else{
				if(emp->n_subordinates==0){
					strcpy(supervisor->subordinates[supervisor->n_subordinates].name,emp->name);
					supervisor->subordinates[supervisor->n_subordinates].supervisor=supervisor;
				}
				else{
					char temp1[50];
					strcpy(temp1,emp->name);
					strcpy(emp->name,emp->subordinates[0].name);
					strcpy(supervisor->subordinates[supervisor->n_subordinates].name,temp1);
					supervisor->subordinates[supervisor->n_subordinates].supervisor=supervisor;
				}
			}
		}
	}
}

/**
 * The office disbands
 * (You will need to free all memory associated with employees attached to
 *   the office and the office itself)
 */

void del(struct employee* curr){
	if(curr->n_subordinates==0){
		delete curr;
	}
	else{
		int i=0;
		for(i=0;i<curr->n_subordinates;i++){
			del(&curr->subordinates[i],name);
		}
		delete curr;
	}
}


void office_disband(struct office* office){
struct employee* curr=office->department_head;
del(curr);
delete office;	
}

