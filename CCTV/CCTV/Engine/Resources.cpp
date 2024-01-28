#include "Resources.h"

using namespace Engine;

std::unordered_map<std::string, ResourceObject*> Resources::storage;

bool Resources::ResourceExist(std::string path)
{
	return storage.count(path);
}

ResourceObject* Resources::GetResource(std::string path)
{
	return storage[path];
}

void Resources::Clear()
{
	for (auto& it : storage)
	{
		ResourceObject* resource = it.second;

		resource->Dispose();
		delete resource;
	}

	storage.clear();
}