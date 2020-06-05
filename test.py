def ESIEA3() :
    chaine1 = input("Saisir une première chaine : ")
    chaine2 = input("Saisir une deuxième chaine : ")

    if len(chaine1) > len(chaine2) :
        print(chaine1)
    elif len(chaine1) < len(chaine2) :
        print(chaine2)
    else :
        print(chaine1+chaine2)  

ESIEA3()