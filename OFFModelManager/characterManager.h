#ifndef _CHARACTERMANAGER_H_
#define _CHARACTERMANAGER_H_

#include <vector>
#include <map>
#include <string>

using namespace std;

typedef struct {
	int nVertices, nFaces, nEdges;
	vector<vector<float>> vertices; // nVertices个点
	vector<vector<int>> faces;  // 第一个代表有多少个点
} Character;


class CharacterManager
{
public:
	static CharacterManager* getInstance();
	void loadCharacterModel(char character);  // 只能载入大写的字母
	void displayCharacter(char character, float biasX, float biasY, float biasZ, bool isLineMode);
	void displayString(string sentence, float baseX, float baseY, float baseZ, bool isLineMode);

private:
	CharacterManager();
	static CharacterManager* instance;
	map<char, Character> characters;
};

#endif