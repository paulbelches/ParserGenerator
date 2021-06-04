import random

NUMBERS = list(map(chr, range(48, 58)))
WHITE = [chr(13), chr(9), chr(10), ' ']
SIGN = ['-', '']
OPERATOR = ['+', '-', '/', '*']


def getNum(min, max):
    return str(random.randint(min, max))

def getFloat(min, max):
    return str(random.uniform(min, max))

def randomString(listOfElements, length, separator, functionFlag):
    result = ""
    l = len(listOfElements) - 1
    for i in range(length):
        if (functionFlag):
            result += separator + listOfElements[random.randint(0, l)]()
        else:
            result += separator + listOfElements[random.randint(0, l)]
    return result

def writeFile(fileName, data):
    f = open(fileName, "w")
    f.write(data)
    f.close()

def number():
    return getNum(1, 10000)

def decnumber():
    return getFloat(1, 10000)

def white():
    return randomString(WHITE, 1, '', False)

def signnumber():
    return randomString(SIGN, 1, '', False) + number()

def operator():
    return randomString(OPERATOR, 1, '', False)

def operation():
    return randomString([number, decnumber, operation], 1, '', True) + operator() + randomString([number, decnumber, operation], 1, '', True) 

def operationError():
    return randomString([number, decnumber, operation, white], 1, '', True) + operator() + operator() + randomString([number, decnumber, operation], 1, '', True) 

DOUBLEATG = randomString([operation], 1000, ';',True)
DOUBLEERRORATG = randomString([operationError], 1000, ';',True)

writeFile("double.txt", operation() + DOUBLEATG + ";.")
writeFile("doubleErrors.txt", operation() + DOUBLEERRORATG + ";.")
