#pragma once

#include <unordered_map>
#include "OSHandler.h"

namespace Engine
{
	class ResourceObject
	{
	public:
		virtual void Load(std::string fullpath) { };
		virtual void Dispose() {};
	};

	class Resources
	{
	public:
		template <typename T>
		static T* Load(std::string path)
		{
			if (!ResourceExist(path))
			{
				std::string fullpath = OSHandler::GetPath() + "/resources/" + path;

				T* t_resource = new T();
				ResourceObject* resource = (ResourceObject*)t_resource;
				resource->Load(fullpath);

				storage[path] = resource;
			}

			return (T*)GetResource(path);
		}

		static bool ResourceExist(std::string path);
		static ResourceObject* GetResource(std::string path);
		static void Clear();
	private:
		static std::unordered_map<std::string, ResourceObject*> storage;
	};
}