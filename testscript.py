#Autor: Martin Hlinský (xhlins01)
#Usage: python3 testscript.py [-h/--help] / [-f/--folder <folder>] [-r]
#Bude spustitelny POUZE pomoci Linuxu / WSL, protoze bude volat ./ifj21, nikoliv ifj21.exe

import argparse
import os

parser = argparse.ArgumentParser(description='IFJ projekt: Testovací skript', add_help=False)
parser.add_argument('-h', '--help', action='help', default=argparse.SUPPRESS, help='Vypíše manuál a ukončí běh programu.')
parser.add_argument('-f', '--folder', metavar='folder', help='Složka, která se bude prohledávat.')
parser.add_argument('-r', '--recursive', help='Flag určující, zda se složka prohledává rekurzivně', action="store_true", default=False)
args = parser.parse_args()

folder = args.folder
recursiveFlag = args.recursive

if(folder is None):
        folder = "."
else:
    if(not os.path.isdir(folder) or not os.path.exists(folder)):
        print("Složka neexistuje, nebo byla zadána špatně.")
        exit(1)

#print(f"folder: {folder}")
#print(f"recursiveFlag: {recursiveFlag}")

def test(root, testFile, outDict):
    testFileRc = os.path.join(root, testFile[:-3] + ".rc")
    testFile = os.path.join(root, testFile)
    
    if os.path.isfile(testFileRc):
        with open(testFileRc, 'r') as f:
            try:
                returnCode = int(f.read())
                print(returnCode)
            except ValueError:
                print("Nepovedlo se přečíst soubor .rc")
                exit(2)
    else:
        returnCode = 0

    # TODO Jak bude udelany nejaky codegen nebo neco co se muze porovnat s .out soubory
    # tak bude podminka -> jestli je returnCode 0, tak probehne porovnani vystupu -xhlins01

    returnCodeOur = os.system(f"./ifj21 <{testFile} >outTemp 2>errTemp")
    returnCodeOur = returnCodeOur >> 8

    if(returnCodeOur != returnCode):
        outDict[testFile] = f"Očekávaný návratový kód: {returnCode}. Skutečný návratový kód: {returnCodeOur}"

def main():
    outDict = dict()
    numberOfTests = 0

    if(recursiveFlag):
        for root, _, files in os.walk(folder):
            for testFile in files:
                if testFile.endswith(".tl"):
                    numberOfTests += 1
                    test(root, testFile, outDict)              
    else:
        for testFile in os.listdir(folder):
            if testFile.endswith(".tl"):
                numberOfTests += 1
                test(folder, testFile, outDict)
    
    os.system("rm outTemp errTemp")
    
    print(f"{numberOfTests - len(outDict)}/{numberOfTests} testů proběhlo úspěšně.")
    if(len(outDict) > 0):
        for key in outDict.keys():
            print(f"V testu \"{key}\" nastala chyba: {outDict[key]}.")

if __name__ == "__main__":
    main()