#include "Precomp.h"
#include "application/AppContext.h"
#include "MeshLoader.h"
#include "IQMLoader.h"
#include "opengl/Mesh.h"
#include "utility/Logger.h"
#include "core/FileSystem.h"
#include "boost/filesystem/path.hpp"

MeshLoader::MeshLoader()
{
	AddLoader(new IQMLoader());
}

MeshLoader::~MeshLoader()
{
	for (IMeshLoader * l : m_loaders)
		delete l;
}

void MeshLoader::AddLoader(IMeshLoader * loader)
{
	auto it = std::find_if(m_loaders.begin(), m_loaders.end(), [&loader](IMeshLoader * l)
	{
		return l == loader;
	});

	if (it == m_loaders.end())
		m_loaders.push_back(loader);
}

MeshPtr MeshLoader::Load(const Path & fileName)
{
	bool found_usable_loader = false;
	resource<Mesh> res;
	res = this->get_resource(fileName);

	if (res._resource)
	{
		GetContext().GetLogger()->log(LOG_LOG, "Found mesh in cache, skipping loading.");
		return res._resource;
	}

	std::string ext = fileName.extension().generic_string();
	GetContext().GetLogger()->log(LOG_LOG, "Mesh extension: '%s'", ext.c_str());

	///REFACTOR: Search loader by extension func, return loader. Then try loading.
	if (GetContext().GetFileSystem()->FileExists(fileName.c_str()))
		for (IMeshLoader * l : m_loaders)
		{
			if (l->CheckByExtension(ext))
			{
				found_usable_loader = true;

				FilePtr file = GetContext().GetFileSystem()->OpenRead(fileName);

				if (file->IsOpen())
				{
					ByteBufferPtr buffer = file->Read();
					GetContext().GetLogger()->log(LOG_LOG, "Mesh file size: %u", buffer->size());

					res._path = fileName;
					res._resource = MeshPtr(l->Load((const char*)buffer->data(), buffer->size()));
					this->add_resource(res);
					res._resource->Init();
					return res._resource;
				}
				else
				{
					GetContext().GetLogger()->log(LOG_ERROR, "File %s appears to be empty.", fileName.generic_string().c_str());
				}
			}
		}

	if (!found_usable_loader)
		GetContext().GetLogger()->log(LOG_ERROR, "No loader can load '%s' mesh files.", ext.c_str());

	return nullptr;
}