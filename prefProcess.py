# keep in mind that this all takes MINUTES to run ... it's probably
# not the most efficient but who cares, it only needs to be done once


# if you don't use ip.txt but want to use the original webpage txt
# then use these three lines below, but this takes a really long time
# so you can also use the ip.txt that I created using these exact lines
import urllib
f = urllib.urlopen("http://bgp.potaroo.net/as2.0/bgptable.txt")
s = f.read()


# read from the txt that I made from the webpage
# f = open("ip.txt",'r')
# s = f.read()

# get the prefixes and next hops line by line as a list
lines = s.split('*')
lines = lines[2:]

# just remove the first couple of characters (*>) to make
# processing simpler
lines_new = list()
for line in lines:
	lines_new.append(line[2:])

# iterate through all of the prefixes and their next hops,
# building up a mapping from prefix to next hop
wr = open("makeSureWorks.txt",'w')
i = 0
while i < 100 :# range(len(lines_new)):
	
	# make a list of each of the fields
	line_split = lines_new[i].split()

	# most of the time, the pattern is prefix line then
	# next hop line but I think there's one off line so
	# need to account for that
	if '/' in line_split[0]:

		# take the prefix
		pref = line_split[0]

		# get the matching next hop
		i = i + 1
		nexthop = lines_new[i].split()[0]

		# and write the mapping to a new text file
		# that can be processed ... takes the form:
		# IP_PREFIX NEXT_HOP_ADDRESS
		wr.write(pref+' '+nexthop+'\n')

	# go to the next prefix
	i = i + 1

wr.close()
f.close()