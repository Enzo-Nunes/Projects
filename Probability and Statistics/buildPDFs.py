import os
from fpdf import FPDF
from PIL import Image
import sys

def buildPDF(exercise_number):

	# Construct the filenames for the plot image and code file
	plot_dir = f"Ex{exercise_number}/plot.png"
	code_dir = f"Ex{exercise_number}/Ex{exercise_number}.r"
	pdf_dir = f"Ex{exercise_number}/Ex{exercise_number}.pdf"

	# Check if these files exist
	if not os.path.isfile(plot_dir) or not os.path.isfile(code_dir):
		print("One or both of the files do not exist.")
		sys.exit(1)

	# Create a PDF file and add the plot image to it
	pdf = FPDF()
	pdf.add_page()
	pdf.add_font(
		"JetBrains Mono",
		fname="/home/kaiser145/.local/share/fonts/JetBrainsMono/ttf/JetBrainsMono-Regular.ttf",
	)

	# Open the image file and get its size
	with Image.open(plot_dir) as img:
		img_width, img_height = img.size

	# Calculate the height of the image in the PDF
	pdf_image_width = 160  # The width we're going to set for the image in the PDF
	pdf_image_height = pdf_image_width * img_height / img_width  # Keep the aspect ratio

	# Add the image to the PDF
	pdf.image(plot_dir, x=10, y=10, w=pdf_image_width, h=pdf_image_height)

	# Calculate the y-coordinate of the bottom of the image
	y_after_image = 10 + pdf_image_height  # 10 is the y-coordinate where the image starts

	# Open the .r file, read its content, and add it to the PDF as text
	with open(code_dir, 'r') as file:
		code_text = file.read()
	pdf.set_font("Jetbrains Mono", size = 9)
	pdf.set_xy(10, y_after_image)
	pdf.multi_cell(0, 4, code_text)

	# Save the PDF file in the appropriate directory
	pdf.output(pdf_dir)

# Search for dirs of the form Ex<number> and build PDFs for them
for dirpath, dirnames, filenames in os.walk("."):
	for dirname in dirnames:
		if dirname.startswith("Ex"):
			exercise_number = int(dirname[2:])
			buildPDF(exercise_number)