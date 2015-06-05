#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include <map>
#include <string>
#include <GL/glut.h>
#include <GL/glaux.h>

using namespace std;

class TextureManager {
public:
	static TextureManager* getInstance();
	void loadTexture(string textureName, string filename);
	AUX_RGBImageRec* loadBMP(string fileName);
	void setTexture(string textureName);
	GLuint* getTextureIdByName(string textureName);

private:
	TextureManager();
	static TextureManager* instance;
	map<string, GLuint> textures;
};

#endif