import matplotlib.pyplot as plt

def varToString(var):
	return ord(var[0])-ord('w')

# retorna un array de lambdas que executen les comandes
# les lambdas reben dos arguments: l'array de variables, i array d'inputs (les lambdes poden modificar els dos)
def getInstructions(file_name):
	r = []
	
	with open(file_name) as file:
		for line in file:
			l = line[:-1].split()
			if l[0] == "inp":
				index = varToString(l[1])
				r.append( lambda variables, inputs : (
								exec(f"variables[{index}] = inputs[0]"),
								inputs.pop(0)) )
			elif l[0] == "eql":
				index1 = varToString(l[1])
				
				try:
					index2 = int(l[2])
					r.append( lambda variables, inputs : exec(f"variables[{index1}] = 1 if (variables[{index1}] == {index2}) else 0"))
				except ValueError:
					index2 = varToString(l[2])
					r.append( lambda variables, inputs : exec(f"variables[{index1}] = 1 if (variables[{index1}] == variables[{index2}]) else 0"))
				
			else:
				index1 = varToString(l[1])
				if l[0] == "add":
					operation = "+"
				elif l[0] == "mul":
					operation = "*"
				elif l[0] == "div":
					operation = "/"
				elif l[0] == "mod":
					operation = "%"
				
				try:
					index2 = int(l[2])
					if index2 == 1 and (operation == "*" or operation == "/"): continue
					if index2 == 0 and operation != "*": continue
					r.append( lambda variables, inputs : exec(f"variables[{index1}] {operation}= {index2}"))
				except ValueError:
					index2 = varToString(l[2])
					r.append( lambda variables, inputs : exec(f"variables[{index1}] {operation}= variables[{index2}]"))
	
	return r

def subtractOne(input):
	l = len(input)-1
	
	while l >= 0:
		if input[l] == 1:
			input[l] = 9
			l -= 1
		else:
			input[l] -= 1
			return True
	
	return False

if __name__ == '__main__':
	instructions = getInstructions('input.txt')
	
	variables = [0, 0, 0, 0] # w, x, y, z
	input = [9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9]
	result = []
	try:
		while True:
			for ins in instructions:
				ins(variables, input[:])
			
			result.append(variables[varToString("z")])
			print(input)
			print(variables[varToString("z")])
			if variables[varToString("z")] == 0:
				print(input)
				break
			
			if not subtractOne(input):
				break
	except KeyboardInterrupt:
		pass
	
	plt.plot(range((99999999999999+1) - len(result), 99999999999999+1), result)
	plt.xlabel('input')
	plt.ylabel('result')
	plt.savefig('result.png')
