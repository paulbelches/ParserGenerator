import random

NUMBERS = list(map(chr, range(48, 58)))
CAPLETTERS = list(map(chr, range(65, 90)))
LOWLETTERS = list(map(chr, range(97, 123)))
WHITE = [chr(13), chr(9), chr(10), ' ']
SIGN = ['+', '-', '']
ARITMETICAKEYWORDS = ["while", "do", "if", "switch"]
HEXNUMBERKEYWORDS = ["while", "do"]

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
    return getNum(0, 10000)

def decnumber():
    return getFloat(0, 10000)

def ident():
    return randomString(NUMBERS + CAPLETTERS + LOWLETTERS, 5, '', False)

def hexnumber():
    return randomString(NUMBERS + CAPLETTERS[0:6], 5, '', False) + 'H'

def white():
    return randomString(WHITE, 1, '', False)

def signnumber():
    return randomString(SIGN, 1, '', False) + number()

def keyHex():
    return randomString(HEXNUMBERKEYWORDS, 1, '', False)

def keyAri():
    return randomString(ARITMETICAKEYWORDS, 1, '', False)

HEXNUMBERTATG = randomString([ident, hexnumber, number, signnumber, white, keyHex], 10000, ' ',True)
DOUBLEATG = randomString([number, decnumber, white], 10000, ' ',True)
ARITMETICAATG = randomString([number, decnumber, white, keyAri, ident], 10000, ' ',True)

#print("Hex\n", HEXNUMBERTATG)
#print("double\n", DOUBLEATG)
#print("ari\n", ARITMETICAATG)

writeFile("hex.txt", HEXNUMBERTATG)
writeFile("double.txt", DOUBLEATG)
writeFile("ari.txt", ARITMETICAATG)
