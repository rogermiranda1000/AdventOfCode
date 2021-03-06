import networkx as nx
#import matplotlib.pyplot as plt

file_name = "input.txt"

def stoc(str):
	if str == 'start':	return 0
	if str == 'end':	return 1
	if len(str) == 1:	return ord(str[0])
	return ord(str[0]) << 8 | ord(str[1])

def visitable(G, node):
	return G.nodes[node]['visited'] == False or (node&((1<<8) - 1) >= ord('A') and node&((1<<8) - 1) <= ord('Z'))

def iterate(G, node, depth):
	if not visitable(G, node): return 0
	#print(('\t'*depth) + str(node))
	if node == 1: return 1
	
	count = 0
	neighbors = list(G.neighbors(node))
	G.nodes[node]['visited'] = True
	for x in range(len(neighbors)-1):
		count += iterate(G.copy(), neighbors[x], depth+1)
	count += iterate(G, neighbors[len(neighbors)-1], depth+1)
	
	return count

if __name__ == '__main__':
	G=nx.Graph()
	
	with open(file_name, "r") as file:
		for line in file:
			[n1,n2] = line.split("-")
			n2 = n2.strip() # remove '\n'
			
			#if n1 not in G: G.add_node(n1)
			#if n2 not in G: G.add_node(n2)
			G.add_edge(stoc(n1), stoc(n2))
			
			G.nodes[stoc(n1)]['visited'] = False
			G.nodes[stoc(n2)]['visited'] = False
	
	print(iterate(G, stoc('start'), 0))
	
	#print(*G.neighbors('A'))
	#nx.draw(G, with_labels=True, font_weight='bold')
	#plt.savefig("Graph.png", format="PNG")
