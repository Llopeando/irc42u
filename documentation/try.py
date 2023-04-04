import sys
import PyPDF2

def main():
	pdf = PyPDF2.PdfFileWriter()
	page = pdf.addBlankPage(612, 792)  # 8.5 x 11 inches (letter size)
	with open('output.pdf', 'wb') as file:
		pdf.write(file)


if __name__ == '__main__':
	main()