#include <fstream>
#include <iostream>
#include <GL/glut.h>
#include "characterManager.h"

CharacterManager* CharacterManager::instance = NULL;

CharacterManager::CharacterManager() {
	for (int i = 'A'; i <= 'Z'; i++)
		loadCharacterModel(i);
}

CharacterManager* CharacterManager::getInstance()
{
	if (instance == NULL)
		instance = new CharacterManager();
	return instance;
}

void CharacterManager::loadCharacterModel(char character) {
	string path = "model/letters/";
	path = path + character + ".off";

	std::ifstream in(path);
	if (!in.is_open()) {
		cout << "load character " << character << " failed!" << endl;
		return;
	}

	Character model;
	int tmpI;
	float tmpF;
	std::string tmp;

	in >> tmp;  // 读掉第一行的OFF
	in >> model.nVertices >> model.nFaces >> model.nEdges;

	for (int i = 0; i < model.nVertices; i++) {
		std::vector<float> vertice;
		for (int j = 0; j < 3; j++) {
			in >> tmpF;
			vertice.push_back(tmpF);
		}
		model.vertices.push_back(vertice);
	}

	for (int i = 0; i < model.nFaces; i++) {
		int nFaceEdges;
		std::vector<int> face;

		in >> nFaceEdges;
		face.push_back(nFaceEdges);
		for (int j = 0; j < nFaceEdges; j++) {
			in >> tmpI;
			face.push_back(tmpI);
		}
		model.faces.push_back(face);
	}

	characters[character] = model;
}

void CharacterManager::displayCharacter(char character, float biasX, float biasY, float biasZ, bool isLineMode)
{
	float colorR, colorG, colorB;
	colorR = 0.0;
	colorG = 0.5;
	colorB = 1.0;

	Character model = characters[character];

	if (isLineMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	for (int i = 0; i < model.nFaces; i++) {
		std::vector<int> indexs = model.faces[i];
		glColor3f(colorR, colorG, colorB);
		glBegin(GL_POLYGON);
		for (int j = 1; j <= indexs[0]; j++) {
			glVertex3f(
				biasX + 8 * model.vertices[indexs[j]][0],
				biasY + 10 * model.vertices[indexs[j]][1],
				biasZ + 5 * model.vertices[indexs[j]][2]
				);
		}
		glEnd();
		colorR = ((colorR + 0.3) <= 1.0) ? (colorR + 0.3) : 0.0;
		colorG = ((colorG + 0.2) <= 1.0) ? (colorG + 0.2) : 0.0;
		colorB = ((colorB + 0.1) <= 1.0) ? (colorB + 0.1) : 0.0;
	}
}

void CharacterManager::displayString(string sentence, float baseX, float baseY, float baseZ, bool isLineMode)
{
	for (int i = 0; i < sentence.length(); i++) {
		if (sentence[i] >= 'A' && sentence[i] < 'Z')
			displayCharacter(sentence[i], baseX + 32 * i, baseY, baseZ, isLineMode);
	}
}