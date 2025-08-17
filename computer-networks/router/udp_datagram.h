
int recieve_udp(int sockfd, struct routing_table* routing_tab, struct routing_tree* tree);
int send_udp_datagram(int sockfd, struct node* node, struct neighbour_node* rec_node);