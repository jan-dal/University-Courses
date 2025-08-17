

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configuration.h"

void read_configuration(struct routing_table* routing_tab, struct routing_tree* tree)
{
	uint8_t n;
	scanf("%hhd\n", &n);

	routing_tab->n = n;
	tree->n = n;

	for (uint8_t i = 0; i < n; i++) {
		struct neighbour_node* curr_nnode = &tree->neighbours[i];
		struct node* curr_node = &curr_nnode->nodes[0];
		routing_tab->nodes[i] = curr_node;

		scanf("\n%[^/]/%hhd distance %d", curr_nnode->interface_addr_repr, &curr_nnode->mask, &curr_nnode->distance);

		curr_nnode->up = 1;
		curr_nnode->n = 1;

		int ex = inet_pton(AF_INET, curr_nnode->interface_addr_repr, &curr_nnode->interface_addr);

		if (ex == 0) {
			printf("Invalid ip — could not proceed with the request\n");
			exit(1);
		} else if (ex != 1) {
			perror("An error occured while creating address structure while reading configuration");
			exit(1);
		}
		curr_nnode->net_addr.s_addr = network_address(curr_nnode->interface_addr.s_addr, curr_nnode->mask);

		if(inet_ntop(AF_INET, &curr_nnode->net_addr, curr_nnode->net_addr_repr, sizeof(curr_nnode->net_addr_repr)) == NULL) {
			perror("An error occured while converting address to string");
        	exit(1);
    	}

		curr_node->not_responded = 0;
		curr_node->send_inf = 0;
		curr_node->mask = curr_nnode->mask;
		curr_node->via = curr_nnode;
		curr_node->net_addr.s_addr = curr_nnode->net_addr.s_addr;
		memcpy(curr_node->net_addr_repr, curr_nnode->net_addr_repr, sizeof(curr_nnode->net_addr_repr));
		curr_node->removed = 0;
		curr_node->distance = curr_nnode->distance;
	}
}

void print_routing(struct routing_table* routing_tab)
{
	for (int i = 0; i < routing_tab->n; i++) {
		struct node* node = routing_tab->nodes[i];
		
		if (!node->removed) {
			printf("%s/%u ", node->net_addr_repr, node->mask);

			// Print if it is a neighbour
			if (node->net_addr.s_addr == node->via->net_addr.s_addr) {
				if (!node->via->up) {
					printf("unreachable connected directly\n");
				} else if(node->distance >= INF) {
					printf("distance inf connected directly\n");
				} else {
					printf("distance %u connected directly\n", node->distance);
				}
			} else {
				if (node->distance >= INF) {
					printf("distance inf ");
				} else {
					printf("distance %u ", node->distance);
				}
				printf("via %s\n", node->via->interface_addr_repr);
			}
		}
	}
}

void update_routing(struct routing_table* routing_tab, struct node* rec_node, struct neighbour_node* sender_node)
{
	// Find if info about this network is already present in the routing table 
	struct node* existing_node = find_node(routing_tab, rec_node->net_addr, rec_node->mask);
	
	// Update existing node
	if (existing_node != NULL) {
		// Update distance if the information comes from the same neighbour
		if (existing_node->via == sender_node) {
			existing_node->not_responded = 0;
			if (sender_node->net_addr.s_addr == rec_node->net_addr.s_addr) {
				existing_node->distance = rec_node->distance;
			} else {
				existing_node->distance = rec_node->distance + sender_node->distance;
			}
			return;
		}
		// Remove worse (current) route 
		else if(existing_node->distance > rec_node->distance + sender_node->distance) {
			existing_node->removed = 1;
		} 
		// Existing node is better
		else {
			return;
		}
	}

	// Add a new node to the routing table 
	struct node* new_entry = NULL;
	// Find logically removed node 
	for (uint8_t i=1; i < sender_node->n; i++) {
		if (sender_node->nodes[i].removed) {
			new_entry = &sender_node->nodes[i];
			break;	
		}
	}
	// Take a brand new node
	if (new_entry == NULL) {
		new_entry = &sender_node->nodes[sender_node->n++];
	}

	// Write the node to the routing table
	uint8_t node_added = 0;
	for (uint8_t i=0; i < routing_tab->n; i++) {
		if (routing_tab->nodes[i]->removed) {
			routing_tab->nodes[i] = new_entry;
			node_added = 1;
			break;
		}
	}
	if (!node_added) {
		routing_tab->nodes[routing_tab->n++] = new_entry;
	}

	new_entry->net_addr.s_addr = rec_node->net_addr.s_addr;
	new_entry->mask = rec_node->mask;
	new_entry->distance = rec_node->distance + sender_node->distance;
	new_entry->via = sender_node;
	new_entry->removed = 0;
	new_entry->send_inf = 0;
	new_entry->not_responded = 0;

	if(inet_ntop(AF_INET, &new_entry->net_addr, new_entry->net_addr_repr, sizeof(new_entry->net_addr_repr)) == NULL) {
		perror("An error occured while converting address to string");
		exit(1);
	}
}

void update_distances(struct routing_table* routing_tab, struct routing_tree* tree)
{
	for (uint8_t i=0; i < routing_tab->n; i++) {
		struct node* node = routing_tab->nodes[i];
		if (node->net_addr.s_addr == node->via->net_addr.s_addr && node->via->up) {
			node->distance = node->via->distance;
		}
		if (node->not_responded >= NO_RESPONSE_LIMIT && node->net_addr.s_addr != node->via->net_addr.s_addr) {
			node->distance = INF;
		}
		if (node->send_inf >= BROADCAST_INF && node->net_addr.s_addr != node->via->net_addr.s_addr){
			node->removed = 1;
		}
	}
	// If there is no info in the routing table about direct networks - add 
	for (uint8_t i=0; i < tree->n; i++) {
		struct node* found_node = find_node(routing_tab, tree->neighbours[i].net_addr, tree->neighbours[i].mask);
		if (found_node == NULL) {
			uint8_t added_node = 0;
			tree->neighbours[i].nodes[0].removed = 0;
			for (uint8_t j=0; j < routing_tab->n; j++) {
				if (routing_tab->nodes[j]->removed) {
					routing_tab->nodes[j] = &(tree->neighbours[i].nodes[0]);
					added_node = 1;
					break;
				}
			}
			if (!added_node) {
				routing_tab->nodes[routing_tab->n++] = &(tree->neighbours[i].nodes[0]);
			}
		}
	}
}

uint32_t network_address(uint32_t address, uint8_t mask) 
{
	return ntohl(htonl(address) & (0xffffffff << (32 - mask)));
}

uint32_t broadcast_address(uint32_t address, uint8_t mask)
{
	return ntohl(htonl(address) | (0xffffffff >> mask));
}

struct node* find_node(struct routing_table* routing_tab, struct in_addr addr, uint8_t mask)
{
	struct node* next_node;

	for (uint8_t i=0; i < routing_tab->n; i++) {
		next_node = routing_tab->nodes[i];
		if(next_node->net_addr.s_addr == addr.s_addr && next_node->mask == mask && !next_node->removed) {
			return next_node;
		}
	}
	return NULL;
}

struct neighbour_node* find_neighbour_interface(struct routing_tree* tree, struct in_addr addr)
{
	struct neighbour_node* nnode;

	for (uint8_t i=0; i < tree->n; i++) {
		nnode = &tree->neighbours[i];
		if(nnode->interface_addr.s_addr == addr.s_addr) {
			return nnode;
		}
	}
	return NULL;
}