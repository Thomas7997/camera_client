#!/bin/python3.5

nb = 2

def addcat (liste, start) :
    x = 0
    words = [0]*100
    for i in range(start, start+nb+1, 1) :
        print(i)
        words[x] = liste[i]
        x += 1
    return words

def compare_each (liste, val, nb) :
    for i in range(0, nb) :
        if liste[i] != val :
            return False
    return True

def compare (l1, l2, n, x) :
    x1 = 0
    x2 = 0
    y = 0
    keys = []
    words = []
    f = open("keys.txt", "a")
    f.write("\n\n")
    if l1[0] == '' :
        return 0
    else :
        for x1 in range(nb*x, nb*(x+1)+1, 1) :
            words = addcat(l1, x1)
            for x2 in range(0, n) :
                if compare_each(words, l2[x2], x2+1) :
                    # Sauvegarder la clé
                    f.write("CLÉ : " + l2[x2] + " | INDEX : " + str(x2))
                    break
                elif x2 % nb == 0 :
                    y += 1
            return compare(l1, l2, n, x+1)

img1 = []
img2 = []
img3 = []

f1 = open("img1.txt", "r")
f2 = open("img2.txt", "r")
f3 = open("img3.txt", "r")

line = f1.readline(2000000)
img1 = line.split(" ")
line = f2.readline(2000000)
img2 = line.split(" ")
line = f3.readline(2000000)
img3 = line.split(" ")

compare(img1, img2, len(img2), 0)


