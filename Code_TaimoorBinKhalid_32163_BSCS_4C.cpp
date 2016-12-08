#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>

using namespace std;

int longestChainLen = 0;
vector<vector<string>> longestChains;
vector<int> frequencyDistribution;

class Words {
	// declaring private variables
private:
	struct ListNode {
		string word;
		vector<string> wordsChain;
		struct ListNode *next;
	};
	ListNode *head;
	static int lengthDict;

	// declaring public variables
public:

	Words() : head(NULL) {}											// constructor
	~Words(void);													// destructor

	void insertNode(string word);									// inserts node
	int getLength() const;
	vector<string> searchByWord(string word);

	void createChain();
	void printWordsChains();

	void findChains();
	void tracePath(string word1, string word2);

	bool isEmpty() const;
};

void Words::insertNode(string word) {
	ListNode *newNode, *nodePtr;
	newNode = new ListNode;

	newNode->word = word;
	newNode->wordsChain.push_back(word);

	nodePtr = head;
	newNode->next = head;

	this->lengthDict++;

	head = newNode;
}

int Words::getLength() const {
	return this->lengthDict;
}

vector<string> Words::searchByWord(string word) {
	bool check = true;
	int condition;

	ListNode *nodePtr;
	nodePtr = this->head;

	while (nodePtr) {
		condition = nodePtr->word.find(word);

		if (condition != -1) {	
			check = false;
			break;
		}

		nodePtr = nodePtr->next;
	}

	if (check) {
		vector<string> temp;
		temp.push_back("-1");
		return temp;
	}

	return nodePtr->wordsChain;
}

void Words::createChain() {
	int charDifLen;
	bool check;

	ListNode *nodePtr, *anotherPtr;
	nodePtr = head;

	if (this->isEmpty()) {										// checks if list is empty
		cout << "There are no nodes in the list.\n";
		return;
	}

	cout << "\t\t\t\t[TASK: 2]\nIdentifying chains, please wait..." << endl;

	while (nodePtr) {
		anotherPtr = head;
		
		while (anotherPtr) {
			check = true;
			charDifLen = 0;
			
			if (nodePtr->word.length() == anotherPtr->word.length() && nodePtr->word != anotherPtr->word) {
				for (int i = 0; i < anotherPtr->word.length(); i++) {
					if (nodePtr->word[i] != anotherPtr->word[i]) {
						charDifLen++;

						if (charDifLen > 1) {
							check = false;
							break;
						}
					}
				}
				if (check)
					nodePtr->wordsChain.push_back(anotherPtr->word);
			}

			anotherPtr = anotherPtr->next;
		}
		nodePtr = nodePtr->next;
	}
	cout << "Chains identified successfully." << endl;

	return;
}

void Words::printWordsChains() {
	vector<string>::iterator chainIterator;

	ListNode *nodePtr;
	nodePtr = head;

	cout << "Printing chains..." << endl;

	while (nodePtr) {
		for (chainIterator = nodePtr->wordsChain.begin();
			chainIterator != nodePtr->wordsChain.end();
			chainIterator++) {
			cout << *chainIterator << " -> ";
		}
		cout << "END" << endl;

		nodePtr = nodePtr->next;
	}

	cout << "Successfully printed all the chains." << endl;

	return;
}

void Words::findChains() {
	ListNode *nodePtr, *anotherPtr;
	nodePtr = this->head;

	while (nodePtr) {
		anotherPtr = this->head;

		while (anotherPtr) {
			if (nodePtr->word != anotherPtr->word)
				this->tracePath(nodePtr->word, anotherPtr->word);
			anotherPtr = anotherPtr->next;
		}
		nodePtr = nodePtr->next;
	}

	return;
}

void Words::tracePath(string word1, string word2) {
	ofstream wS("wordsSolvable.txt", std::ios_base::app);
	ofstream wNS("wordsNotSolvable.txt", std::ios_base::app);
	ofstream cSW("chainsSolvableWords.txt", std::ios_base::app);

	vector<string>::iterator chainIterator;
	vector<string>::iterator chainIterator2;
	vector<string>::iterator chainIterator3;

	vector<string> fullPath;

	ListNode *nodePtr;
	nodePtr = head;

	int previousIndex = -1;
	string startWord;

	startWord = word1;

	if (word1.length() != word2.length()) {
		//cout << endl << "Words should be of same length." << endl;			// (testing purpose)
	}
	else {
		bool check, check2, check3, found, unwanted;
		int charSameLen;

		found = false;
		fullPath.push_back(word1);

		while (nodePtr) {
			check = true;
			charSameLen = 0;
			unwanted = false;

			if (nodePtr->word == word1) {
				for (chainIterator = nodePtr->wordsChain.begin();
					chainIterator != nodePtr->wordsChain.end();
					chainIterator++) {
					int wordLen;
					string temp;

					check2 = false;

					temp = *chainIterator;
					wordLen = temp.length();

					for (chainIterator2 = fullPath.begin();
						chainIterator2 != fullPath.end();
						chainIterator2++) {
						if (*chainIterator2 == temp) {
							check2 = true;
							break;
						}
					}

					if (check2)
						continue;

					for (int i = 0; i < wordLen; i++) {
						if (temp[i] == word2[i] && word1[i] != word2[i]) {	
							check3 = true;
							for (int k = 0; k < i; k++) {
								if (temp[k] != word1[k]) {
									check3 = false;
									break;
								}
							}
							if (check3) {
								charSameLen++;
							}
						}
					}

					if (charSameLen > 0) {
						for (int i = 0; i < word1.length(); i++) {
							if (temp[i] != word1[i] && i == previousIndex) {
								unwanted = true;
								break;
							}

							if (temp[i] != word1[i] && i != previousIndex)
								previousIndex = i;
						}
						if (unwanted) {
							fullPath.pop_back();
							unwanted = false;
						}
						
						fullPath.push_back(temp);
						word1 = temp;

						if (word1 == word2)
							found = true;

						check = false;

						nodePtr = head;
						break;
					}
				}
			}

			if (found)
				break;

			if (check)
				nodePtr = nodePtr->next;
		}


		if (found) {

			if (wS.is_open())
				wS << "(" << startWord << ", " << word2 << ")\n";
			else
				cout << "File wordsSolvable.txt couldn't open." << endl;
			wS.close();

			int localLen = 0;

			if (cSW.is_open()) {
				cSW << "(" << startWord << ", " << word2 << ")\t\t";
				for (chainIterator3 = fullPath.begin();
					chainIterator3 != fullPath.end();
					chainIterator3++) {
					cSW << *chainIterator3 << " -->> ";
					localLen++;
				}
				cSW << "END" << endl;

				if (localLen > longestChainLen) {
					longestChainLen = localLen;
					longestChains.clear();
					frequencyDistribution.resize(longestChainLen, 0);
				}

				frequencyDistribution[localLen - 1]++;

				if (localLen == longestChainLen) 
					longestChains.push_back(fullPath);
			}
			else
				cout << "File chainsSolvableWords.txt couldn't open." << endl;
			cSW.close();
		}
		else {
			if (wNS.is_open())
				wNS << "(" << startWord << ", " << word2 << ")\n";
			else
				cout << "File wordsNotSolvable.txt couldn't open." << endl;
			wNS.close();
		}
	}
	return;
}

bool Words::isEmpty() const {
	ListNode *nodePtr;
	nodePtr = this->head;

	if (!nodePtr)										// checks if list is empty
		return true;
	return false;
}

Words::~Words(void) {
	ListNode *nodePtr, *anotherPtr;
	nodePtr = head;

	if (this->isEmpty()) {								// checks if list is empty
		this->lengthDict = 0;
		return;
	}
	else {

		while (nodePtr->next != NULL) {					// deletes entire list
			this->lengthDict--;
			anotherPtr = nodePtr;
			nodePtr = nodePtr->next;
			delete anotherPtr;
		}
		this->lengthDict--;
		anotherPtr = nodePtr;							// sets head to NULL
		delete anotherPtr;
		head = NULL;
	}
}

int Words::lengthDict = 0;

void main() {
	string word, temp1;
	string previousWord = "";
	size_t pos;
	
	Words dictionary;

	bool check, check2;
	int countSemi;

	ifstream file("testDictionary.txt");
	if (file.is_open()) {
		cout << "\t\t\t\t[TASK: 1]\nWords are being populated in linked list, please wait..." << endl;
		while (file.good()) {
			getline(file, word, '\n');

			check2 = false;
			countSemi = 0;

			if (word.length() > 0)
				check = true;
			else
				check = false;
			for (int i = 0; i < word.length(); i++) {
				if (!((word[i] < 65 || word[i] > 90) && (word[i] < 97 || word[i] > 122)) || word[i] == 59 || word[i] == 45 || word[i] == 32) {
					if (word[i] == 59) {
						check2 = true;
						countSemi++;
						i++;
					}

					else if (!isupper(word[i]) && !(word[i] == 59 || word[i] == 45 || word[i] == 32)) {
						check = false;
						break;
					}
				}
				else {
					check = false;
					break;
				}
			}
			if (check) {

				if (check2) {
					for (int k = 0; k < countSemi; k++) {
						pos = word.find("; ");
						temp1 = word.substr(0, pos);

						if (previousWord != temp1) {
							dictionary.insertNode(temp1);
							//countWords++;			// to print words stored in dictionary (for testing purpose)
							previousWord = temp1;
						}
						word = word.substr(pos + 2);
					}
				}

				if (previousWord != word && previousWord[0] <= word[0]) {
					//cout << word << endl;			// to print words stored in dictionary (for testing purpose)
					dictionary.insertNode(word);
					previousWord = word;
				}
			}
		}
	}
	else {
		cout << "Can't open file." << endl;
	}

	cout << "Total Words in Dictionary: " << dictionary.getLength() << endl << endl;

	dictionary.createChain();						// create chains
	cout << endl;

	//dictionary.printWordsChains();				// to print chains
	//cout << endl;

	cout << "\t\t\t\t[TASK: 3]\nCompiling results, please wait..." << endl;
	dictionary.findChains();
	cout << "Results compiled successfully, please check the text files in your directory." << endl;

	vector<vector<string>>::iterator vIterator;

	cout << "\n\t\t\t\t[TASK: 4]\nCalculated minimum chains for all solvable words are saved in a separate text file (wordsSolvable.txt and chainsSolvableWords.txt)." << endl;

	cout << endl << "\t\t\t\t[TASK: 5]\nSize of longest chain: " << longestChainLen << endl;
	cout << endl << "Longest chain(s) & word pair(s):" << endl;
	
	for (vIterator = longestChains.begin(); vIterator != longestChains.end(); vIterator++) {
		string startWord, lastWord;
		
		startWord = (*vIterator)[0];
		vector<string>::iterator vIterator2;
		
		for (vIterator2 = (*vIterator).begin();
			vIterator2 != (*vIterator).end();
			vIterator2++) {
			cout << *vIterator2 << " -->> ";
			lastWord = *vIterator2;
		}
		cout << "END\t\t(" << startWord << ", " << lastWord << ")" << endl;
	}

	cout << endl;

	int c = 1;
	vector<int>::iterator vIterator3;
	
	cout << "\t\t\t\t[TASK: 6]\nNon-solvable words are stored in separate text file (wordsNotSolvable.txt)." << endl << endl;

	cout << "\t\t\t\t[TASK: 7]\nChain Length\t\t\tFrequency" << endl;
	for (vIterator3 = frequencyDistribution.begin();
		vIterator3 != frequencyDistribution.end();
		vIterator3++) {
		cout << c << "\t\t\t\t" << *vIterator3 << endl;
		c++;
	}

	cout << "\n\t\t\t\t[TASK: 8]\nAll the results are displayed on console and also it's screenshot is attached in document." << endl;
	cout << "\n\t\t\t\t[TASK: 9]\nDescription document is attached in zip file." << endl;
	cout << "\n\t\t\t\t[TASK: 10]\nTalked to you about this. (Public Repository Issue)" << endl;

	return;
}