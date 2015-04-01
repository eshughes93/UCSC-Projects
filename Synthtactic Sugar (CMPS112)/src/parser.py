# CMPS 112 WINTER 2015 FINAL PROJECT
# AUTHORS: Evan Hughes, Carl Eadler, Hadley Black
# Python Parser of English Text for SYNTHTACTIC SUGAR

import sys, requests, codecs
import XMLParse
import toIML

#some global vars
# merriamw_url = 'http://www.dictionaryapi.com/api/v1/references/collegiate/xml/'
# mw_api_key = '?key=67ef1dea-4395-448a-9958-4816b821ae76'
c_url = 'https://dictionary.cambridge.org/api/v1/dictionaries/american-english/search/first'
cambridge_url = 'https://dictionary.cambridge.org/api/v1/dictionaries/american-english/search/first/?q='
c_url_end = '&format=html'
cb_api_key = 'Uf5IusEscXRnsV1l8MWY8HJ7IQ5WCI5svL0QSwuboSl7gDIvad74rJg4kjKnQmbP'
#dictionary to store word:char_following from input
wordchars = {}
wordlist = []
tups = []
needdefs = []
#saves xml to a file
def save_xml(definition, word):
    fname = '../defs/' + word.lower() + '_def.xml'
    f = open(fname, 'w')
    f.write(definition)
    f.close

#gets xml from local directory defs
def get_xml(word):
    try:
        fname = '../defs/' + word + '_def.xml'
        f = codecs.open(fname, 'r', 'utf-8')
        return f.read()
    except IOError:
        print ("Definition doesn't exist for \"" + word + "\", using default.")
        return codecs.open('../defs/default_def.xml', 'r', 'utf-8').read()

#get definition via cambridge API for provided word
def get_c_def(word):
    cambridge_header = {'accessKey' : cb_api_key}
    payload = {'format' : 'xml' , 'q' : word}
    lookup = c_url
    response = requests.get(lookup,params=payload, headers=cambridge_header)
    #print(response.url);
    #print(response.headers);
    if response.status_code == 200:
        #print(response.status_code)
        save_xml(response.text.encode('utf-8'),word)
    else:
        print(response.status_code, "No definition found for \"" + word + "\" in the Cambridge Dictionary API")   


#gets def with api call of given word from merriam webster
# NOT BEING USED  
# def get_def(word):
#     lookup = merriamw_url + word + mw_api_key
#     response = requests.get(lookup)
#     if response.status_code == 200:
#         print(response.text)
#     else:
#         print("Bad response!")

#gets the definitions of all words in wordlist
def get_wordlist_defs():
    for word in wordlist:
        if not has_def_file(word):
            get_c_def(word)
            print("Get definition for: " + word)


#check if word definition has already been stored
def has_def_file(word):
    fname = '../defs/' + word + '_def.xml'
    try:
        f = open(fname)
        f.close()
        return True;
    except IOError:
        return False;

#parses file one line at a time
#adds words to wordchars and wordlist
#notes char that comes after the word (punctuation, whitespace)
def parseline(line):
    punctuation = ".:;,!?-() "
    word = ''
    punct_after = ''
    for i in line:
        if not(i in punctuation):
            word = word + i.lower()
        else:
            punct_after = i
            wordlist.append(word)
            wordchars.update({word:punct_after})
            word = ''
    clearempty()

def clearempty():
    for i in wordlist:
        if(i == ''):
            wordlist.remove(i)

#Opens a file and passes lines to parseline
def openfile(fname):
    try:
        f = open(fname)
        for line in f:
            parseline(line)
        f.close
    except IOError:
        print("This file does not exist.")

def maketuples():
    for i in wordlist:
        tups.append(maketuple(i))

def maketuple(word):
    ch = wordchars[word]
    tup = XMLParse.syllableStack(word, get_xml)
    r = (tup[0], tup[1], ch)
    return r

def main():
    if len(sys.argv) >= 2:
        openfile(sys.argv[1])
        key = sys.argv[2]
        scale = sys.argv[3]
        tempo = sys.argv[4]
        voices = sys.argv[5]
        get_wordlist_defs()
        maketuples()
        toIML.toIML(tups, key, scale, tempo, voices)
    else:
        print("No arguments")

if __name__ == '__main__':
  main()

