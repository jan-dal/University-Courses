
#define NO_RESPONSE_LIMIT 2 // Maximum tours to wait for response
#define BROADCAST_INF 3		// Broadcast inf distance for this many tours
#define LISTENING_TIME 5 	// Listen time in seconds
#define INF 16 				// Infinity constant

struct node {
	struct neighbour_node* via;
	struct in_addr net_addr;
	char net_addr_repr[20];
	uint8_t mask;
	uint8_t removed;
	uint8_t send_inf;
	uint8_t not_responded;
	uint32_t distance;
};

struct neighbour_node {
	struct in_addr interface_addr;
	struct in_addr net_addr;
	char interface_addr_repr[20];
	char net_addr_repr[20];
	uint8_t n;
	uint8_t up;
	uint8_t mask;
	uint32_t distance;
	struct node nodes[20];
};

struct routing_tree {
	uint8_t n;
	struct neighbour_node neighbours[20];
};

struct routing_table {
	uint8_t n;
	struct node* nodes[20];
};

void update_distances(struct routing_table* routing_tab, struct routing_tree* tree);
void print_routing(struct routing_table* routing_tab);
void read_configuration(struct routing_table* routing_tab, struct routing_tree* tree);
void update_routing(struct routing_table* routing_tab, struct node* rec_node, struct neighbour_node* sender_node);
uint32_t broadcast_address(uint32_t address, uint8_t mask);
uint32_t network_address(uint32_t address, uint8_t mask);
struct node* find_node(struct routing_table* routing_tab, struct in_addr addr, uint8_t mask);
struct neighbour_node* find_neighbour_interface(struct routing_tree* tree, struct in_addr addr);