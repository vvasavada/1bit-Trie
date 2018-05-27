import random

f1 = open("prefix_nexthop.txt",'r')
f2 = open("prefix_nexthop_rand.txt",'w')

def makeRandNH():
	ret = ""
	for i in range(4):
		num = random.randint(0,255)
		ret = ret + str(num)
		if i is not 3:
			ret = ret + ':'
	return ret

for line in f1:
	pref = line.split(' ')[0]
	f2.write(pref + ' ' + makeRandNH() + '\n')



f1.close()
f2.close()
