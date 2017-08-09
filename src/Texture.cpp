/**
 * /author Bret Jackson
 *
 * /file  Texture.cpp
 * /brief Class wraps an opengl texture
 *
 */ 

#include "Texture.h"


namespace basicgraphics {

	Texture::Texture(const std::string &name, int width, int height, int depth, int numMipMapLevels, bool autoMipMap, GLenum target, GLenum internalFormat, GLenum externalFormat, GLenum dataFormat, const void* bytes[6])
	{
		_name = name;
		_fileName = "";
		_target = target;
		_internalFormat = internalFormat;
		_width = width;
		_height = height;
		_depth = depth;
		_dataFormat = dataFormat;
		_externalFormat = externalFormat;
		_numMipMapLevels = numMipMapLevels;
		_autoGenMipMaps = autoMipMap;

		_empty = false;
		if (bytes[0] == nullptr) {
			_empty = true;
		}

		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		//glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

		glGenTextures(1, &_texID);
		//glEnable(_target);
		glBindTexture(_target, _texID);

		//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 4); //TODO: this assumption is not always correct
		//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		//glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		//glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		switch (_target) {
		case GL_TEXTURE_1D:

			if (!_empty) {
				glTexImage1D(_target, 0, _internalFormat, _width, 0, _externalFormat, _dataFormat, bytes[0]);
			}
			else {
				std::vector<GLubyte> emptyData(_width * _height * 4, 0);
				glTexImage1D(_target, 0, _internalFormat, _width, 0, _externalFormat, _dataFormat, &emptyData[0]);
			}
			break;
		case GL_TEXTURE_1D_ARRAY:
		case GL_TEXTURE_2D:
			if (!_empty) {
				glTexImage2D(_target, 0, _internalFormat, _width, _height, 0, _externalFormat, _dataFormat, bytes[0]);
			}
			else {
				std::vector<GLubyte> emptyData(_width * _height * 4, 0);
				glTexImage2D(_target, 0, _internalFormat, _width, _height, 0, _externalFormat, _dataFormat, &emptyData[0]);
			}
			break;
		case GL_TEXTURE_CUBE_MAP:
			if (!_empty) {
				for (int face = 0; face < 6; face++) {
					GLenum faceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;
					glTexImage2D(faceTarget, 0, _internalFormat, _width, _height, 0, _externalFormat, _dataFormat, bytes[face]);
				}
			}
			else {
				std::vector<GLubyte> emptyData(_width * _height * 4, 0);
				for (int face = 0; face < 6; face++) {
					GLenum faceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;
					glTexImage2D(faceTarget, 0, _internalFormat, _width, _height, 0, _externalFormat, _dataFormat, &emptyData[0]);
				}
			}
			break;
		case GL_TEXTURE_2D_ARRAY:
		case GL_TEXTURE_3D:
			if (!_empty) {
				glTexImage3D(_target, 0, _internalFormat, _width, _height, _depth, 0, _externalFormat, _dataFormat, bytes[0]);
			}
			else {
				std::vector<GLubyte> emptyData(_width * _height * _depth * 4, 0);
				glTexImage3D(_target, 0, _internalFormat, _width, _height, _depth, 0, _externalFormat, _dataFormat, &emptyData[0]);
			}
			break;
		default:
			assert(false && "Texture target type not yet supported");
		}
        
        // Setup reasonable Defaults. Note this may cause invalid enum errors if the target is GL_TEXTURE_RECTANGLE
        setTexParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
        setTexParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
        setTexParameteri(GL_TEXTURE_WRAP_R, GL_REPEAT);
        setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (!_empty && _autoGenMipMaps) {
			glGenerateMipmap(_target);
            setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		//glPopClientAttrib();
		//glPopAttrib();
	}

	void Texture::update(const void* bytes, GLenum externalFormat, GLenum dataFormat, int unpackAlignment/*=4*/, int unpackRowLength/*=-1*/, int cubeMapFace/*=0*/)
	{
		_externalFormat = externalFormat;
		_dataFormat = dataFormat;

		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		//glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		//glEnable(_target);
		glBindTexture(_target, _texID);

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		//glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
		//if (unpackRowLength != -1) {
		//	glPixelStorei(GL_UNPACK_ROW_LENGTH, unpackRowLength);
		//}

		switch (_target) {
		case GL_TEXTURE_1D:
			glTexSubImage1D(_target, 0, 0, _width, _externalFormat, _dataFormat, bytes);
			break;
		case GL_TEXTURE_1D_ARRAY:
		case GL_TEXTURE_2D:
			glTexSubImage2D(_target, 0, 0, 0, _width, _height, _externalFormat, _dataFormat, bytes);
			break;
		case GL_TEXTURE_CUBE_MAP:
		{
			GLenum faceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeMapFace;
			glTexSubImage2D(faceTarget, 0, 0, 0, _width, _height, _externalFormat, _dataFormat, bytes);
			break;
		}
		case GL_TEXTURE_2D_ARRAY:
		case GL_TEXTURE_3D:
			glTexSubImage3D(_target, 0, 0, 0, 0, _width, _height, _depth, _externalFormat, _dataFormat, bytes);
			break;
		default:
			assert(false && "Texture target type not yet supported");
		}

		if (_empty) {
			_empty = false;
		}

		if (_autoGenMipMaps) {
			glGenerateMipmap(_target);
		}
		//glPopClientAttrib();
		//glPopAttrib();
	}

	void Texture::generateMipMaps()
	{
		if (!_empty && _numMipMapLevels > 1) {
			//glPushAttrib(GL_ALL_ATTRIB_BITS);
			//glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
			//glEnable(_target);
			glBindTexture(_target, _texID);
			glGenerateMipmap(_target);
			//glPopClientAttrib();
			//glPopAttrib();
		}
	}

	void Texture::bind(GLenum textureNum)
	{
		glActiveTexture(GL_TEXTURE0 + textureNum);
		glBindTexture(_target, _texID);
	}

	void Texture::setFileName(const std::string &filename)
	{
		_fileName = filename;
	}

	std::string Texture::getFileName() const
	{
		return _fileName;
	}

	GLuint Texture::getID() const
	{
		return _texID;
	}

	float Texture::getHeightToWidthRatio() const
	{
		return ((float)_height) / (float)_width;
	}

	int Texture::getWidth() const
	{
		return _width;
	}

	int Texture::getHeight() const
	{
		return _height;
	}

	bool Texture::isOpaque() const
	{
		if (_internalFormat == GL_LUMINANCE_ALPHA ||
			_internalFormat == GL_LUMINANCE8_ALPHA8 ||
			_internalFormat == GL_RGBA ||
			_internalFormat == GL_RGBA2 ||
			_internalFormat == GL_RGBA4 ||
			_internalFormat == GL_RGBA4 ||
			_internalFormat == GL_RGB5_A1 ||
			_internalFormat == GL_RGBA8 ||
			_internalFormat == GL_RGBA8_SNORM ||
			_internalFormat == GL_RGB10_A2 ||
			_internalFormat == GL_RGB10_A2UI ||
			_internalFormat == GL_RGBA12 ||
			_internalFormat == GL_RGBA16 ||
			_internalFormat == GL_RGBA16_SNORM ||
			_internalFormat == GL_SRGB8_ALPHA8 ||
			_internalFormat == GL_RGBA16F ||
			_internalFormat == GL_RGBA32F ||
			_internalFormat == GL_RGBA8I ||
			_internalFormat == GL_RGBA8UI ||
			_internalFormat == GL_RGBA16I ||
			_internalFormat == GL_RGBA16UI ||
			_internalFormat == GL_RGBA32I ||
			_internalFormat == GL_RGBA32UI) {
			return false;
		}
		return true;
	}

	void Texture::save2D(const std::string &file)
	{
		
		assert(_target == GL_TEXTURE_2D);
		glBindTexture(_target, _texID);

		int size = sizeof(unsigned char) * _width * _height * 4;
		unsigned char *raw_img = (unsigned char*)malloc(size);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw_img);

		int result = SOIL_save_image(file.c_str(), SOIL_SAVE_TYPE_BMP, _width, _height, 4, raw_img);

		if (result == 0) {
			std::cerr << SOIL_last_result() << std::endl;
			assert(false);
		}
		
/*
		assert(_target == GL_TEXTURE_2D);
		glBindTexture(_target, _texID);

		int size = sizeof(BYTE) * _width * _height * 4;
		BYTE *raw_img = (BYTE*)malloc(size);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, raw_img);

		fipImage* image = new fipImage(FIT_BITMAP, _width, _height, 32);

		BYTE* pixels(image->accessPixels());

		const size_t bytesPerPixel = 4;
		const size_t rowStride = size_t(_width * bytesPerPixel);

		for (int row = 0; row < _height; ++row) {
			std::memcpy(image->getScanLine(row), raw_img + rowStride * row, rowStride);
		}

		int flag = 0;
		if (!image->save(file.c_str(), flag)) {
			assert(false);
		}
		free(raw_img);
		delete image;
 */

	}

	void Texture::setTexParameterfv(GLenum param, GLfloat* val)
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		//glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		//glEnable(_target);
		glBindTexture(_target, _texID);
		glTexParameterfv(_target, param, val);
		//glPopClientAttrib();
		//glPopAttrib();
	}

	void Texture::setTexParameteriv(GLenum param, GLint* val)
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		//glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		//glEnable(_target);
		glBindTexture(_target, _texID);
		glTexParameterIiv(_target, param, val);
		//glPopClientAttrib();
		//glPopAttrib();
	}

	void Texture::setTexParameteruiv(GLenum param, GLuint* val)
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		//glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		//glEnable(_target);
		glBindTexture(_target, _texID);
		glTexParameterIuiv(_target, param, val);
		//glPopClientAttrib();
		//glPopAttrib();
	}

	void Texture::setTexParameteri(GLenum param, GLint val)
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		//glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		//glEnable(_target);
		glBindTexture(_target, _texID);
		glTexParameteri(_target, param, val);
		//glPopClientAttrib();
		//glPopAttrib();
	}

	void Texture::setTexParameterf(GLenum param, GLfloat val)
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		//glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		//glEnable(_target);
		glBindTexture(_target, _texID);
		glTexParameterf(_target, param, val);
		//glPopClientAttrib();
		//glPopAttrib();
	}

	std::shared_ptr<Texture> Texture::createEmpty(const std::string &name, int width, int height, int depth, int numMipMapLevels, bool autoMipMap, GLenum target, GLenum internalFormat)
	{
		const void* bytes[6];
		bytes[0] = nullptr;
		return std::shared_ptr<Texture>(new Texture(name, width, height, depth, numMipMapLevels, autoMipMap, target, internalFormat, getExternalFormat(internalFormat), determineDataType(internalFormat), bytes));
	}

	std::shared_ptr<Texture> Texture::createCubeMapFromFiles(const std::string filenames[6], bool generateMipMaps/*=false*/, int numMipMapLevels/*=1*/)
	{
		
		GLenum internalFormat = 0;
		int width, height, channels;
		const void* bytesArray[6];
		unsigned char* images[6];
		for (int face = 0; face < 6; face++) {
			images[face] = SOIL_load_image(filenames[face].c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
			if (images[face] == NULL) {
				assert(false && ("Unable to load texture: " + filenames[face] + "\n" + SOIL_last_result()).c_str());
			}

			GLenum faceInternalFormat = 0;
			switch (channels) {
			case 1:
				faceInternalFormat = GL_LUMINANCE8;
				break;

			case 2:
				faceInternalFormat = GL_LUMINANCE8_ALPHA8;

			case 3:
				faceInternalFormat = GL_RGB8;
				break;

			case 4:
				faceInternalFormat = GL_RGBA8;
				break;

			default:
				assert(false && ("Loaded image data in unsupported format: " + filenames[0]).c_str());
			}

			if (face == 0) {
				internalFormat = faceInternalFormat;
			}
			else if (faceInternalFormat != internalFormat) {
				assert(false && "All faces in a cubemap must have the same internal format");
			}

			bytesArray[face] = images[face];
		}

		std::shared_ptr<Texture> tex(new Texture(filenames[0], width, height, 1, numMipMapLevels, generateMipMaps, GL_TEXTURE_CUBE_MAP, internalFormat, getExternalFormat(internalFormat), determineDataType(internalFormat), bytesArray));

		// Don't delete the images until after the texture is generated or else the bytesArray gets deallocated
		for (int i = 0; i < 6; i++) {
			SOIL_free_image_data(images[i]);
		}

		return tex;

    /*
		GLenum internalFormat;
		int width, height;
		const void* bytesArray[6];
		fipImage* images[6];
		for (int face = 0; face < 6; face++) {
			images[face] = new fipImage();
			bool success = images[face]->load(filenames[face].c_str());
			if (!success) {
				assert(false);//BOOST_ASSERT_MSG(false, ("Unable to load texture: " + filenames[face]).c_str());
			}

			GLenum faceInternalFormat = determineImageFormat(images[face]);

			// Convert palettized images so row data can be copied easier
			if (images[face]->getColorType() == FIC_PALETTE) {
				switch (images[face]->getBitsPerPixel()) {
				case 1:
					images[face]->convertToGrayscale();
					faceInternalFormat = GL_LUMINANCE8;
					break;

				case 8:
				case 24:
					images[face]->convertTo24Bits();
					faceInternalFormat = GL_RGB8;
					break;

				case 32:
					images[face]->convertTo32Bits();
					faceInternalFormat = GL_RGBA8;
					break;

				default:
					assert(false);//BOOST_ASSERT_MSG(false, ("Loaded image data in unsupported palette format: " + filenames[face]).c_str());
				}
			}

			if (face == 0) {
				internalFormat = faceInternalFormat;
				width = images[face]->getWidth();
				height = images[face]->getHeight();
			}
			else if (faceInternalFormat != internalFormat) {
				assert(false);
			}

			BYTE* pixels = images[face]->accessPixels();
			bytesArray[face] = pixels;
		}

		std::shared_ptr<Texture> tex(new Texture(filenames[0], width, height, 1, numMipMapLevels, generateMipMaps, GL_TEXTURE_CUBE_MAP, internalFormat, getExternalFormat(internalFormat), determineDataType(internalFormat), bytesArray));

		// Don't delete the images until after the texture is generated or else the bytesArray gets deallocated
		for (int i = 0; i < 6; i++) {
			delete images[i];
		}

		return tex;
     */
	}

	std::shared_ptr<Texture> Texture::createFromMemory(const std::string &name, const void* bytes, GLenum dataFormat, GLenum externalFormat, GLenum internalFormat, GLenum target, int width, int height, int depth, bool generateMipMaps/*=false*/, int numMipMapLevels/*=1*/)
	{
		const void* bytesArray[6];
		bytesArray[0] = bytes;
		return std::shared_ptr<Texture>(new Texture(name, width, height, depth, numMipMapLevels, generateMipMaps, target, internalFormat, externalFormat, dataFormat, bytesArray));
	}

	std::shared_ptr<Texture> Texture::create2DTextureFromFile(const std::string &filename, bool generateMipMaps/*=false*/, int numMipMapLevels/*=1*/)
	{
	
		int width, height, channels;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
		if (image == NULL) {
            std::string errormsg = SOIL_last_result();
			assert(false && ("Unable to load texture"));
		}

		GLenum internalFormat = 0;

		switch (channels) {
		case 1:
			internalFormat = GL_LUMINANCE8;
			break;

		case 2:
			internalFormat = GL_LUMINANCE8_ALPHA8;

		case 3:
			internalFormat = GL_RGB8;
			break;

		case 4:
			internalFormat = GL_RGBA8;
			break;

		default:
			assert(false && ("Loaded image data in unsupported format: " + filename).c_str());
		}


		const void* bytesArray[6];
		bytesArray[0] = image;
		std::shared_ptr<Texture> tex(new Texture(filename, width, height, 1, numMipMapLevels, generateMipMaps, GL_TEXTURE_2D, internalFormat, getExternalFormat(internalFormat), determineDataType(internalFormat), bytesArray));//internalFormat, getExternalFormat(internalFormat), determineDataType(internalFormat), bytesArray));
		SOIL_free_image_data(image);

		return tex;
		/*

		fipImage* image = new fipImage();
		bool success = image->load(filename.c_str());
		if (!success) {
			assert(false);
		}

		GLenum internalFormat = determineImageFormat(image);

		// Convert palettized images so row data can be copied easier
		if (image->getColorType() == FIC_PALETTE) {
			switch (image->getBitsPerPixel()) {
			case 1:
				image->convertToGrayscale();
				internalFormat = GL_LUMINANCE8;
				break;

			case 8:
			case 24:
				image->convertTo24Bits();
				internalFormat = GL_RGB8;
				break;

			case 32:
				image->convertTo32Bits();
				internalFormat = GL_RGBA8;
				break;

			default:
				assert(false);
			}
		}

		BYTE* pixels = image->accessPixels();
		const void* bytesArray[6];
		bytesArray[0] = pixels;
		std::shared_ptr<Texture> tex(new Texture(filename, image->getWidth(), image->getHeight(), 1, numMipMapLevels, generateMipMaps, GL_TEXTURE_2D, internalFormat, getExternalFormat(internalFormat), determineDataType(internalFormat), bytesArray));
		delete image;

		return tex;
*/
	}

    /*
	GLenum Texture::determineImageFormat(const fipImage* image)
	{
		assert(image->isValid() && image->getImageType() != FIT_UNKNOWN);

		switch (image->getImageType())
		{
		case FIT_BITMAP:
		{
			switch (image->getBitsPerPixel())
			{
			case 8:
				return GL_LUMINANCE8;
			case 16:
				// todo: find matching image format
				assert(false);
				break;
			case 24:
				return GL_RGB8;
			case 32:
				return GL_RGBA8;
			default:
				assert(false);
				break;
			}
			break;
		}
		case FIT_UINT16:
			return GL_LUMINANCE16;
		case FIT_FLOAT:
			return GL_LUMINANCE32F_ARB;
		case FIT_RGBF:
			return GL_RGB32F;
		case FIT_RGBAF:
			return GL_RGBA32F;

		case FIT_INT16:
		case FIT_UINT32:
		case FIT_INT32:
		case FIT_DOUBLE:
		case FIT_RGB16:
		case FIT_RGBA16:
		case FIT_COMPLEX:
		default:
			assert(false);
			break;
		}

		if (image->getColorType() == FIC_CMYK) {
			assert(false);
		}

		return 0;
	}

*/

	Texture::~Texture()
	{
		glDeleteTextures(1, &_texID);
	}

	std::string Texture::getName() const
	{
		return _name;
	}

	GLenum Texture::determineDataType(const GLenum internalType)
	{
		switch (internalType)
		{
		case GL_LUMINANCE:
			return GL_UNSIGNED_BYTE;
		case GL_LUMINANCE8:
			return GL_UNSIGNED_BYTE;
		case GL_LUMINANCE16:
			return GL_UNSIGNED_SHORT;
		case GL_LUMINANCE8_ALPHA8:
			return GL_UNSIGNED_BYTE;
		case GL_RGBA:
			return GL_UNSIGNED_BYTE;
		case GL_RGB8:
			return GL_UNSIGNED_BYTE;
		case GL_RGBA8:
			return GL_UNSIGNED_BYTE;
		case GL_RGB32F:
			return GL_FLOAT;
		case GL_RGBA32F:
			return GL_FLOAT;
		case GL_DEPTH_COMPONENT32F:
			return GL_FLOAT;
		default:
			assert(false && "Unsupported InternalType. Cannot determine the individual data type");
			return GL_UNSIGNED_BYTE;
		}
	}

	GLenum Texture::getExternalFormat(const GLenum internalType)
	{
		switch (internalType)
		{
		case GL_LUMINANCE:
		case GL_LUMINANCE8:
		case GL_LUMINANCE16:
		case GL_DEPTH_COMPONENT32F:
			return GL_RED;

		case GL_LUMINANCE_ALPHA:
		case GL_LUMINANCE8_ALPHA8:
			return GL_LUMINANCE_ALPHA;

		case GL_RGB:
		case GL_RGB8:
		case GL_RGB32F:
			return GL_RGB;

		case GL_RGBA:
		case GL_RGBA8:
		case GL_RGBA32F:
			return GL_RGBA;

		default:
			assert(false && "Unsupported InternalType. Cannot determine the external format");
			return GL_RGBA;
		}
	}

}