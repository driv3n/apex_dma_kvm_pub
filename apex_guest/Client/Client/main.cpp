#include "main.h"

typedef struct player
{
	float dist = 0;
	int entity_team = 0;
	float boxMiddle = 0;
	float h_y = 0;
	float width = 0;
	float height = 0;
	float b_x = 0;
	float b_y = 0;
	bool knocked = false;
	bool visible = false;
	int health = 0;
	int shield = 0;
	char name[33] = { 0 };
}player;

uint32_t check = 0xABCD;

int aim_key = VK_RBUTTON;
int shoot_key = VK_LBUTTON;
bool active = true;
bool ready = false;
extern visuals v;
int aim = 2; //read
bool esp = false; //read
bool item_glow = false;
bool player_glow = true;
bool aim_no_recoil = false;
bool aiming = false; //read
uint64_t g_Base = 0; //write
float max_dist = 200.0f * 40.0f; //read
float smooth = 110.0f;
float max_fov = 15.0f;
int bone = 0;
bool thirdperson = false;
int spectators = 0; //write
int allied_spectators = 0; //write
bool chargerifle = false;
int skinchanger = false;
bool shooting = false; //read

bool valid = false; //write
bool next = false; //read write

uint64_t add[21];

bool k_f5 = 0;
bool k_f6 = 0;
bool k_f8 = 0;

bool IsKeyDown(int vk)
{
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

player players[100];

void randomBone() {
	int boneArray[2] = { 1, 2 };
	int randVal = rand() % 2;
	bone = boneArray[randVal];
	Sleep(1250);
}

int main(int argc, char** argv)
{
	add[0] = (uintptr_t)&check;
	add[1] = (uintptr_t)&aim;
	add[2] = (uintptr_t)&esp;
	add[3] = (uintptr_t)&aiming;
	add[4] = (uintptr_t)&g_Base;
	add[5] = (uintptr_t)&next;
	add[6] = (uintptr_t)&players[0];
	add[7] = (uintptr_t)&valid;
	add[8] = (uintptr_t)&max_dist;
	add[9] = (uintptr_t)&item_glow;
	add[10] = (uintptr_t)&player_glow;
	add[11] = (uintptr_t)&aim_no_recoil;
	add[12] = (uintptr_t)&smooth;
	add[13] = (uintptr_t)&max_fov;
	add[14] = (uintptr_t)&bone;
	add[15] = (uintptr_t)&thirdperson;
	add[16] = (uintptr_t)&spectators;
	add[17] = (uintptr_t)&allied_spectators;
	add[18] = (uintptr_t)&chargerifle;
	add[19] = (uintptr_t)&shooting;
	add[20] = (uintptr_t)&skinchanger;


	printf(XorStr("add offset: 0x%I64x\n"), (uint64_t)&add[0] - (uint64_t)GetModuleHandle(NULL));
	printf(XorStr("Waiting for host process...\n"));
	while (check == 0xABCD)
	{
		if (IsKeyDown(VK_F4))
		{
			active = false;
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	if (active)
	{
		ready = true;
		printf(XorStr("Ready\n\n"));
	}
		
	while (active)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (IsKeyDown(VK_F4))
		{
			active = false;
		}

		if (IsKeyDown(VK_F5) && k_f5 == 0)
		{
			k_f5 = 1;
			esp = !esp;
		}
		else if (!IsKeyDown(VK_F5) && k_f5 == 1)
		{
			k_f5 = 0;
		}

		if (IsKeyDown(VK_F6) && k_f6 == 0)
		{
			k_f6 = 1;
			switch (aim)
			{
			case 0:
				aim = 1;
				break;
			case 1:
				aim = 2;
				break;
			case 2:
				aim = 0;
				break;
			default:
				break;
			}
		}
		else if (!IsKeyDown(VK_F6) && k_f6 == 1)
		{
			k_f6 = 0;
		}

		

		if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0) {
			aim_no_recoil = true;
			//smooth = 75.0f;
			//max_fov = 10.0f;
			//printf(XorStr("\rNORECOIL ENABLED "));
		}
		else {
			aim_no_recoil = false;
			//smooth = 50.0f;
			//max_fov = 10.0f;
			//printf(XorStr("\rNORECOIL DISABLED"));
		}
		if (IsKeyDown(VK_F10))
		{
			printf(XorStr("ENTER SKIN ID (1-20):"));
			scanf("%d", &skinchanger);
		}
		if (IsKeyDown(VK_F9))
		{
			thirdperson = !thirdperson;
			
		}

		if (IsKeyDown(VK_F8) && k_f8 == 0)
		{
			k_f8 = 1;
			item_glow = !item_glow;
		}
		else if (!IsKeyDown(VK_F8) && k_f8 == 1)
		{
			k_f8 = 0;
		}

		if (IsKeyDown(VK_LEFT))
		{
			if (max_dist > 100.0f * 40.0f)
				max_dist -= 50.0f * 40.0f;
			std::this_thread::sleep_for(std::chrono::milliseconds(130));
		}

		if (IsKeyDown(VK_RIGHT))
		{
			if (max_dist < 800.0f * 40.0f)
				max_dist += 50.0f * 40.0f;
			std::this_thread::sleep_for(std::chrono::milliseconds(130));
		}

		if (IsKeyDown(aim_key))
		{
			aiming = true;
			randomBone();
		}
		else
			aiming = false;

		if (IsKeyDown(shoot_key))
		{
			aiming = true;
			randomBone();
		}
		else
			aiming = false;

		if (IsKeyDown(shoot_key))
			shooting = true;
		else
			shooting = false;
	}
	ready = false;

}