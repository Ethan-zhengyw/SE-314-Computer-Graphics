#include "textureManager.h"


TextureManager* TextureManager::instance = NULL;
TextureManager::TextureManager() {}

TextureManager* TextureManager::getInstance() {
	if (instance == NULL)
		instance = new TextureManager();
	return instance;
}

void TextureManager::setTexture(string textureName)
{
	glBindTexture(GL_TEXTURE_2D, textures[textureName]);
}

AUX_RGBImageRec* TextureManager::loadBMP(string fileName)				// Loads A Bitmap Image
{
	FILE *File = NULL;									// File Handle
	if (!fileName.c_str())										// Make Sure A fileName Was Given
	{
		return NULL;									// If Not Return NULL
	}
	File = fopen(fileName.c_str(), "r");							// Check To See If The File Exists
	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(fileName.c_str());				// Load The Bitmap And Return A Pointer
	}
	return NULL;										// If Load Failed Return NULL
}

void TextureManager::loadTexture(string textureName, string filename) {
	GLuint texture;
	static AUX_RGBImageRec *TextureImage = NULL;
	TextureImage = loadBMP(filename);
	if (TextureImage == NULL) {
		return;
	}
	glGenTextures(1, &texture);		// create tag for texure
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// free
	if (TextureImage)									// If Texture Exists
	{
		if (TextureImage->data)							// If Texture Image Exists
		{
			free(TextureImage->data);					// Free The Texture Image Memory
		}

		free(TextureImage);								// Free The Image Structure
	}
	textures[textureName] = texture;
}

GLuint* TextureManager::getTextureIdByName(string textureName) {
	return &textures[textureName];
}