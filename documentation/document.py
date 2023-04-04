import sys
import PyPDF2

class CustomPDF(object):
	def __init__(self, seed = 0):
		self.seed = seed
	def setSeed(self, seed):
		self.seed = seed

	@staticmethod
	def checkSeed(seed):
		if seed < 0 or seed > 2:
			return True
		else:
			return False
	def loadPDF(self, seed = -1):
		if seed is not -1:
			self.seed = seed
		

def convertSeed(seed):
	try:
		intSeed = int(seed)
	except ValueError:
		print("Error: seed: seed was not an intiger number")
		return -1
	if CustomPDF.checkSeed(intSeed):
		print("Error: seed: invalid seed.")
		intSeed = 0
	return intSeed

def initShell(customPDF, lauch):
	if lauch == True:
		customPDF.loadPDF()
	while True:
		cmd = input("< ").split()
		if cmd[0] == "exit":
			sys.exit()
		elif len(cmd) == 3 and cmd[0] == "change" and cmd[1] == "seed":
			seed = convertSeed(cmd[2])
			if seed is not -1:
				customPDF.loadPDF(seed)
		elif len(cmd) > 2 and cmd[0] == "load" and cmd[1] == "PDF":
			customPDF.loadPDF()

def main():
	lauch = False
	seed = 0
	if len(sys.argv) > 1:
		seed = convertSeed(sys.argv[1])
		if seed == -1 or CustomPDF.checkSeed(seed):
			seed = 0
		else:
			lauch = True
	customPDF = CustomPDF(seed)
	initShell(customPDF, lauch)


if __name__ == '__main__':
	main()