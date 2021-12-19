import networkx as nx
#import matplotlib.pyplot as plt

file_name = "test.txt"

def stoc(str):
	if str == 'start':	return 0
	if str == 'end':	return 1
	return str[0]

def visitable(G, node):
	return G.nodes[node]['visited'] == False or (isinstance(node, int) == False and node >= 'A' and node <= 'Z')

def iterate(G, node):
	if not visitable(G, node): return 0
	
	count = 1
	neighbors = list(G.neighbors(node))
	G.nodes[node]['visited'] = True
	for x in range(len(neighbors)-1):
		count += iterate(G.copy(), neighbors[x])
	count += iterate(G, neighbors[len(neighbors)-1])
	
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
	
	print(iterate(G, stoc('start')))
	
	#print(*G.neighbors('A'))
	#nx.draw(G, with_labels=True, font_weight='bold')
	#plt.savefig("Graph.png", format="PNG")
