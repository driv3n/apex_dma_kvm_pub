#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <random>
#include <chrono>
#include <iostream>
#include <cfloat>
#include "Game.h"
#include <thread>
//this is a test, with seconds
Memory apex_mem;
Memory client_mem;

//Just setting things up, dont edit.
bool firing_range = false;
bool active = true;
uintptr_t aimentity = 0;
uintptr_t tmp_aimentity = 0;
uintptr_t lastaimentity = 0;
float max = 999.0f;
float max_dist = 200.0f*40.0f;
int team_player = 0;
float max_fov = 25;
const int toRead = 100;
int aim = true;
bool esp = true;
bool item_glow = true;
bool player_glow = true;
extern bool aim_no_recoil;
bool aiming = false;
extern float smooth;
extern int bone;
bool thirdperson = false;


//chargerifle hack, removed but not all the way, dont edit.
bool chargerifle = false;
bool shooting = false;


//Player Glow Color and Brightness. Just setting things up, dont edit.
float glowr = 0.0f; //Red 0-255, higher is brighter color.
float glowg = 120.0f; //Green 0-255, higher is brighter color.
float glowb = 120.0f; //Blue 0-255, higher is brighter color.
//visable
float glowrviz = 0.0f; //Red 0-255, higher is brighter color.
float glowgviz = 120.0f; //Green 0-255, higher is brighter color.
float glowbviz = 120.0f; //Blue 0-255, higher is brighter color.
//knocked
float glowrknocked = 0.0f; //Red 0-255, higher is brighter color.
float glowgknocked = 120.0f; //Green 0-255, higher is brighter color.
float glowbknocked = 120.0f; //Blue 0-255, higher is brighter color.

//Removed but not all the way, dont edit.
int glowtype = 1;
int glowtype2 = 2;


//Ha think i was done ?
//Backpacks
bool lightbackpack = false;
bool medbackpack = false;
bool heavybackpack = false;


//Shield upgrades
bool shieldupgrade = false;
bool shieldupgradehead = false;
bool shielddown = false;


//heaing and Misc
bool accelerant = false;
bool phoenix = false;
bool healthlarge = false;
bool healthsmall = false;
bool shieldbattsmall = false;
bool shieldbattlarge = false;


//Ammo
bool ammosniper = false;
bool ammohc = false;
bool ammosc = false;
bool ammonrg = false;
bool ammoshotgun = false;


//Optics
bool optic = false;
bool optic2x = false;
bool opticholo1x = false;
bool opticholo1x2x = false;
bool opticthreat = false;
bool optic3x = false;
bool optic2x4x = false;
bool opticsniper6x = false;
bool opticsniper4x8x = false;
bool opticsniperthreat = false;


//Magazines
bool magsniper = false;
bool magenergy = false;
bool lightammomag = false;
bool heavyammomag = false;


//Attachments 
bool lasersight = false;
bool stocksniper = false;
bool stockregular = false;
bool suppressor = false;
bool weaponmod = false;
bool shotgunbolt = false;


//Nades
bool grenade_frag = false;
bool grenade_arc_star = false;
bool grenade_thermite = false;


//Kraber
bool weapon_kraber = false;


//Shotguns
bool weapon_mastiff = false;
bool weapon_eva8  = false;
bool weapon_peacekeeper  = false;
bool weapon_mozambique  = false;


//Energy weapons
bool weapon_lstar = false;
bool weapon_havoc = false;
bool weapon_devotion = false;
bool weapon_triple_take = false;
bool weapon_prowler  = false;
bool weapon_volt  = false;


//Heavy Weapons
bool weapon_flatline = false;
bool weapon_hemlock  = false;
bool weapon_3030_repeater = false; 
bool weapon_rampage  = false;
bool weapon_car_smg  = false;


//Light weapons
bool weapon_p2020  = false;
bool weapon_re45  = false;
bool weapon_g7_scout  = false;
bool weapon_alternator  = false;
bool weapon_r99  = false;
bool weapon_spitfire  = false;
bool weapon_r301 = false;


//Snipers.. wingman is the odd one...and the bow..
bool weapon_wingman  = false;
bool weapon_longbow  = false;
bool weapon_charge_rifle  = false;
bool weapon_sentinel  = false;
bool weapon_bow  = false;


//aim dist check. Just setting things up, dont edit.
float aimdist = 200.0f * 40.0f;


//item glow brightness. Just setting things up, dont edit.
int itemglowbrightness = 10;


//Just setting things up, dont edit.
bool actions_t = false;
bool esp_t = false;
bool aim_t = false;
bool vars_t = false;
bool item_t = false;
uint64_t g_Base;
uint64_t c_Base;
bool next2 = false;
bool valid = false;
bool lock = false;


//Player Definitions, dont edit unless you know what you are doing.
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
	int maxshield = 0;
	int armortype = 0;
	Vector EntityPosition;
	Vector LocalPlayerPosition;
	QAngle localviewangle;
	char name[33] = { 0 };
}player;


//Your in the matrix neo.
struct Matrix
{
	float matrix[16];
};


//Visual check and aim check.?
float lastvis_esp[toRead];
float lastvis_aim[toRead];


//Specator stuff. Just setting things up, dont edit.
int tmp_spec = 0, spectators = 0;
int tmp_all_spec = 0, allied_spectators = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////

void SetPlayerGlow(Entity& LPlayer, Entity& Target, int index)
{
	if (player_glow >= 1)
	{
		
		
			if (!Target.isGlowing() || (int)Target.buffer[OFFSET_GLOW_THROUGH_WALLS_GLOW_VISIBLE_TYPE] != 1) {
				float currentEntityTime = 5000.f;
				if (!isnan(currentEntityTime) && currentEntityTime > 0.f) {
					GColor color;
					if (!(firing_range) && (Target.isKnocked() || !Target.isAlive()))
					{
						color = { glowrknocked, glowgknocked, glowbknocked };
					}
					else if (Target.lastVisTime() > lastvis_aim[index] || (Target.lastVisTime() < 0.f && lastvis_aim[index] > 0.f))
					{
						color = { glowrviz, glowgviz, glowbviz };
					}
					else 
					{
						color = { glowr, glowg, glowb };
					}

					Target.enableGlow(color);
				}
			}
		
		else if((player_glow == 0) && Target.isGlowing())
		{
			Target.disableGlow();
		}
	}
}



void ProcessPlayer(Entity& LPlayer, Entity& target, uint64_t entitylist, int index)
{
	int entity_team = target.getTeamId();

	if (!target.isAlive())
	{
		float localyaw = LPlayer.GetYaw();
		float targetyaw = target.GetYaw();

		if(localyaw==targetyaw)
		{
			if(LPlayer.getTeamId() == entity_team)
				tmp_all_spec++;
			else
				tmp_spec++;
		}
		return;
	}

	Vector EntityPosition = target.getPosition();
	Vector LocalPlayerPosition = LPlayer.getPosition();
	float dist = LocalPlayerPosition.DistTo(EntityPosition);
	//Prints POS of localplayer for map cords for full map radar. only enable when adding a new map or fixing a old one, will output to console.
	//std::printf("  X: %.6f   ||    Y:%.6f",LocalPlayerPosition.x, LocalPlayerPosition.y); //Prints x and y cords of localplayer to get mainmap radar stuff.
	if (dist > aimdist) return;
	
	
	
	//Firing range stuff
	if(!firing_range)
		if (entity_team < 0 || entity_team>50 || entity_team == team_player) return;
	
	
	//Vis check aiming? dunno
	if(aim==2)
	{
		if((target.lastVisTime() > lastvis_aim[index]))
		{
			float fov = CalculateFov(LPlayer, target);
			if (fov < max)
			{
				max = fov;
				tmp_aimentity = target.ptr;
			}
		}
		else
		{
			if(aimentity==target.ptr)
			{
				aimentity=tmp_aimentity=lastaimentity=0;
			}
		}
	}
	else
	{
		float fov = CalculateFov(LPlayer, target);
		if (fov < max)
		{
			max = fov;
			tmp_aimentity = target.ptr;
		}
	}
	SetPlayerGlow(LPlayer, target, index);
	lastvis_aim[index] = target.lastVisTime();
}


//Main stuff, dont edit.
void DoActions()
{
	actions_t = true;
	while (actions_t)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		bool tmp_thirdperson = false;
		bool tmp_chargerifle = false;
		uint32_t counter = 0;

		while (g_Base!=0 && c_Base!=0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(30));	

			uint64_t LocalPlayer = 0;
			apex_mem.Read<uint64_t>(g_Base + OFFSET_LOCAL_ENT, LocalPlayer);
			if (LocalPlayer == 0) continue;

			Entity LPlayer = getEntity(LocalPlayer);

			team_player = LPlayer.getTeamId();
			if (team_player < 0 || team_player>50)
			{
				continue;
			}

			if(thirdperson && !tmp_thirdperson)
			{
				if(!aiming)
				{
					apex_mem.Write<int>(g_Base + OFFSET_THIRDPERSON, 1);
					apex_mem.Write<int>(LPlayer.ptr + OFFSET_THIRDPERSON_SV, 1);
					tmp_thirdperson = true;
				}			
			}
			else if((!thirdperson && tmp_thirdperson) || aiming)
			{
				if(tmp_thirdperson)
				{
					apex_mem.Write<int>(g_Base + OFFSET_THIRDPERSON, -1);
					apex_mem.Write<int>(LPlayer.ptr + OFFSET_THIRDPERSON_SV, 0);
					tmp_thirdperson = false;
				}	
			}

			uint64_t entitylist = g_Base + OFFSET_ENTITYLIST;

			uint64_t baseent = 0;
			apex_mem.Read<uint64_t>(entitylist, baseent);
			if (baseent == 0)
			{
				continue;
			}
			
			
			//Dont edit.
			max = 999.0f;
			tmp_aimentity = 0;
			tmp_spec = 0;
			tmp_all_spec = 0;
			if(firing_range)
			{
				int c=0;
				//Ammount of ents to loop, dont edit.
				for (int i = 0; i < 10000; i++)
				{
					uint64_t centity = 0;
					apex_mem.Read<uint64_t>(entitylist + ((uint64_t)i << 5), centity);
					if (centity == 0) continue;
					if (LocalPlayer == centity) continue;

					Entity Target = getEntity(centity);
					if (!Target.isDummy())
					{
						continue;
					}

					ProcessPlayer(LPlayer, Target, entitylist, c);
					c++;
				}
			}
			else
			{
				for (int i = 0; i < toRead; i++)
				{
					uint64_t centity = 0;
					apex_mem.Read<uint64_t>(entitylist + ((uint64_t)i << 5), centity);
					if (centity == 0) continue;
					if (LocalPlayer == centity) continue;

					Entity Target = getEntity(centity);
					if (!Target.isPlayer())
					{
						continue;
					}
					
					ProcessPlayer(LPlayer, Target, entitylist, i);

					int entity_team = Target.getTeamId();
					if (entity_team == team_player)
					{
						continue;
					}

				}
			}

			if(!spectators && !allied_spectators)
			{
				spectators = tmp_spec;
				allied_spectators = tmp_all_spec;
			}
			else
			{
				//refresh spectators count every ~2 seconds
				counter++;
				if(counter==70)
				{
					spectators = tmp_spec;
					allied_spectators = tmp_all_spec;
					counter = 0;
				}
			}

			if(!lock)
				aimentity = tmp_aimentity;
			else
				aimentity = lastaimentity;
			
			if(chargerifle)
			{
				charge_rifle_hack(LocalPlayer);
				tmp_chargerifle = true;
			}
			else
			{
				if(tmp_chargerifle)
				{
					apex_mem.Write<float>(g_Base + OFFSET_TIMESCALE + 0x68, 1.f);
					tmp_chargerifle = false;
				}
			}
		}
	}
	actions_t = false;
}

// /////////////////////////////////////////////////////////////////////////////////////////////////////

player players[toRead];

//ESP loop.. this helps right?
static void EspLoop()
{
	esp_t = true;
	while(esp_t)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		while(g_Base!=0 && c_Base!=0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if (esp)
			{
				valid = false;

				uint64_t LocalPlayer = 0;
				apex_mem.Read<uint64_t>(g_Base + OFFSET_LOCAL_ENT, LocalPlayer);
				if (LocalPlayer == 0)
				{
					next2 = true;
					while(next2 && g_Base!=0 && c_Base!=0 && esp)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}
					continue;
				}
				Entity LPlayer = getEntity(LocalPlayer);
				int team_player = LPlayer.getTeamId();
				if (team_player < 0 || team_player>50)
				{
					next2 = true;
					while(next2 && g_Base!=0 && c_Base!=0 && esp)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}
					continue;
				}
				Vector LocalPlayerPosition = LPlayer.getPosition();

				uint64_t viewRenderer = 0;
				apex_mem.Read<uint64_t>(g_Base + OFFSET_RENDER, viewRenderer);
				uint64_t viewMatrix = 0;
				apex_mem.Read<uint64_t>(viewRenderer + OFFSET_MATRIX, viewMatrix);
				Matrix m = {};
				apex_mem.Read<Matrix>(viewMatrix, m);

				uint64_t entitylist = g_Base + OFFSET_ENTITYLIST;
				
				memset(players,0,sizeof(players));

				if(firing_range)
				{
					int c=0;
					//Ammount of ents to loop, dont edit.
					for (int i = 0; i < 10000; i++)
					{
						uint64_t centity = 0;
						apex_mem.Read<uint64_t>( entitylist + ((uint64_t)i << 5), centity);
						if (centity == 0)
						{
							continue;
						}		
						
						if (LocalPlayer == centity)
						{
							continue;
						}

						Entity Target = getEntity(centity);

						if (!Target.isDummy())
						{
							continue;
						}

						if (!Target.isAlive())
						{
							continue;
						}
						int entity_team = Target.getTeamId();

						Vector EntityPosition = Target.getPosition();
						float dist = LocalPlayerPosition.DistTo(EntityPosition);

						if (dist > max_dist || dist < 50.0f)
						{	
							continue;
						}
						
						Vector bs = Vector();
						//Change res to your res here, default is 1080p but can copy paste 1440p here
						WorldToScreen(EntityPosition, m.matrix, 1920, 1080, bs); //2560, 1440
						if (esp)
						{
							Vector hs = Vector();
							Vector HeadPosition = Target.getBonePositionByHitbox(0);
							//Change res to your res here, default is 1080p but can copy paste 1440p here
							WorldToScreen(HeadPosition, m.matrix, 1920, 1080, hs); //2560, 1440
							float height = abs(abs(hs.y) - abs(bs.y));
							float width = height / 2.0f;
							float boxMiddle = bs.x - (width / 2.0f);
							int health = Target.getHealth();
							int shield = Target.getShield();
							int maxshield = Target.getMaxshield();
							int armortype = Target.getArmortype();
							players[c] = 
							{
								dist,
								entity_team,
								boxMiddle,
								hs.y,
								width,
								height,
								bs.x,
								bs.y,
								0,
								(Target.lastVisTime() > lastvis_esp[c]),
								health,
								shield,
								maxshield,
								armortype
								
							};
							Target.get_name(g_Base, i-1, &players[c].name[0]);
							lastvis_esp[c] = Target.lastVisTime();
							valid = true;
							c++;
						}
					}
				}	
				else
				{
					for (int i = 0; i < toRead; i++)
					{
						uint64_t centity = 0;
						apex_mem.Read<uint64_t>( entitylist + ((uint64_t)i << 5), centity);
						if (centity == 0)
						{
							continue;
						}
						
						if (LocalPlayer == centity)
						{
							continue;
						}

						Entity Target = getEntity(centity);
						
						if (!Target.isPlayer())
						{
							continue;
						}

						if (!Target.isAlive())
						{
							continue;
						}

						int entity_team = Target.getTeamId();
						if (entity_team < 0 || entity_team>50 || entity_team == team_player)
						{
							continue;
						}

						Vector EntityPosition = Target.getPosition();
						float dist = LocalPlayerPosition.DistTo(EntityPosition);
						if (dist > max_dist || dist < 50.0f)
						{	
							continue;
						}

						Vector bs = Vector();
						//Change res to your res here, default is 1080p but can copy paste 1440p here
						WorldToScreen(EntityPosition, m.matrix, 1920, 1080, bs); //2560, 1440
						if (esp)
						{
							Vector hs = Vector();
							Vector HeadPosition = Target.getBonePositionByHitbox(0);
							//Change res to your res here, default is 1080p but can copy paste 1440p here
							WorldToScreen(HeadPosition, m.matrix, 1920, 1080, hs); //2560, 1440
							float height = abs(abs(hs.y) - abs(bs.y));
							float width = height / 2.0f;
							float boxMiddle = bs.x - (width / 2.0f);
							int health = Target.getHealth();
							int shield = Target.getShield();
							int maxshield = Target.getMaxshield();
							int armortype = Target.getArmortype();
							Vector EntityPosition = Target.getPosition();
							Vector LocalPlayerPosition = LPlayer.getPosition();
							QAngle localviewangle = LPlayer.GetViewAngles();
							players[i] = 
							{
								dist,
								entity_team,
								boxMiddle,
								hs.y,
								width,
								height,
								bs.x,
								bs.y,
								Target.isKnocked(),
								(Target.lastVisTime() > lastvis_esp[i]),
								health,
								shield,
								maxshield,
								armortype,
								EntityPosition,
								LocalPlayerPosition,
								localviewangle
							};
							Target.get_name(g_Base, i-1, &players[i].name[0]);
							lastvis_esp[i] = Target.lastVisTime();
							valid = true;
						}
					}
				}

				next2 = true;
				while(next2 && g_Base!=0 && c_Base!=0 && esp)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			}
		}
	}
	esp_t = false;
}
//Aimbot Loop stuff
static void AimbotLoop()
{
	aim_t = true;
	while (aim_t)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		while (g_Base!=0 && c_Base!=0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if (aim>0)
			{
				if (aimentity == 0 || !aiming)
				{
					lock=false;
					lastaimentity=0;
					continue;
				}
				lock=true;
				lastaimentity = aimentity;
				uint64_t LocalPlayer = 0;
				apex_mem.Read<uint64_t>(g_Base + OFFSET_LOCAL_ENT, LocalPlayer);
				if (LocalPlayer == 0) continue;
				Entity LPlayer = getEntity(LocalPlayer);
				QAngle Angles = CalculateBestBoneAim(LPlayer, aimentity, max_fov);
				if (Angles.x == 0 && Angles.y == 0)
				{
					lock=false;
					lastaimentity=0;
					continue;
				}
				LPlayer.SetViewAngles(Angles);
			}
		}
	}
	aim_t = false;
}
//Client memory vars/reads. HAVE to match windows client numbers.
static void set_vars(uint64_t add_addr)
{
	printf("Reading the client vars...\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	//Get addresses of client vars
	uint64_t check_addr = 0;
	client_mem.Read<uint64_t>(add_addr, check_addr);
	uint64_t aim_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t), aim_addr);
	uint64_t esp_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*2, esp_addr);
	uint64_t aiming_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*3, aiming_addr);
	uint64_t g_Base_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*4, g_Base_addr);
	uint64_t next2_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*5, next2_addr);
	uint64_t player_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*6, player_addr);
	uint64_t valid_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*7, valid_addr);
	uint64_t max_dist_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*8, max_dist_addr);
	uint64_t item_glow_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*9, item_glow_addr);
	uint64_t player_glow_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*10, player_glow_addr);
	uint64_t aim_no_recoil_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*11, aim_no_recoil_addr);
	uint64_t smooth_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*12, smooth_addr);
	uint64_t max_fov_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*13, max_fov_addr);
	uint64_t bone_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*14, bone_addr);
	uint64_t thirdperson_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*15, thirdperson_addr);
	uint64_t spectators_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*16, spectators_addr);
	uint64_t allied_spectators_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*17, allied_spectators_addr);
	uint64_t glowr_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*18, glowr_addr);
	uint64_t glowg_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*19, glowg_addr);
	uint64_t glowb_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*20, glowb_addr);
	uint64_t firing_range_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*21, firing_range_addr);
	uint64_t lightbackpack_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*22, lightbackpack_addr);
	uint64_t medbackpack_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*23, medbackpack_addr);
	uint64_t heavybackpack_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*24, heavybackpack_addr);
	uint64_t shieldupgrade_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*25, shieldupgrade_addr);
	uint64_t shieldupgradehead_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*26, shieldupgradehead_addr);
	uint64_t accelerant_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*27, accelerant_addr);
	uint64_t phoenix_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*28, phoenix_addr);
	uint64_t healthlarge_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*29, healthlarge_addr);
	uint64_t healthsmall_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*30, healthsmall_addr);
	uint64_t shieldbattsmall_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*31, shieldbattsmall_addr);
	uint64_t shieldbattlarge_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*32, shieldbattlarge_addr);
	uint64_t ammosniper_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*33, ammosniper_addr);
	uint64_t ammohc_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*34, ammohc_addr);
	uint64_t optic_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*35, optic_addr);
	uint64_t ammosc_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*36, ammosc_addr);
	uint64_t ammonrg_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*37, ammonrg_addr);
	uint64_t ammoshotgun_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*38, ammoshotgun_addr);
	uint64_t lasersight_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*39, lasersight_addr);
	uint64_t magsniper_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*40, magsniper_addr);
	uint64_t magenergy_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*41, magenergy_addr);
	uint64_t stocksniper_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*42, stocksniper_addr);
	uint64_t stockregular_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*43, stockregular_addr);
	uint64_t shielddown_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*44, shielddown_addr);
	uint64_t lightammomag_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*45, lightammomag_addr);
	uint64_t heavyammomag_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*46, heavyammomag_addr);
	uint64_t optic2x_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*47, optic2x_addr);
	uint64_t opticholo1x_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*48, opticholo1x_addr);
	uint64_t opticholo1x2x_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*49, opticholo1x2x_addr);
	uint64_t opticthreat_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*50, opticthreat_addr);
	uint64_t optic3x_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*51, optic3x_addr);
	uint64_t optic2x4x_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*52, optic2x4x_addr);
	uint64_t opticsniper6x_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*53, opticsniper6x_addr);
	uint64_t opticsniper4x8x_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*54, opticsniper4x8x_addr);
	uint64_t opticsniperthreat_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*55, opticsniperthreat_addr);
	uint64_t suppressor_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*56, suppressor_addr);
	uint64_t weaponmod_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*57, weaponmod_addr);
	uint64_t grenade_frag_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*58, grenade_frag_addr);
	uint64_t grenade_arc_star_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*59, grenade_arc_star_addr);
	uint64_t grenade_thermite_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*60, grenade_thermite_addr);
	uint64_t shotgunbolt_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*61, shotgunbolt_addr);
	uint64_t weapon_kraber_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*62, weapon_kraber_addr);
	uint64_t weapon_mastiff_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*63, weapon_mastiff_addr);
	uint64_t weapon_lstar_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*64, weapon_lstar_addr);
	uint64_t weapon_havoc_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*65, weapon_havoc_addr);
	uint64_t weapon_devotion_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*66, weapon_devotion_addr);
	uint64_t weapon_triple_take_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*67, weapon_triple_take_addr);
	uint64_t weapon_flatline_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*68, weapon_flatline_addr);
	uint64_t weapon_hemlock_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*69, weapon_hemlock_addr);
	uint64_t weapon_g7_scout_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*70, weapon_g7_scout_addr);
	uint64_t weapon_alternator_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*71, weapon_alternator_addr);
	uint64_t weapon_r99_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*72, weapon_r99_addr);
	uint64_t weapon_prowler_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*73, weapon_prowler_addr);
	uint64_t weapon_volt_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*74, weapon_volt_addr);
	uint64_t weapon_longbow_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*75, weapon_longbow_addr);
	uint64_t weapon_charge_rifle_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*76, weapon_charge_rifle_addr);
	uint64_t weapon_spitfire_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*77, weapon_spitfire_addr);
	uint64_t weapon_r301_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*78, weapon_r301_addr);
	uint64_t weapon_eva8_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*79, weapon_eva8_addr);
	uint64_t weapon_peacekeeper_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*80, weapon_peacekeeper_addr);
	uint64_t weapon_mozambique_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*81, weapon_mozambique_addr);
	uint64_t weapon_wingman_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*82, weapon_wingman_addr);
	uint64_t weapon_p2020_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*83, weapon_p2020_addr);
	uint64_t weapon_re45_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*84, weapon_re45_addr);
	uint64_t weapon_sentinel_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*85, weapon_sentinel_addr);
	uint64_t weapon_bow_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*86, weapon_bow_addr);
	uint64_t weapon_3030_repeater_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*87, weapon_3030_repeater_addr);
	uint64_t weapon_rampage_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*88, weapon_rampage_addr);
	uint64_t weapon_car_smg_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*89, weapon_car_smg_addr);
	uint64_t aimdist_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*90, aimdist_addr);
	uint64_t itemglowbrightness_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*91, itemglowbrightness_addr);
	//glow visable
	uint64_t glowrviz_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*92, glowrviz_addr);
	uint64_t glowgviz_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*93, glowgviz_addr);
	uint64_t glowbviz_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*94, glowbviz_addr);
	//knocked
	uint64_t glowrknocked_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*95, glowrknocked_addr);
	uint64_t glowgknocked_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*96, glowgknocked_addr);
	uint64_t glowbknocked_addr = 0;
	client_mem.Read<uint64_t>(add_addr + sizeof(uint64_t)*97, glowbknocked_addr);
	
	//good god 97 of em.. why (lol)
	

	uint32_t check = 0;
	client_mem.Read<uint32_t>(check_addr, check);
	
	if(check != 0xABCD)
	{
		//Add offset msg
		printf("Incorrect values read. Check if the add_off is correct. Quitting.\n");
		active = false;
		return;
	}
	vars_t = true;
	while(vars_t)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if(c_Base!=0 && g_Base!=0)
		{
			client_mem.Write<uint32_t>(check_addr, 0);
			printf("\nLet the fun begin!\n");
		}

		while(c_Base!=0 && g_Base!=0)
		{
			//same as above, has to match with eveything else
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			client_mem.Write<uint64_t>(g_Base_addr, g_Base);
			client_mem.Write<int>(spectators_addr, spectators);
			client_mem.Write<int>(allied_spectators_addr, allied_spectators);
			client_mem.Read<int>(aim_addr, aim);
			client_mem.Read<bool>(esp_addr, esp);
			client_mem.Read<bool>(aiming_addr, aiming);
			client_mem.Read<float>(max_dist_addr, max_dist);
			client_mem.Read<bool>(item_glow_addr, item_glow);
			client_mem.Read<bool>(player_glow_addr, player_glow);
			client_mem.Read<bool>(aim_no_recoil_addr, aim_no_recoil);
			client_mem.Read<float>(smooth_addr, smooth);
			client_mem.Read<float>(max_fov_addr, max_fov);
			client_mem.Read<int>(bone_addr, bone);
			client_mem.Read<bool>(thirdperson_addr, thirdperson);
			client_mem.Read<float>(glowr_addr, glowr);
			client_mem.Read<float>(glowg_addr, glowg);
			client_mem.Read<float>(glowb_addr, glowb);
			client_mem.Read<bool>(firing_range_addr, firing_range);
			client_mem.Read<bool>(lightbackpack_addr, lightbackpack);
			client_mem.Read<bool>(medbackpack_addr, medbackpack);
			client_mem.Read<bool>(heavybackpack_addr, heavybackpack);
			client_mem.Read<bool>(shieldupgrade_addr, shieldupgrade);
			client_mem.Read<bool>(shieldupgradehead_addr, shieldupgradehead);
			client_mem.Read<bool>(accelerant_addr, accelerant);
			client_mem.Read<bool>(phoenix_addr, phoenix);
			client_mem.Read<bool>(healthlarge_addr, healthlarge);
			client_mem.Read<bool>(healthsmall_addr, healthsmall);
			client_mem.Read<bool>(shieldbattsmall_addr, shieldbattsmall);
			client_mem.Read<bool>(shieldbattlarge_addr, shieldbattlarge);
			client_mem.Read<bool>(ammosniper_addr, ammosniper);
			client_mem.Read<bool>(ammohc_addr, ammohc);
			client_mem.Read<bool>(optic_addr, optic);
			client_mem.Read<bool>(ammosc_addr, ammosc);
			client_mem.Read<bool>(ammonrg_addr, ammonrg);
			client_mem.Read<bool>(ammoshotgun_addr, ammoshotgun);
			client_mem.Read<bool>(lasersight_addr, lasersight);
			client_mem.Read<bool>(magsniper_addr, magsniper);
			client_mem.Read<bool>(magenergy_addr , magenergy);
			client_mem.Read<bool>(stocksniper_addr, stocksniper);
			client_mem.Read<bool>(stockregular_addr, stockregular);
			client_mem.Read<bool>(shielddown_addr, shielddown);
			client_mem.Read<bool>(lightammomag_addr, lightammomag);
			client_mem.Read<bool>(heavyammomag_addr, heavyammomag);
			client_mem.Read<bool>(optic2x_addr, optic2x);
			client_mem.Read<bool>(opticholo1x_addr, opticholo1x);
			client_mem.Read<bool>(opticholo1x2x_addr, opticholo1x2x);
			client_mem.Read<bool>(opticthreat_addr, opticthreat);
			client_mem.Read<bool>(optic3x_addr, optic3x);
			client_mem.Read<bool>(optic2x4x_addr, optic2x4x);
			client_mem.Read<bool>(opticsniper6x_addr, opticsniper6x);
			client_mem.Read<bool>(opticsniper4x8x_addr, opticsniper4x8x);
			client_mem.Read<bool>(opticsniperthreat_addr, opticsniperthreat);
			client_mem.Read<bool>(suppressor_addr, suppressor);
			client_mem.Read<bool>(weaponmod_addr, weaponmod);
			client_mem.Read<bool>(grenade_frag, grenade_frag);
			client_mem.Read<bool>(grenade_arc_star, grenade_arc_star);
			client_mem.Read<bool>(grenade_thermite_addr, grenade_thermite);
			client_mem.Read<bool>(shotgunbolt_addr, shotgunbolt);
			client_mem.Read<bool>(weapon_kraber_addr, weapon_kraber);
			client_mem.Read<bool>(weapon_mastiff_addr, weapon_mastiff);
			client_mem.Read<bool>(weapon_lstar_addr, weapon_lstar);
			client_mem.Read<bool>(weapon_havoc_addr, weapon_havoc);
			client_mem.Read<bool>(weapon_devotion_addr, weapon_devotion);
			client_mem.Read<bool>(weapon_triple_take_addr, weapon_triple_take);
			client_mem.Read<bool>(weapon_flatline_addr, weapon_flatline);
			client_mem.Read<bool>(weapon_hemlock_addr, weapon_hemlock);
			client_mem.Read<bool>(weapon_g7_scout_addr, weapon_g7_scout);
			client_mem.Read<bool>(weapon_alternator_addr, weapon_alternator);
			client_mem.Read<bool>(weapon_r99_addr, weapon_r99);
			client_mem.Read<bool>(weapon_prowler_addr, weapon_prowler);
			client_mem.Read<bool>(weapon_volt_addr, weapon_volt);
			client_mem.Read<bool>(weapon_longbow_addr, weapon_longbow);
			client_mem.Read<bool>(weapon_charge_rifle_addr, weapon_charge_rifle);
			client_mem.Read<bool>(weapon_spitfire_addr, weapon_spitfire);
			client_mem.Read<bool>(weapon_r301_addr, weapon_r301);
			client_mem.Read<bool>(weapon_eva8_addr, weapon_eva8);
			client_mem.Read<bool>(weapon_peacekeeper_addr, weapon_peacekeeper);
			client_mem.Read<bool>(weapon_mozambique_addr, weapon_mozambique);
			client_mem.Read<bool>(weapon_wingman_addr, weapon_wingman);
			client_mem.Read<bool>(weapon_p2020_addr, weapon_p2020);
			client_mem.Read<bool>(weapon_re45_addr, weapon_re45);
			client_mem.Read<bool>(weapon_sentinel_addr, weapon_sentinel);
			client_mem.Read<bool>(weapon_bow_addr, weapon_bow);
			client_mem.Read<bool>(weapon_3030_repeater_addr, weapon_3030_repeater);
			client_mem.Read<bool>(weapon_rampage_addr, weapon_rampage);
			client_mem.Read<bool>(weapon_car_smg_addr, weapon_car_smg);
			client_mem.Read<float>(aimdist_addr, aimdist);
			client_mem.Read<int>(itemglowbrightness_addr, itemglowbrightness);
			//glow visable
			client_mem.Read<float>(glowrviz_addr, glowrviz);
			client_mem.Read<float>(glowgviz_addr, glowgviz);
			client_mem.Read<float>(glowbviz_addr, glowbviz);
//knocked
			client_mem.Read<float>(glowrknocked_addr, glowrknocked);
			client_mem.Read<float>(glowgknocked_addr, glowgknocked);
			client_mem.Read<float>(glowbknocked_addr, glowbknocked);


			if(esp && next2)
			{
				if(valid)
				client_mem.WriteArray<player>(player_addr, players, toRead);
				client_mem.Write<bool>(valid_addr, valid);
				client_mem.Write<bool>(next2_addr, true); //next2

				bool next2_val = false;
				do
				{
					client_mem.Read<bool>(next2_addr, next2_val);
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				} while (next2_val && g_Base!=0 && c_Base!=0);
				
				next2 = false;
			}
		}
	}
	vars_t = false;
}

static void item_glow_t()
{
	item_t = true;
	while(item_t)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		int k = 0;
		while(g_Base!=0 && c_Base!=0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			uint64_t entitylist = g_Base + OFFSET_ENTITYLIST;
			if (item_glow)
			{
				//item ENTs to loop, 10k-15k is normal. 10k might be better but will not show all the death boxes i think.
				for (int i = 0; i < 15000; i++)
				{
					uint64_t centity = 0;
					apex_mem.Read<uint64_t>(entitylist + ((uint64_t)i << 5), centity);
					if (centity == 0) continue;
					Item item = getItem(centity);
 
 
					
					
					if(item.isItem() && !item.isGlowing())
					{
						//item.enableGlow();
					}
					//Item filter glow name setup and search.
					char glowName[200] = { 0 };
					uint64_t name_ptr;
					apex_mem.Read<uint64_t>(centity + OFFSET_MODELNAME, name_ptr);
					apex_mem.ReadArray<char>(name_ptr, glowName, 200);
					//Prints stuff you want to console
					//if (strstr(glowName, "mdl/weapons/")) 
					//{
					//printf("%s\n", glowName);
					//}
					//Search model name and if true sets glow, must be a better way to do this.. if only i got the item id to work..
					if (lightbackpack && strstr(glowName, "mdl/humans_r5/loot/w_loot_char_backpack_light.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					
					}
					else 
					{
						apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 0);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 0);
						item.disableGlow();
					}
				
					if (medbackpack && strstr(glowName, "mdl/humans_r5/loot/w_loot_char_backpack_medium.rmdl")) 
					{
						apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 2); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 191 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					
					}
					if (heavybackpack && strstr(glowName, "mdl/humans_r5/loot/w_loot_char_backpack_heavy.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 128 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 128 / itemglowbrightness); // b
					
					}
					//item id would help so much here, cant make them all the same color so went with loba glow for body shield and helmet
					if (shieldupgrade && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_cha_shield_upgrade_body.rmdl")) 
					{
					item.enableGlow();
					}
					if (shieldupgradehead && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_cha_shield_upgrade_head.rmdl")) 
					{
					item.enableGlow();
					}
					if (accelerant && strstr(glowName, "mdl/weapons_r5/loot/w_loot_wep_iso_ultimate_accelerant.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 191 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (phoenix && strstr(glowName, "mdl/weapons_r5/loot/w_loot_wep_iso_phoenix_kit_v1.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 2); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,127 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (item.isBox())
					{
						apex_mem.Write<int>(centity + OFFSET_GLOW_T1, 16256);
						apex_mem.Write<int>(centity + OFFSET_GLOW_T2, 1193322764);
						apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 7);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 2);
						
					}
					
					if (item.isTrap())
					{
						apex_mem.Write<int>(centity + OFFSET_GLOW_T1, 16256);
						apex_mem.Write<int>(centity + OFFSET_GLOW_T2, 1193322764);
						apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 7);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 2);
						
					}
					
					//Gas Trap
					if (strstr(glowName, "mdl/props/caustic_gas_tank/caustic_gas_tank.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (healthlarge && strstr(glowName, "mdl/weapons_r5/loot/w_loot_wep_iso_health_main_large.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 128 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (healthsmall && strstr(glowName, "mdl/weapons_r5/loot/w_loot_wep_iso_health_main_small.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 128 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (shieldbattsmall && strstr(glowName, "mdl/weapons_r5/loot/w_loot_wep_iso_shield_battery_small.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 225 / itemglowbrightness); // b
					}
					if (shieldbattlarge && strstr(glowName, "mdl/weapons_r5/loot/w_loot_wep_iso_shield_battery_large.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 225 / itemglowbrightness); // b
					}
					if (ammosniper && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_ammo_sniper.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 92 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 92 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (ammohc && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_ammo_hc.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 250 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 154 / itemglowbrightness); // b
					}
					if (optic && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_optic_cq_hcog_r1.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (ammosc && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_ammo_sc.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 140 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (ammonrg && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_ammo_nrg.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 154 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 205 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 50 / itemglowbrightness); // b
					}
					if (ammoshotgun && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_ammo_shg.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (lasersight && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_lasersight_v1.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 2); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,127 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (magsniper && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_mag_sniper_v1.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 92 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 92 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (magenergy && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_mag_energy_v1.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 154 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 205 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 50 / itemglowbrightness); // b
					}
					if (stocksniper && strstr(glowName, "mdl/weapons_r5/loot/w_loot_wep_iso_stock_folded_sniper.rmdl")) 
					{
					item.enableGlow();
					}
					if (stockregular && strstr(glowName, "mdl/weapons_r5/loot/w_loot_wep_iso_stock_folded_regular.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 2); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,127 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (shielddown && strstr(glowName, "mdl/weapons_r5/loot/w_loot_wep_iso_shield_down_v1.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 2); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,127 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 255 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (lightammomag && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_mag_v1b.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 140 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (heavyammomag && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_mag_v2b.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 250 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 154 / itemglowbrightness); // b
					}
					if (optic2x && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_optic_cq_hcog_r2.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 191 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (opticholo1x && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_optic_cq_holo_var.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 191 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (opticholo1x2x && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_optic_cq_holo_var_2x.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 191 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (opticthreat && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_optic_cq_threat.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 215 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 32 / itemglowbrightness); // b
					}
					if (optic3x && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_optic_rng_hcog_acgs.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 148 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 211 / itemglowbrightness); // b
					}
					if (optic2x4x && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_optic_rng_aog_var_r1.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 148 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 211 / itemglowbrightness); // b
					}
					if (opticsniper6x && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_optic_sni_dcom.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 191 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (opticsniper4x8x && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_optic_sni_var_talon.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 148 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 211 / itemglowbrightness); // b
					}
					if (opticsniperthreat && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_optic_sni_threat_wyeon.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 215 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 32 / itemglowbrightness); // b
					}
					if (suppressor && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_suppr_v2b.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 2); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,127 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (weaponmod && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_chip.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 2); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,127 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 255 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (shotgunbolt && strstr(glowName, "mdl/weapons_r5/loot/_master/w_loot_wep_mods_mag_v3b.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					//Nades
					if (grenade_frag && strstr(glowName, "mdl/weapons/grenades/w_loot_m20_f_grenade_projectile.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					
					if (grenade_thermite && strstr(glowName, "mdl/Weapons/grenades/w_thermite_grenade.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (grenade_arc_star && strstr(glowName, "mdl/weapons_r5/loot/w_loot_wep_iso_shuriken.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					//Weapons
					if (weapon_kraber && strstr(glowName, "mdl/weapons/at_rifle/w_at_rifle.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 255 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (weapon_mastiff && strstr(glowName, "mdl/weapons/mastiff_stgn/w_mastiff.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (weapon_lstar && strstr(glowName, "mdl/weapons/lstar/w_lstar.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 154 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 205 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 50 / itemglowbrightness); // b
					}
					if (weapon_havoc && strstr(glowName, "mdl/Weapons/beam_ar/w_beam_ar.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 154 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 205 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 50 / itemglowbrightness); // b
					}
					if (weapon_devotion && strstr(glowName, "mdl/weapons/hemlock_br/w_hemlock_br.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 154 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 205 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 50 / itemglowbrightness); // b
					}
					if (weapon_triple_take && strstr(glowName, "mdl/weapons/doubletake/w_doubletake.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 154 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 205 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 50 / itemglowbrightness); // b
					}
					if (weapon_flatline  && strstr(glowName, "mdl/techart/mshop/weapons/class/assault/flatline/flatline_base_w.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 250 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 154 / itemglowbrightness); // b
					}
					if (weapon_hemlock && strstr(glowName, "mdl/weapons/m1a1_hemlok/w_hemlok.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 250 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 154 / itemglowbrightness); // b
					}
					if (weapon_g7_scout && strstr(glowName, "mdl/weapons/g2/w_g2a4.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 140 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (weapon_alternator && strstr(glowName, "mdl/weapons/alternator_smg/w_alternator_smg.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 140 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (weapon_r99 && strstr(glowName, "mdl/weapons/r97/w_r97.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 140 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (weapon_prowler && strstr(glowName, "mdl/Weapons/prowler_smg/w_prowler_smg.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 250 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 154 / itemglowbrightness); // b
					}
					if (weapon_volt && strstr(glowName, "mdl/weapons/hemlok_smg/w_hemlok_smg.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 154 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 205 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 50 / itemglowbrightness); // b
					}
					if (weapon_longbow && strstr(glowName, "mdl/weapons/rspn101_dmr/w_rspn101_dmr.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 92 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 92 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (weapon_charge_rifle && strstr(glowName, "mdl/weapons/defender/w_defender.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 92 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 92 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (weapon_spitfire && strstr(glowName, "mdl/weapons/lmg_hemlok/w_lmg_hemlok.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 140 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (weapon_r301 && strstr(glowName, "mdl/weapons/rspn101/w_rspn101.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 140 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (weapon_eva8 && strstr(glowName, "mdl/weapons/w1128/w_w1128.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (weapon_peacekeeper && strstr(glowName, "mdl/weapons/peacekeeper/w_peacekeeper.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (weapon_mozambique && strstr(glowName, "mdl/weapons/pstl_sa3/w_pstl_sa3.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 0 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (weapon_wingman && strstr(glowName, "mdl/weapons/b3wing/w_b3wing.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 92 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 92 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (weapon_p2020 && strstr(glowName, "mdl/weapons/p2011/w_p2011.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 140 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (weapon_re45 && strstr(glowName, "mdl/weapons/p2011_auto/w_p2011_auto.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 140 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (weapon_sentinel && strstr(glowName, "mdl/Weapons/sentinel/w_sentinel.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 92 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 92 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 255 / itemglowbrightness); // b
					}
					if (weapon_bow && strstr(glowName, "mdl/Weapons/compound_bow/w_compound_bow.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 255 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 255 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 0 / itemglowbrightness); // b
					}
					if (weapon_3030_repeater && strstr(glowName, "mdl/weapons/3030repeater/w_3030repeater.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 250 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 154 / itemglowbrightness); // b
					}
					if (weapon_rampage && strstr(glowName, "mdl/techart/mshop/weapons/class/lmg/dragon/dragon_base_w.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 250 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 154 / itemglowbrightness); // b
					}
					if (weapon_car_smg && strstr(glowName, "mdl/techart/mshop/weapons/class/smg/car/car_base_w.rmdl")) 
					{
					apex_mem.Write<int>(centity + OFFSET_GLOW_ENABLE, 1);
						apex_mem.Write<int>(centity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1 = far, 2 = close
						apex_mem.Write<GlowMode>(centity + GLOW_START_TIME, { 101,101,99,90 });
 
						apex_mem.Write<float>(centity + GLOW_COLOR_R, 0 / itemglowbrightness); // r
						apex_mem.Write<float>(centity + GLOW_COLOR_G, 250 / itemglowbrightness); // g
						apex_mem.Write<float>(centity + GLOW_COLOR_B, 154 / itemglowbrightness); // b
					}
					
					

						
					
				}
				k=1;
				//Change the 60 ms to lower to make the death boxes filker less.
				std::this_thread::sleep_for(std::chrono::milliseconds(60));
			}
			else
			{		
				if(k==1)
				{
					//same and the ents above to turn the glow off
					for (int i = 0; i < 15000; i++)
					{
						uint64_t centity = 0;
						apex_mem.Read<uint64_t>(entitylist + ((uint64_t)i << 5), centity);
						if (centity == 0) continue;
 
						Item item = getItem(centity);
 
						if(item.isItem() && item.isGlowing())
						{
							item.disableGlow();
						}
					}
					k=0;
				}
			}	
		}
	}
	item_t = false;
}


int main(int argc, char *argv[])
{
	if(geteuid() != 0)
	{
		//run as root..
		printf("Error: %s is not running as root\n", argv[0]);
		return 0;
	}

	const char* cl_proc = "Stagnant.exe";
	const char* ap_proc = "R5Apex.exe";
	//const char* ap_proc = "EasyAntiCheat_launcher.exe";

	//Client "add" offset
	uint64_t add_off = 0x4fcc0; //todo make this auto update..

	std::thread aimbot_thr;
	std::thread esp_thr;
	std::thread actions_thr;
	std::thread itemglow_thr;
	std::thread vars_thr;
	while(active)
	{
		if(apex_mem.get_proc_status() != process_status::FOUND_READY)
		{
			if(aim_t)
			{
				aim_t = false;
				esp_t = false;
				actions_t = false;
				item_t = false;
				g_Base = 0;

				aimbot_thr.~thread();
				esp_thr.~thread();
				actions_thr.~thread();
				itemglow_thr.~thread();
			}

			std::this_thread::sleep_for(std::chrono::seconds(1));
			printf("Searching for apex process...\n");

			apex_mem.open_proc(ap_proc);

			if(apex_mem.get_proc_status() == process_status::FOUND_READY)
			{
				g_Base = apex_mem.get_proc_baseaddr();
				printf("\nApex process found\n");
				printf("Base: %lx\n", g_Base);

				aimbot_thr = std::thread(AimbotLoop);
				esp_thr = std::thread(EspLoop);
				actions_thr = std::thread(DoActions);
				itemglow_thr = std::thread(item_glow_t);
				aimbot_thr.detach();
				esp_thr.detach();
				actions_thr.detach();
				itemglow_thr.detach();
			}
		}
		else
		{
			apex_mem.check_proc();
		}

		if(client_mem.get_proc_status() != process_status::FOUND_READY)
		{
			if(vars_t)
			{
				vars_t = false;
				c_Base = 0;

				vars_thr.~thread();
			}
			
			std::this_thread::sleep_for(std::chrono::seconds(1));
			printf("Searching for client process...\n");

			client_mem.open_proc(cl_proc);

			if(client_mem.get_proc_status() == process_status::FOUND_READY)
			{
				c_Base = client_mem.get_proc_baseaddr();
				printf("\nClient process found\n");
				printf("Base: %lx\n", c_Base);

				vars_thr = std::thread(set_vars, c_Base + add_off);
				vars_thr.detach();
			}
		}
		else
		{
			client_mem.check_proc();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}
