#include "SendManager.h"

void SendManager::SendPacket(GameObject* player, void *packet) {
	

	EXOver *o = new EXOver;
	char *p = reinterpret_cast<char *>(packet);
	memcpy(o->ioBuf, packet, p[0]);
	o->eventType = EVT_SEND;
	o->wsaBuf.buf = o->ioBuf;
	o->wsaBuf.len = p[0];
	ZeroMemory(&o->wsaOver, sizeof(WSAOVERLAPPED));

	int ret = WSASend(dynamic_cast<Player*>(player)->s_, &o->wsaBuf, 1, NULL, 0, &o->wsaOver, NULL);
	if (0 != ret) {
		//int err_no = WSAGetLastError();
		//if (WSA_IO_PENDING != err_no) // WSA_IO_PENDING 뭐냐? 샌드가 계속 실행되고 있다. IOCP는 WSA_IO_PENDING 에러를 리턴한다.
		//	error_display("Error in SendPacket:", err_no);
	}


	//printf("SendPacket to Client [ %d ] Type [ %d ] Size [ %d ]\n", cl, (int)p[1], (int)p[0]);
}

void SendManager::SendObjectPos(GameObject* player, GameObject* object)
{

	sc_packet_pos pos_p;

	if (TYPE_MONSTER == object->objectType_)
		pos_p.id = object->ID_ + NPC_ID_START_NUM;
	else
		pos_p.id = object->ID_;

	pos_p.size = sizeof(sc_packet_pos);
	pos_p.type = SC_POS;
	pos_p.posX = object->world_._41;
	pos_p.posY = object->world_._42;
	pos_p.posZ = object->world_._43;

	SendPacket(player, &pos_p);
}

void SendManager::SendObjectLook(GameObject* player, GameObject* object)
{
	sc_packet_look_degree degree_p;

	if (TYPE_MONSTER == object->objectType_)
		degree_p.id = object->ID_ + NPC_ID_START_NUM;
	else
		degree_p.id = object->ID_;

	degree_p.size = sizeof(sc_packet_look_degree);
	degree_p.type = SC_ROTATE;
	degree_p.lookDegree = object->lookDegree_;

	SendPacket(player, &degree_p);
}

void SendManager::SendObjectState(GameObject* player, GameObject* object)
{
	sc_packet_state state_p;

	if (TYPE_MONSTER == object->objectType_)
		state_p.id = object->ID_ + NPC_ID_START_NUM;
	else
		state_p.id = object->ID_;

	state_p.size = sizeof(sc_packet_state);
	state_p.type = SC_STATE;
	state_p.state = object->state_;

	SendPacket(player, &state_p);
}

void SendManager::SendPutObject(GameObject* player, GameObject* object)
{
	sc_packet_put_player put_p;

	if (TYPE_MONSTER == object->objectType_)
		put_p.id = object->ID_ + NPC_ID_START_NUM;
	else
		put_p.id = object->ID_;

	put_p.size = sizeof(sc_packet_put_player);
	put_p.type = SC_PUT_PLAYER;
	put_p.posX = object->world_._41;
	put_p.posY = object->world_._42;
	put_p.posZ = object->world_._43;
	put_p.lookDegree = object->lookDegree_;
	put_p.state = object->state_;

	SendPacket(player, &put_p);
}

void SendManager::SendPutMonster(GameObject* player, GameObject* object)
{
	sc_packet_put_monster put_p;

	if (TYPE_MONSTER == object->objectType_)
		put_p.id = object->ID_ + NPC_ID_START_NUM;
	else
		put_p.id = object->ID_;

	put_p.size = sizeof(sc_packet_put_monster);
	put_p.type = SC_PUT_MONSTER;
	put_p.posX = object->world_._41;
	put_p.posY = object->world_._42;
	put_p.posZ = object->world_._43;
	put_p.lookDegree = object->lookDegree_;
	put_p.state = object->state_;
	put_p.monsterType = dynamic_cast<NPC*>(object)->objectType_;

	SendPacket(player, &put_p);
}

void SendManager::SendRemoveObject(GameObject* player, GameObject* object)
{
	sc_packet_remove_object p;

	if (TYPE_MONSTER == object->objectType_)
		p.id = object->ID_ + NPC_ID_START_NUM;
	else
		p.id = object->ID_;

	p.size = sizeof(sc_packet_remove_object);
	p.type = SC_REMOVE_OBJECT;

	SendPacket(player, &p);
}

void SendManager::SendObjectHp(GameObject* player, GameObject* object)
{
	sc_packet_hp p;

	if (TYPE_MONSTER == object->objectType_)
		p.id = object->ID_ + NPC_ID_START_NUM;
	else
		p.id = object->ID_;

	p.size = sizeof(sc_packet_hp);
	p.type = SC_HP;
	p.hp = object->hp_;

	SendPacket(player, &p);
}
