#include "Networks.h"
#include "ReplicationManagerServer.h"

// TODO(you): World state replication lab session
void ReplicationManagerServer::create(uint32 networkId)
{
	action[networkId] = ReplicationAction::Create;
}

void ReplicationManagerServer::update(uint32 networkId)
{
	action[networkId] = ReplicationAction::Update;
}

void ReplicationManagerServer::destroy(uint32 networkId)
{
	action[networkId] = ReplicationAction::Destroy;
}

void ReplicationManagerServer::write(OutputMemoryStream &packet)
{

	for (auto it = action.begin(); it != action.end(); ++it)
	{
		if ((*it).second == ReplicationAction::Create)
		{
			GameObject* go = App->modLinkingContext->getNetworkGameObject((*it).first);

		}

		if ((*it).second == ReplicationAction::Update)
		{
			GameObject* go = App->modLinkingContext->getNetworkGameObject((*it).first);
		}

		if ((*it).second == ReplicationAction::Destroy)
		{
			it = action.erase(it);
		}

		if ((*it).second == ReplicationAction::None)
		{
			
		}


	}
}