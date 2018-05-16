//  Copyright (c) 2016 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <gfx/Texture.h>

#include <gfx/GfxSystem.h>
#include <gfx/Node3.h>
#include <gfx/Renderer.h>

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>

#include <bx/readerwriter.h>

#include <stb_image.h>
#include <string>

namespace mud
{
	using string = std::string;

	string readable_size(double size)
	{
		int i = 0;
		const char* units[] = { "B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
		for(; size > 1024; ++i)
			size /= 1024;
		char buffer[10];
		sprintf(buffer, "%.*f %s", i, size, units[i]);
		return string(buffer);
	}

	static void* load_mem(bx::FileReaderI* reader, bx::AllocatorI* allocator, const char* file_path, uint32_t* size)
	{
		if(!bx::open(reader, file_path))
		{
			printf("ERROR: Failed to open: %s.\n", file_path);
			return nullptr;
		}

		uint32_t data_size = (uint32_t)bx::getSize(reader);
		void* data = BX_ALLOC(allocator, data_size);
		bx::read(reader, data, data_size);
		bx::close(reader);

		if(size)
			*size = data_size;
		return data;
	}

	static void release_image(void* _ptr, void* _userData)
	{
		BX_UNUSED(_ptr);
		bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
		bimg::imageFree(imageContainer);
	}

	bgfx::TextureHandle load_texture(bx::AllocatorI& allocator, bx::FileReaderI& reader, const char* file_path, uint32_t flags, bgfx::TextureInfo* info, bimg::Orientation::Enum* orientation)
	{
		bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

		uint32_t size;
		void* data = load_mem(&reader, &allocator, file_path, &size);
		if(!data)
			return handle;

		bimg::ImageContainer* image = bimg::imageParse(&allocator, data, size);
		if(!image)
			return handle;

		if(orientation)
			*orientation = image->m_orientation;

		const bgfx::Memory* mem = bgfx::makeRef(image->m_data, image->m_size, release_image, image);
		BX_FREE(&allocator, data);

		printf("INFO: Loaded image %s of size %s in memory\n", file_path, readable_size(image->m_size).c_str());

		if(image->m_cubeMap)
		{
			handle = bgfx::createTextureCube(uint16_t(image->m_width), 1 < image->m_numMips,
											 image->m_numLayers, bgfx::TextureFormat::Enum(image->m_format), flags, mem);
		}
		else if(1 < image->m_depth)
		{
			handle = bgfx::createTexture3D(uint16_t(image->m_width), uint16_t(image->m_height), uint16_t(image->m_depth),
										   1 < image->m_numMips, bgfx::TextureFormat::Enum(image->m_format), flags, mem);
		}
		else
		{
			handle = bgfx::createTexture2D(uint16_t(image->m_width), uint16_t(image->m_height), 1 < image->m_numMips,
										   image->m_numLayers, bgfx::TextureFormat::Enum(image->m_format), flags, mem);
		}

		if(info)
		{
			bgfx::calcTextureSize(*info, uint16_t(image->m_width), uint16_t(image->m_height), uint16_t(image->m_depth),
								  image->m_cubeMap, 1 < image->m_numMips, image->m_numLayers, bgfx::TextureFormat::Enum(image->m_format));
		}

		return handle;
	}

	bimg::ImageContainer* load_image(bx::AllocatorI& allocator, bx::FileReaderI& _reader, const char* _filePath, bgfx::TextureFormat::Enum _dstFormat)
	{
		uint32_t size = 0;
		void* data = load_mem(&_reader, &allocator, _filePath, &size);
		return bimg::imageParse(&allocator, data, size, bimg::TextureFormat::Enum(_dstFormat));
	}

	void load_texture(GfxSystem& gfx_system, Texture& texture, cstring path)
	{
		bgfx::TextureInfo texture_info;
		texture.m_texture = load_texture(gfx_system.m_allocator, gfx_system.m_file_reader, path, 0U, &texture_info);
		texture.m_width = texture_info.width;
		texture.m_height = texture_info.height;
	}

	Texture::Texture(GfxSystem& gfx_system, cstring name)
		: m_name(name)
	{
		carray<cstring, 1> exts = { "" };
		cstring path = gfx_system.locate_file(("textures/" + string(name)).c_str(), exts);
		load_texture(gfx_system, *this, (string(path) + "textures/" + name).c_str());
	}

	Texture::Texture(GfxSystem& gfx_system, cstring path, cstring name)
		: m_name(name)
	{
		load_texture(gfx_system, *this, (string(path) + name).c_str());
	}

	Texture::Texture(GfxSystem& gfx_system, cstring name, uint16_t width, uint16_t height, array<uint8_t> data)
		: m_name(name)
		, m_width(width)
		, m_height(height)
	{
		UNUSED(gfx_system);
		const bgfx::Memory* memory = bgfx::alloc(sizeof(uint8_t) * data.m_count);
		std::copy(data.m_pointer, data.m_pointer + data.m_count, memory->data);
		m_texture = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::RGBA8, GFX_TEXTURE_POINT, memory);
	}

	Texture::~Texture()
	{}
}