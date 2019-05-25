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


def readFile(name):
	asmFile = open(name, "r")
	lineno = 1
	for line in asmFile:
		words = line.split()
		if len(words) == 0:
			pass
		elif words[0] == "i":
			if (len(words) <= 1):
				print("too few words in instruction: line {}".format(lineno))
				exit(1)
			if len(words) > 5:
				print("too much words in instruction: line {}".format(lineno))
				exit(1)
			compiled = 0
			for word in words[1:]:
				try:
					memory.append(codes.index(word.upper()))
					compiled += 1
				except:
					print("wrong command name {}: line {}".format(word, lineno))
					exit(1)
			for i in range(4-compiled):
				memory.append(codes.index("NOP"))
		elif words[0] == "n":
			try:
				num = int(words[1])
			except:
				print("wrong number: line {}".format(lineno))
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
			str = line[2:]
			if len(str) > 0 and str[-1] == "\n":
				str = str[:-1]
			str = str.replace("\\n", "\n")
			for ch in str:
				compile(ord(ch))
			compile(0)
		elif words[0] == "/":
			pass
		elif words[0] == "C":
			if len(words) != 3:
				print("wrong const: line {}".format(lineno))
				exit(1)
			try:
				links[words[1]] = int(words[2])
			except:
				print("wrong number in const: line {}".format(lineno))
		else:
			print("unknown directive {}: line {}".format(words[0], lineno))
			exit(1)
		lineno += 1

def passLinks():
	for address in places:
		compileAt(address*4, links[places[address]])

def writeFile(name):
	outFile = open(name, "wb")
	outFile.write(bytearray(memory))

def main():
	print("asm for scooma v0.0.3")
	if len(sys.argv) == 2:
		importCodes()
		readFile(sys.argv[1])
		passLinks()
		writeFile("image.bin")
		print("compiled {} cells ({} bytes)".format(len(memory)/4, len(memory)))
	else:
		print("pass asm file to compile it")
		exit(1)

if __name__ == '__main__':
	main()