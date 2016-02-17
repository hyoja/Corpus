#include <string.h>
#include <iostream>
#include <map>
#include <utility>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

class Item // 아이탬을 저장하는 클래스입니다.
{
public:
	string item; //아이탬은 문자열을 저장합니다.
	int count; // 카운트는 빈도를 저장합니다.
	Item(string inputItem) // 생성자는 초기변수값을 저장합니다.
	{
		this->item = inputItem;
		this->count = 1;
	}
};

// c++에는 split함수를 지원하지 않음으로 구현합니다.
string* StringSplit(string inputStrTarget, string inputSplitToken) // 인풋은 문자열과, 구분자 입니다.
{
	unsigned int cutPosition; // 현제 문자열의 위치를 나타냅니다.
	int arrayIndex = 0; // 구분하여 배열로 저장하고 반환할것이기 때문에 배열의 index를 나타냅니다
	string* strResult = new string[1000]; // 배열의 크기를 정합니다.

	while ((cutPosition = inputStrTarget.find_first_of(inputSplitToken))
			!= inputStrTarget.npos) // npos -1 이 아니라면 처음으로 발견되는 구분자를 첫 위치로 저장합니다.
	{
		if (cutPosition > 0) // 첫번째라면 달리 처리합니다.
		{
			strResult[arrayIndex++] = inputStrTarget.substr(0, cutPosition); // 처음부터 포지션까지 잘라서 배열에 넣습니다.
		}
		inputStrTarget = inputStrTarget.substr(cutPosition + 1); // 크기만큼 잘라 넣습니다.
	}

	if (inputStrTarget.length() > 0) // 다 했는데도 불구하고 마지막으로 남아 있는 부분을 처리합니다.
	{
		strResult[arrayIndex++] = inputStrTarget.substr(0, cutPosition);
	}

	return strResult;//배열을 반환합니다.
}

int main()
{
	string notNeedWordArray[] =
	{ "about", "above", "after", "against", "along", "around", "at", "beneath",
			"between", "but", "by", "down", "during", "for", "from", "in",
			"into", "of", "off", "on", "out", "over", "per", "round", "since",
			"through", "till", "to", "toward", "until", "up", "upon", "with",
			"within", "without", "as", "a", "an", "the", " " }; // 필요없는 단어들을 배열로 가지고 있습니다.
	string specialLettersArray[] =
	{ "/", ",", "(", ")", ".", "-", "'", ";", ":", "_", "\"", "]", "!", "?",
			"#", "&", "*", "[", "`", "—", "”", "“", "£", "‘", "’", "«", "{",
			"~", "–", "é", "~", "?" ," " }; // 필요 없는 특수문자의 모읍입니다.

	ifstream inputStream("shortInputFile.xml");// 인풋 파일을 읽어옵니다.
	map<string, Item*> itemMap; // 맵을 사용합니다. 맵을 사용하는 이유는 간편하고, 성능에서 좋기 때문입니다.
	while (!inputStream.eof()) // end가 아니라면 계속 합니다.
	{
		string readedLine = ""; // 읽어온 문장을 저정하는 변수입니다.
		getline(inputStream, readedLine); // 문장을 읽어옵니다.
		while ((int) readedLine.find("  ") > -1) // 읽어온 한줄에서 공백을 제거합니다.
		{
			readedLine.replace(readedLine.find("  "), readedLine.find("  ") + 1,"");
			// 공백 시작점에서 공백 끝점까지 지워버립니다.
		}

		while ((int) readedLine.find("<") > -1
				&& (int) readedLine.find(">") > -1) // 읽어온 한줄에서 태그를 제거합니다.
		{
			readedLine.replace((int) readedLine.find("<"),
					(int) readedLine.find(">"), "");
			// < 시작에서 > 끝까지 지워버립니다.
		}

		string* splitedStringArray = StringSplit(readedLine, " "); // 읽어온 한줄을 " " 공백으로 분리해 줍니다. 분리하면 문장이 단어들로 변합니다.

		for (int i = 0; splitedStringArray[i] != ""; i++)
		{
			// 분리되어진 단어들이 필요한 단어인지 확인합니다, 필요한 단어라면
			if (splitedStringArray[i] != ">" && splitedStringArray[i] != " "
					&& splitedStringArray[i] != "")
			{
				// 읽어온 문장에서 특수 문자를 제거합니다.
				for (int j = 0; specialLettersArray[j] != " "; j++)
				{
					if ((int) splitedStringArray[i].find(specialLettersArray[j])
							> -1)
					{
						splitedStringArray[i].replace(
								splitedStringArray[i].find(
										specialLettersArray[j]),
								splitedStringArray[i].find(
										specialLettersArray[j]) + 1, "");
					}
				}

				// 필요 없는 단어를 찾기위한 변수입니다.
				bool isFineded = false;
				for (int j = 0; notNeedWordArray[j] != " "; j++) // 앞서 분리한 단어와 필요 없는 단어와 같다면
				{
					if (splitedStringArray[i] == notNeedWordArray[j])
					{
						isFineded= true;// 필요없는 단어를 찾았습니다.
						break;
					}
				}
				if(isFineded == false)// 필요없는 문가 아니라면
				{
					std::transform( splitedStringArray[i].begin(), splitedStringArray[i].end(), splitedStringArray[i].begin(), ::tolower);// 분리한 문자열들을 소문자로 바꿔줍니다.
					map<string, Item*>::iterator FindIter = itemMap.find(splitedStringArray[i]); // 키는 단어 자체이고, 값은 item입니다. item은 빈도와, string값 자체를 나타냅니다.
					if (FindIter != itemMap.end()) // 트리맵에서 키가 중복된다면 이미 존제하는 자료입니다.
					{
						(FindIter->second)->count += 1; // 그럼으로 카운트를 증가시켜줍니다.
					}
					else
					{
						// 중복하는 키가 존제하지 않는다면 새로운 item으로서 새로 생성해 트리맵에 저장합니다.
						itemMap.insert(make_pair(splitedStringArray[i], new Item(splitedStringArray[i])));
					}
				}
			}
		}
	}
	inputStream.close(); // 일을 맞췄음으로 읽어오는 reader를 close합니다.

	ofstream outFile("output.txt");  // output.txt 로 저장하기 위해서 writer를 생성합니다.
	map<string, Item*>::iterator i;
	for (i = itemMap.begin(); i != itemMap.end(); ++i)
	{
		outFile << i->first << " , " << (i->second)->count << endl; //앞서 구성한 map에서 불러와서 output.txt에 저장힙니다.
	}
    outFile.close(); // 파일을 다 사용했으니 꺼줍니다.

	cout << "job done!";//일을 다하면 다했다고 말해줍니다.
}

