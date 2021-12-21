import networkx as nx

size = 100
file_name = "input.txt"

def ptoi(x, y):
	return (x << 16) | y

if __name__ == '__main__':
	G=nx.DiGraph()
	
	with open(file_name, "r") as file:
		x = 0
		y = 0
		for line in file:
			for e in line[:-1]:
				if x > 0: G.add_edge(ptoi(x-1, y), ptoi(x, y), weight=int(e))
				if y > 0: G.add_edge(ptoi(x, y-1), ptoi(x, y), weight=int(e))
				x += 1
			
			y += 1
			x = 0
		
		path = nx.dijkstra_path(G, ptoi(0, 0), ptoi(size-1, size-1))
		
		risk = 0
		pre = None
		for e in path:
			if pre != None:
				risk += G[pre][e]['weight']
			pre = e
		
		print(risk)
