#include "Networks.h"
#include "ReplicationManagerClient.h"

// TODO(you): World state replication lab session

void ReplicationManagerClient::read(const InputMemoryStream& packet)
{
	size_t actions_count;
	packet >> actions_count;

	for (int i = 0; i < actions_count; ++i)
	{
		uint32 netId;
		packet >> netId;

		ReplicationAction rep_action;
		packet >> rep_action;

		if (rep_action != ReplicationAction::None)
		{
			GameObject* go = nullptr;

			if (rep_action == ReplicationAction::Create)
			{
				go = App->modGameObject->Instantiate();
				App->modLinkingContext->registerNetworkGameObjectWithNetworkId(go, netId);

				if (go == nullptr)
					continue;

				packet >> go->position.x;
				packet >> go->position.y;
				packet >> go->angle;
				packet >> go->size.x;
				packet >> go->size.y;
				packet >> go->tag;

				int num;
				packet >> num;

				if (num == 1)
				{
					go->sprite = App->modRender->addSprite(go);
					int texture_id;
					packet >> texture_id;
					go->sprite->texture = App->modTextures->GetTextureByID(texture_id);
					packet >> go->sprite->color.r;
					packet >> go->sprite->color.g;
					packet >> go->sprite->color.b;
					packet >> go->sprite->color.a;
					packet >> go->sprite->order;
					packet >> go->sprite->pivot.x;
					packet >> go->sprite->pivot.y;
				}

				int behaviour;
				packet >> behaviour;


				go->behaviour = App->modBehaviour->addBehaviour((BehaviourType)behaviour, go);
				/*if (go->behaviour != nullptr)
				{
					go->behaviour->read(packet);
				}*/
				

			}

			if (rep_action == ReplicationAction::Update)
			{
				go = App->modLinkingContext->getNetworkGameObject(netId);
				if (go == nullptr)
					continue;

				packet >> go->position.x;
				packet >> go->position.y;
				packet >> go->angle;
				packet >> go->size.x;
				packet >> go->size.y;
				packet >> go->tag;

				BehaviourType behaviour = BehaviourType::None;
				//packet >> behaviour;

				if (go->behaviour != nullptr)
				{
					go->behaviour->read(packet);
				}
			
			}
			if (rep_action == ReplicationAction::Destroy)
			{
				GameObject* gameobj = App->modLinkingContext->getNetworkGameObject(netId);
				App->modLinkingContext->unregisterNetworkGameObject(gameobj);
				Destroy(gameobj);
			}
		}
		
	}
}
