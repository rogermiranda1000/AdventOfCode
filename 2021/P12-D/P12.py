import networkx as nx
#import matplotlib.pyplot as plt

file_name = "input.txt"

def stoc(str):
	if str == 'start':	return 0
	if str == 'end':	return 1
	if len(str) == 1:	return ord(str[0])
	return ord(str[0]) << 8 | ord(str[1])

def visitable(G, node, comodin_usado):
	if G.nodes[node]['visited'] == True and (node&((1<<8) - 1) >= ord('a') and node&((1<<8) - 1) <= ord('z')):
		if comodin_usado[0]: return False
		comodin_usado[0] = True
		return True
	return G.nodes[node]['visited'] == False or (node&((1<<8) - 1) >= ord('A') and node&((1<<8) - 1) <= ord('Z'))

def iterate(G, node, comodin_usado):
	tmp = [comodin_usado]
	if not visitable(G, node, tmp): return 0
	if node == 1: return 1
	
	count = 0
	neighbors = list(G.neighbors(node))
	G.nodes[node]['visited'] = True
	for x in range(len(neighbors)-1):
		count += iterate(G.copy(), neighbors[x], tmp[0])
	count += iterate(G, neighbors[len(neighbors)-1], tmp[0])
	
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
	
	print(iterate(G, stoc('start'), False))
	
	#print(*G.neighbors('A'))
	#nx.draw(G, with_labels=True, font_weight='bold')
	#plt.savefig("Graph.png", format="PNG")
