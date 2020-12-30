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
	packet << action.size();

	for (auto it = action.begin(); it != action.end(); ++it)
	{
		GameObject* go = App->modLinkingContext->getNetworkGameObject((*it).first);

		packet << go->position.x;
		packet << go->position.y;
		packet << go->angle;
		packet << go->size.x;
		packet << go->size.y;
		packet << go->tag;

		if ((*it).second == ReplicationAction::Create)
		{
			if (go->sprite != nullptr && go->sprite->texture != nullptr)
			{
				packet << 1;
				packet << go->sprite->texture->filename;
				packet << go->sprite->color.r;
				packet << go->sprite->color.g;
				packet << go->sprite->color.b;
				packet << go->sprite->color.a;
				packet << go->sprite->order;
				packet << go->sprite->pivot.x;
				packet << go->sprite->pivot.y;
			}

			else
				packet << -1;

			if (go->behaviour != nullptr)
			{
				packet << (int)go->behaviour->type();
				go->behaviour->write(packet);
			}

			else
				packet << (int)BehaviourType::None;
		}

		if ((*it).second == ReplicationAction::Update)
		{

			if (go->behaviour != nullptr)
			{
				packet << (int)go->behaviour->type();
				go->behaviour->write(packet);
			}

			else
				packet << (int)BehaviourType::None;

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