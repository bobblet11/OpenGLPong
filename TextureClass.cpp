#include "TextureClass.h"

Texture::Texture(std::string textureFileName, GLint filterMethod, GLint S_repeatMethod, GLint T_repeatMethod)
{
	//information used to store the read data from the texture image
	int imagePixelWidth, imagePixelHeight, numberColourChannels;
	unsigned char* bytes = stbi_load(textureFileName.c_str(), &imagePixelWidth, &imagePixelHeight, &numberColourChannels, 0);
	
	//create the texture buffer
	glGenTextures(1, &ID);
	//set the texture buffer to the first unit, there are usually 16 textures in a texture batch
	glActiveTexture(GL_TEXTURE0);
	//set the current texture to ID, and set it as a 2D texture
	glBindTexture(GL_TEXTURE_2D, ID);

	//modify the parameters of the texture, filter methods and repeat methods
	//filter methods control how textures upscale
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMethod);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMethod);
	//repeat methods control how texture behave when stretched, either repeated, repeated and mirrors, stretched at border, or flat colour background
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S_repeatMethod);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, T_repeatMethod);

	//the colour data format of the image
	GLenum colourFormat;
	switch (numberColourChannels)
	{
		case 3:
			//if there is 3 colour channels, ie no alpha, then its RGB. typically JPEGS or PNGs with no transparency
			colourFormat = GL_RGB;
			break;
		case 4:
			//if there is 4 colour channels, ie RGB + Alpha, then its RGBA
			colourFormat = GL_RGBA;
			break;
		default:
			//otherwise there is a texture error and the texture cannot be rendered. so end program
			std::cout << "TEXTURE ERROR: Image lacks correct colour data" << std::endl;
			exit(-1);
	}

	//RGBA is for PNG, RGB is for JPEG
	//assign the image data to the texture object
	glTexImage2D(GL_TEXTURE_2D, 0, colourFormat, imagePixelWidth, imagePixelHeight, 0, colourFormat, GL_UNSIGNED_BYTE, bytes);
	//generate mip-maps creates many different smaller resolution versions of the texture for rendering smaller textures
	glGenerateMipmap(GL_TEXTURE_2D);

	//clear memory of image
	stbi_image_free(bytes);
	//unbind so that it cant be modified
	Unbind();
}

void Texture::Bind()
{
	//sets this texture object as the current texture
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	//resets the current texture to 0
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	//removes this texture from memory
	glDeleteTextures(1, &ID);
}