import random;
import sys
orig_stdout = sys.stdout
f = open('custom_data.h', 'w')
sys.stdout = f

#Initialize array with +-0.5 float
sys.stdout.write('float CUSTOM_DATA_M05_P05_F[1000] = {')
def multiples():
    lis = []
    for x in range(0, 1000):
	num = random.randint(-1000,1000)
	num = num/1000.0*0.5
	lis.append(str(num))
    print ','.join(lis),
multiples()
print "};"


#Initialize array with +-1 float
sys.stdout.write('float CUSTOM_DATA_M1_P1_F[1000] = {')
def multiples():
    lis = []
    for x in range(0, 1000):
	num = random.randint(-1000,1000)
	num = num/1000.0
	lis.append(str(num))
    print ','.join(lis),
multiples()
print "};"

#Initialize array with +-1 double
sys.stdout.write('double CUSTOM_DATA_M1_P1_D[1000] = {')
def multiples():
    lis = []
    for x in range(0, 1000):
	num = random.randint(-1000,1000)
	num = num/1000.0
	lis.append(str(num))
    print ','.join(lis),
multiples()
print "};"

#Initialize array with +-32767 double
sys.stdout.write('int CUSTOM_DATA_M32768_P32767_I[1000] = {')
def multiples():
    lis = []
    for x in range(0, 1000):
	num = random.randint(-32768,32767)
	lis.append(str(num))
    print ','.join(lis),
multiples()
print "};"

#Initialize array with +-10000 double
sys.stdout.write('float CUSTOM_DATA_M10k_P10k_F[1000] = {')
def multiples():
    lis = []
    for x in range(0, 1000):
	num = random.randint(-10000,10000)
	lis.append(str(num))
    print ','.join(lis),
multiples()
print "};"

#Initialize array with +-10000 double
sys.stdout.write('double CUSTOM_DATA_M10k_P10k_D[1000] = {')
def multiples():
    lis = []
    for x in range(0, 1000):
	num = random.randint(-10000,10000)
	lis.append(str(num))
    print ','.join(lis),
multiples()
print "};"

sys.stdout = orig_stdout
f.close()
