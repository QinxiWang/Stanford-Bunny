/*!
 * /author Bret Jackson
 *
 * /file  Texture.h
 * /brief Wrapper around OpenGL textures
 *
 */ 

#ifndef TEXTURE_H
#define TEXTURE_H

#include <SOIL.h>
#include <string>

#include <glad/glad.h>
#include <memory>
#include <iostream>
#include <vector>
#include <assert.h>

//#include <FreeImage/FreeImagePlus.h>

namespace basicgraphics {

	class Texture : public std::enable_shared_from_this<Texture>
	{
	public:
		virtual ~Texture();

		// update a texture with new data, the size of the bytes array must match the size and format of when the texture was created.
		void update(const void* bytes, GLenum externalFormat, GLenum dataFormat, int unpackAlignment = 4, int unpackRowLength = -1, int cubeMapFace = 0);

		// Used if autoGenMipMaps is false to manually generate mipmaps.
		void generateMipMaps();

		void bind(GLenum textureNum); // bind to an active texturenum, ex. GL_TEXTURE0
		int getWidth() const;
		int getHeight() const;
		float getHeightToWidthRatio() const;
		void setTexParameterfv(GLenum param, GLfloat* val);
		void setTexParameteriv(GLenum param, GLint* val);
		void setTexParameteruiv(GLenum param, GLuint* val);
		void setTexParameteri(GLenum param, GLint val);
		void setTexParameterf(GLenum param, GLfloat val);
		std::string getName() const;
		GLuint getID() const;

		// set an associated filename that the texture was loaded from.
		void setFileName(const std::string &filename);
		std::string getFileName() const;

		/**
		True if this texture was created with out an alpha channel.  Note that
		a texture may have a format that is not opaque (e.g. RGBA8) yet still
		have a completely opaque alpha channel, causing texture->opaque to
		be false.  This is just a flag set for the user's convenience-- it does not affect rendering in any way.*/
		bool isOpaque() const;

		// saves a GL_TEXTURE_2D. Mostly used for debugging
		void save2D(const std::string &file);

		static std::shared_ptr<Texture> createEmpty(const std::string &name,
			int width, // in pixels 
			int height, // in pixels
			int depth, // in pixels for 3D textures
			int numMipMapLevels,
			bool autoMipMap,
			GLenum target, // Texture target (GL_TEXTURE_2D, GL_TEXTURE_3D, or GL_TEXTURE_CUBE.)
			GLenum internalFormat); // Recommended internal format

        static std::shared_ptr<Texture> create2DTextureFromFile(const std::string &filename, bool generateMipMaps = false, int numMipMapLevels = 1);

		static std::shared_ptr<Texture> createCubeMapFromFiles(const std::string filenames[6], bool generateMipMaps = false, int numMipMapLevels = 1);

		static std::shared_ptr<Texture> createFromMemory(const std::string &name, const void* bytes, GLenum dataFormat, GLenum externalFormat, GLenum internalFormat, GLenum target, int width, int height, int depth, bool generateMipMaps = false, int numMipMapLevels = 1);

	private:
		Texture() {};
		Texture(const Texture& that) {}; // prevent copying

		Texture(const std::string &name,
			int width, // in pixels 
			int height, // in pixels
			int depth, // in pixels for 3D textures
			int numMipMapLevels,
			bool autoMipMap,
			GLenum target, // Texture target (GL_TEXTURE_2D, GL_TEXTURE_3D, or GL_TEXTURE_CUBE.)
			GLenum internalFormat, // recommmended internal format, ex. GL_RGB8
			GLenum externalFormat, // format of the bytes array, ex. GL_BGRA
			GLenum dataFormat, // format of the data, ex. GL_UNSIGNED_BYTE
			const void* bytes[6]);

		//static GLenum determineImageFormat(const fipImage* image);
		static GLenum determineDataType(const GLenum internalType);
		static GLenum getExternalFormat(const GLenum internalType);

		std::string _name;
		std::string _fileName;
		GLuint _texID;
		GLenum _target;
		GLenum _internalFormat;
		GLenum _externalFormat;
		GLenum _dataFormat;
		int _width;
		int _height;
		int _depth;
		int _numMipMapLevels;
		bool _autoGenMipMaps;
		bool _empty;
	};

}

#endif //TEXTURE_H