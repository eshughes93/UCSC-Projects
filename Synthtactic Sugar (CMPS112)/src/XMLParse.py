# CMPS 112 WINTER 2015 FINAL PROJECT
# AUTHORS: Evan Hughes, Carl Eadler, Hadley Black
# XML Parser for Synthtactic Sugar

import re
import codecs
import sys

def tripleToNote(t):
    # print (t[0], t[1], t[2])
    partOfSpeech = t[1]
    posCode = 0 if (partOfSpeech == 'noun')            else (
                 1 if (partOfSpeech == 'verb')            else (
                 2 if (partOfSpeech == 'adverb')         else (
                 3 if (partOfSpeech == 'preposition')  else (
                 4 if (partOfSpeech == 'adjective')     else (
                 5 if (partOfSpeech == 'pronoun')        else (
                 6 if (partOfSpeech == 'interjection') else (
                 7 if (partOfSpeech == 'conjunction')  else (
                 8))))))))
    comma = (t[2] == ',')
    period = (t[2] == '.')
    last = 3 if period else (2 if comma else 1)
    return '('+ '/'.join([str([posCode]), ':'.join(str([1]*(t[0]-1)+[last]).split(', '))])+')'
    


myTriple = (3, 'verb', '.')
# print (myTriple)
# print (0, "note:", tripleToNote(myTriple))

def matchDef(queryWord, XMLString):
    defs = (re.compile('<title>')).split(XMLString)
    for s in defs:
        if (re.compile(queryWord)).match(s) is not None:
            return s
    return defs[1] if (len(defs) > 1) else '' #default case if nothing else matches
            
def getTag(tag, definition):
    try:
        splits = re.split('<%s>'%tag, definition)
        #print('splits:', splits[1])
        splits2 = re.split('<\\\\/%s>'%tag, splits[1]) if (len(splits) > 1) else []
        #print('splits2:', splits2[0])
        m = (re.match(ur'([^<]+)', splits2[0])) if (len(splits2) > 0) else None
        # print('splits2:', splits2[0])
        x = reduce((lambda x, y: x + y), (re.split('<.*?>', splits2[0])))
        #print(x)
        return x
    except IndexError:
        return 'verb'


def syllableCount(IPASpelling, vowelTuples):
    state = (IPASpelling, 0) #initial state: Original spelling, 0 syllables detected
    for vt in vowelTuples:
        vowelPattern = vt[0]
        vowelVal = vt[1]
        def foldState(newData, curState, vowelValue):
            #Things are spelled out in this function so it's not so damn obfuscated
            modSpelling = newData[0]            #modified spelling after replacement
            numRepl = newData[1]*vowelValue  #number of replacements * vowelValue
            accSylCount = curState[1]          #accumulated number of syllables
            return (modSpelling,  numRepl + accSylCount)
        state = foldState(re.subn(vowelPattern, repr(vowelVal), state[0]), state, vowelVal)
        #state = (lambda t1, t2: (t1[0], t1[1]*vt[1]+t2[1]))(re.subn(vt[0], repr(vt[1]), state[0]), state)
        # print ("State!", repr(state), repr(vt))
    return state[1]
 
vowels = ur'ɑr, ɒr, ær, aɪər, aɪ.ər, aʊər, aʊ.ər, ɛr, ɛər, eɪ.ər, ɪr, ɪər, i.ər, ɔr, ɔ.ər, ɔɪər, ɔɪ.ər, ɔər, oʊ.ər, ʊr, ʊər, u.ər, jʊər, ju.ər, ɜr, ʌr, ɑɪ, ɑ, ɒ, æ, aɪ, aʊ, ɛ, eɪ, ɪ, i, ɔ, ɔɪ, oʊ, ʊ, u, ju, ʌ, ər, ən, əm, əl, ᵊ, ə, ɨ, ɵ, ʉ, i, a, e, i, o, u'.split(', ')
vowelSyl = [1,  1,  1 , 2   , 2    , 2   , 2    , 1 , 1  , 2    , 1 ,   1,    2,  1,    2, 2   , 2    , 1  , 2    , 1 , 1  , 2   , 1   , 2    , 1 , 1 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
VS = zip(vowels, vowelSyl)


testWord = ur'ˈfɑɪt̬·ɪŋ'
# print(syllableCount(testWord, VS))

def syllableStack(word, getXML):
    XMLString = getXML(word)
    definition = matchDef(word, XMLString)
    IPASpelling = getTag('ipa', definition)
    partOfSpeech = getTag('pos', definition)
    return (syllableCount(IPASpelling, VS), partOfSpeech.encode('ascii', 'ignore'))
