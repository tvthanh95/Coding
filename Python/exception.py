""" Learn about exception:
"""
def index_power(array,n):
	try:
		return array[n]**n
	except:
		print "Error"
#array = [1,2,3,4,5,6,7,8,9]
#print index_power(array,3)#print 4^3
#print index_power(array,-1)#print 1/9
#print index_power(array,10)#error
num2 = 0
try:
	res = num1 / num2
except NameError:
	print("La variable num1 ou num2 n\'a pas ete definie.")
except ZeroDivisionError:
	print("la variable num2 est egale a zero")
except TypeError:
	print("La variable num1 ou num2 possede un type incompatible avec la division") 
# a new comment
