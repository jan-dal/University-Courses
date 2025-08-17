#include <stdio.h>
#include <stdlib.h>


typedef struct AVL_node AVL_node;
typedef struct Q_node Q_node;

struct AVL_node{
	short key;
	AVL_node *left, *right;
};

struct Q_node{
	short *Conf;
	int move;
	Q_node *next;
};

void test(short *SConf, int n);
void push(Q_node *tail, short *conf, int n, int moves);
Q_node pop(Q_node *head);

int main(){
	int d = 0, n = 0;
	short *Sconf;
	scanf("%d\n", &d);

	for(int i=0; i < d; i++){
		scanf("%d", &n);
		SConf = malloc(sizeof(short)*n);
		for(int j=0; j < n; j++){
			scanf("%hd", SConf+j);
		}
		test(SConf, n);
	}

	free(SConf);

	return 0;
}

void test(short *SConf, int n){
	int min_moves = 0, curr_moves = 0, total_conf = 0, key = 0;
	Q_node *head = NULL;
	Q_node *tail = NULL;
	Q_node q_conf, q_sconf;
	short *new_conf;

	type sets = malloc(sizeof(*type)*7001); // Figure type

	head = (Q_node *)malloc(sizeof(Q_node));
	head->next = NULL;
	head->move = 0;
	head->Conf = malloc(sizeof(short)*n);

	memcpy(q_sconf.Conf, SConf, n);

	tail = head;

	while(head != NULL){
		key = 0;
		q_conf = pop(head);
		curr_moves = q_conf.moves;
		for(int i=0; i < n; i++){
			key += q_conf.Conf[i];
		}

		// Check if this configuration was reached before
		// if yes then continue else add it to the set and
		// check all possible moves

		if(is_in_set(sets[key], q_conf.Conf, n)){
			continue;
		}


		// AFTER CHECK

		if(curr_moves > min_moves){
			min_moves += 1;
		}
		total_conf += 1;

		for(int i=0; i < n; i++){
			for(int j=0; j < n; j++){
				if(i != j && q_conf.Conf[j] < SConf[j] && q_conf.Conf[i] > 0){
					new_conf = malloc(sizeof(short)*n);
					memcpy(new_conf, q_conf.Conf, n);
					short diff = SConf[j] - q_conf.Conf[j];
					if(diff > q_conf.Conf[i]){
						new_conf[j] += new_conf[i];
						new_conf[i] = 0;
					} else {
						new_conf[j] = SConf[j];
						new_conf[i] -= diff;
					}
					push(tail, new_conf, n, curr_moves+1);
				}	
			}
		}

		for(int i=0; i < n; i++){
			if(q_conf.Conf[i] > 0){
				new_conf = malloc(sizeof(short)*n);
				memcpy(new_conf, q_conf.Conf, n);
				new_conf[i] = 0;
				push(tail, new_conf, n, curr_moves+1);
			}
		}

		free(q_conf.Conf);
		free(q_conf);
	}

	printf("%d %d\n", total_conf, min_moves);

	free(new_conf);
	free(head);
	free(tail);
	free(sets);
}


Q_node *pop(Q_node *head){
	Q_node *top = head;
	head = head->next;
	return top;
}
void push(Q_node *tail, short *conf, int n, int moves){
	Q_node *new_node = malloc(sizeof(Q_node));

	new_node->next = NULL;
	new_node->moves = moves;
	new_node->Conf = malloc(sizeof(short)*n);
	memcpy(new_node.Conf, conf, n);

	tail->next = new_node;
	tail = new_node;
}