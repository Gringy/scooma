import sys
import re

codes = []
memory = []
links = {} # string to int
places = {} # int to string 

def importCodes():
	cFile = open("src/opcodes.h", "r")
	for line in cFile:
		m = re.match("^\\s*VM_([_A-Z]+)", line)
		if (m):
			codes.append(m.group(1))

def place():
	for i in range(4):
		memory.append(0)

def compileAt(n, num):
	memory[n  ] = num & 0xFF
	memory[n+1] = (num>>8) & 0xFF
	memory[n+2] = (num>>16) & 0xFF
	memory[n+3] = (num>>24) & 0xFF

def compile(num):
	place()
	compileAt(len(memory)-4, num)

def compile_string(str):
	for ch in str:
		compile(ord(ch))
	compile(0)

def top():
	return len(memory) / 4

def readFile(name):
	asmFile = open(name, "r")
	lineno = 1
	current_header = 0
	for line in asmFile:
		words = line.split()
		if len(words) == 0:
			pass
		elif words[0] == "i":
			if (len(words) <= 1):
				print("too few words in instruction: line {0}".format(lineno))
				exit(1)
			if len(words) > 5:
				print("too much words in instruction: line {0}".format(lineno))
				exit(1)
			compiled = 0
			for word in words[1:]:
				try:
					memory.append(codes.index(word.upper()))
					compiled += 1
				except:
					print("wrong command name {0}: line {1}".format(word, lineno))
					exit(1)
			for i in range(4-compiled):
				memory.append(codes.index("NOP"))
		elif words[0] == "n":
			try:
				num = int(words[1])
			except:
				print("wrong number: line {0}".format(lineno))
				exit(1)
			compile(num)
		elif words[0] == "r":
			link = line[2:].strip()
			places[len(memory)/4] = link
			place()
		elif words[0] == ":":
			link = line[2:].strip()
			links[link] = len(memory)/4
		elif words[0] == "s":
			string = line[2:]
			if len(string) > 0 and string[-1] == "\n":
				string = string[:-1]
			string = string.replace("\\n", "\n")
			compile_string(string)
		elif words[0] == "/":
			pass
		elif words[0] == "CONST":
			if len(words) != 3:
				print("wrong const: line {0}".format(lineno))
				exit(1)
			try:
				links[words[1]] = int(words[2])
			except:
				print("wrong number in const: line {0}".format(lineno))
		elif words[0] == "HEADER":
			# HEADER class:word xt name
			if len(words) != 4:
				print("bad header: line {0}", lineno)
				exit(1)
			word_class = words[1]
			xt = words[2]
			name = words[3]


			links["word" + str(current_header)] = top()
			links["word_last"] = top()
			# link to prev header
			if current_header != 0:
				places[top()] = "word" + str(current_header-1)
			place()
			# word class
			places[top()] = word_class
			place()
			# xt
			places[top()] = xt
			place()
			# name
			compile_string(name)
			current_header = current_header + 1
		else:
			print("unknown directive {0}: line {1}".format(words[0], lineno))
			exit(1)
		lineno += 1

def passLinks():
	for address in places:
		needed = places[address]
		try:
			compileAt(address*4, links[needed])
		except:
			print("there is no links with name {0}".format(needed))
			exit(1)

def writeFile(name):
	outFile = open(name, "wb")
	outFile.write(bytearray(memory))

def main():
	print("asm for scooma v0.0.11")
	if len(sys.argv) == 2:
		importCodes()
		readFile(sys.argv[1])
		passLinks()
		writeFile("image.bin")
		print("compiled {0} cells ({1} bytes)".format(len(memory)/4, len(memory)))
	else:
		print("pass asm file to compile it")
		exit(1)

if __name__ == '__main__':
	main()
