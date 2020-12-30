#include "Networks.h"
#include "ReplicationManagerClient.h"

// TODO(you): World state replication lab session

void ReplicationManagerClient::read(const InputMemoryStream& packet)
{
	int actions_count;
	packet >> actions_count;

	for (int i = 0; i < actions_count; ++i)
	{
		ReplicationAction rep_action;
		packet >> rep_action;

		uint32 netId;
		packet >> netId;



		GameObject* go = App->modLinkingContext->getNetworkGameObject(netId);

		packet >> go->position.x;
		packet >> go->position.y;
		packet >> go->angle;
		packet >> go->size.x;
		packet >> go->size.y;
		packet >> go->tag;

		if (rep_action == ReplicationAction::Create)
		{
			go = App->modGameObject->Instantiate();
			App->modLinkingContext->registerNetworkGameObjectWithNetworkId(go, netId);

			int num;
			packet >> num;

			if (num == 1)
			{
				packet >> go->sprite->texture->filename;
				packet >> go->sprite->color.r;
				packet >> go->sprite->color.g;
				packet >> go->sprite->color.b;
				packet >> go->sprite->color.a;
				packet >> go->sprite->order;
				packet >> go->sprite->pivot.x;
				packet >> go->sprite->pivot.y;
			}

			BehaviourType behaviour = BehaviourType::None;
			packet >> behaviour;

			if (behaviour != BehaviourType::None)
			{
				go->behaviour->read(packet);
			}

		}

		if (rep_action == ReplicationAction::Update)
		{

		}

		if (rep_action == ReplicationAction::Destroy)
		{
			App->modLinkingContext->unregisterNetworkGameObject(go);
			App->modGameObject->Destroy(go);
		}

		if (rep_action == ReplicationAction::None)
		{

		}
	}
}
