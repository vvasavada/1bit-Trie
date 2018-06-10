import csv
import matplotlib.pyplot as plt

from argparse import ArgumentParser

# parser = ArgumentParser()
# parser.add_argument("infile", type=str, help="CSV FILE")
# args = parser.parse_args()
# csv_file = args.infile
# success_averages = [0] * 25
# fail_averages = [0] * 25

# just a way of computing the average in step just in case
# of weird overflow / precision errors
def runAve(last, val, numVals):
	return last + ((val - last) / numVals)

def make_averages(csv_string):
	averages = [0] * 25
	with open(csv_string,'r') as f:
		reader = csv.reader(f)

		# skip first row, the text
		reader.next()

		num_rows = 0

		for row in reader:
			num_rows = num_rows + 1
			i = 0
			for time in row:
				averages[i] = runAve(averages[i], float(time), num_rows)
				i = i + 1

	return averages

point1 = make_averages('csv_files/successResults_point1.csv')
point2 = make_averages('csv_files/successResults_point2.csv')
point5 = make_averages('csv_files/successResults_point5.csv')
point8 = make_averages('csv_files/successResults_point8.csv')

# with open('failResults.csv','r') as f:
# 	reader = csv.reader(f)

# 	# skip first row, the text
# 	reader.next()

# 	num_rows = 0

# 	for row in reader:
# 		num_rows = num_rows + 1
# 		i = 0
# 		for time in row:
# 			fail_averages[i] = runAve(fail_averages[i], float(time), num_rows)
# 			i = i + 1

# averages = []

# for i in range(0,len(fail_averages)):
# 	averages.append(fail_averages[i] + success_averages[i])


# plot
plt.plot(point1, 'b', label='0.1')
plt.text(24, 440, '0.1')

plt.plot(point2, 'y', label='0.2')
plt.text(24, 460, '0.2')

plt.plot(point5, 'g', label='0.5')
plt.text(24, 467, '0.5')

plt.plot(point8, 'r', label='0.8')
plt.text(24, 515, '0.8')

plt.xlabel('Hint Length')
plt.ylabel('Search Time (ns)')
plt.show()
