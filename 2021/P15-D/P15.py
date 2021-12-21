import networkx as nx

size = 100
repeat = 5
file_name = "input.txt"

def ptoi(x, y):
	return (x << 32) | y

def itop(p):
	return [p >> 32, p & ((1<<32) - 1)]

if __name__ == '__main__':
	G=nx.DiGraph()
	
	lines = []
	with open(file_name, "r") as file:
		for line in file:
			lines.append(line[:-1])
	
	x = 0
	y = 0
	max_x = max_y = (size*repeat)-1
	for increment2 in range(repeat):
		for line in lines:
			for increment in range(repeat):
				for e in line:
					w = ((int(e)+increment2+increment-1) % 9)+1
					if x > 0: G.add_edge(ptoi(x-1, y), ptoi(x, y), weight=w)
					if x < max_x: G.add_edge(ptoi(x+1, y), ptoi(x, y), weight=w)
					if y > 0: G.add_edge(ptoi(x, y-1), ptoi(x, y), weight=w)
					if x < max_y: G.add_edge(ptoi(x, y+1), ptoi(x, y), weight=w)
					x += 1
					#print(w, end = '')
			#print()
			
			y += 1
			x = 0
	
	print(nx.shortest_path_length(G, ptoi(0, 0), ptoi(max_x, max_y), weight='weight'))
	#print(*map(itop, nx.dijkstra_path(G, ptoi(0, 0), ptoi((size*repeat)-1, (size*repeat)-1))))