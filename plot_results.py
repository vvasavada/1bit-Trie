import csv
import matplotlib.pyplot as plt

from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument("infile", type=str, help="CSV FILE")
args = parser.parse_args()
csv_file = args.infile
success_averages = [0] * 25
fail_averages = [0] * 25

# just a way of computing the average in step just in case
# of weird overflow / precision errors
def runAve(last, val, numVals):
	return last + ((val - last) / numVals)

with open(csv_file,'r') as f:
	reader = csv.reader(f)

	# skip first row, the text
	reader.next()

	num_rows = 0

	for row in reader:
		num_rows = num_rows + 1
		i = 0
		for time in row:
			success_averages[i] = runAve(success_averages[i], float(time), num_rows)
			i = i + 1


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
plt.plot(success_averages)
plt.xlabel('Hint Length')
plt.ylabel('Search Time (ns)')
plt.show()
