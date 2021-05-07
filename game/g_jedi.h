//::Pridkett

/*
* Highlander Quake 2
* Main Header File
* Author(s): Pridkett (pridkett@null.net)
*
* Last Revision:   1/1/98 - Wow...the sword actually works now
*                  12/29/97 - Initial Code
*/


/*
* sword variable definitions, I have this here because the sword is most likely still unbalanced
* I know this because I killed a super tank with it without taking ANY damaga
*/
#define SWORD_NORMAL_DAMAGE 100
#define SWORD_DEATHMATCH_DAMAGE 150
#define SWORD_KICK 500
#define SWORD_RANGE 35

/*
=============
fire_sword

attacks with the beloved sword of the highlander
edict_t *self - entity producing it, yourself
vec3_t start - The place you are
vec3_t aimdir - Where you are looking at in this case
int damage - the damage the sword inflicts
int kick - how much you want that bitch to be thrown back
=============
*/

void fire_sword(edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick)
{
	//You may recognize a lot of this from the fire lead command, which
	//is the one that I understood best what the hell was going on

	trace_t tr;             //Not entirely sure what this is, I know that it is used
	//to trace out the route of the weapon being used...gotta limit it
	// Used to figure out what is in front of you--ie, what your aiming at

	vec3_t          dir;            //Another point I am unclear about
	vec3_t          forward;        //maybe someday I will know a little bit
	vec3_t          right;          //better about what these are // the vector pointing right from dir
	vec3_t          up;
	vec3_t          end;

	/* Most of the above vectors are used in fire_lead to calculate the
	* horozontal and vertical spread associated with the machine-guns.
	* Since we don't use spread with the sword, we don't need many of
	* those vectors.                                                   */

	// Figure out what we hit, if anything:

	VectorMA(start, SWORD_RANGE, aimdir, end);  //calculates the range vector                      

	tr = gi.trace(self->s.origin, NULL, NULL, end, self, MASK_SHOT);
	// figuers out what in front of the player up till "end"

	// Figure out what to do about what we hit, if anything


	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_SCREEN_SPARKS);
	gi.WritePosition(self->s.origin);
	gi.WriteDir(aimdir);


	//The fire_lead had an awful lot of stuff in here dealing with the effect of the shot
	//upon water and whatnot, but a sword doesn't make you worry about that sort of stuff
	//thats why highlanders are so damn cool.

	if (!((tr.surface) && (tr.surface->flags & SURF_SKY))) // make sure we didn't hit the sky
	{
		if (tr.fraction < 1.0) // make sure we didn't get out of range
		{
			if (tr.ent->takedamage)
			{
				//This tells us to damage the thing that in our path...hehe
				//ML - added mod parameter
				T_Damage(tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal, damage, kick, 0, 66);
				gi.sound(self, CHAN_AUTO, gi.soundindex("misc/fhit3.wav"), 1, ATTN_NORM, 0);
				// Sound if hit player/monster -- added 1-13-98 by DanE
			}
			else
			{
				if (strncmp(tr.surface->name, "sky", 3) != 0) //check (AGAIN!) if we hit the sky
				{
					gi.WriteByte(svc_temp_entity);
					gi.WriteByte(TE_SPARKS);	//Changed 1-13-98 by DanE to make impact look like sparks
					gi.WritePosition(tr.endpos);
					gi.WriteDir(tr.plane.normal);
					gi.multicast(tr.endpos, MULTICAST_PVS);

					/*
					if (self->client)
						PlayerNoise(self, tr.endpos, PNOISE_IMPACT);
					*/
					gi.sound(self, CHAN_AUTO, gi.soundindex("weapons/grenlb1b.wav"), 1, ATTN_NORM, 0);
					//Sound if hit wall  -- added 1-13-98 by DanE
				}
			}
		}
	}
	return;
}

void sword_attack(edict_t *ent, vec3_t g_offset, int damage)
{
	vec3_t  forward, right;
	vec3_t  start;
	vec3_t  offset;

	if (is_quad)
		damage *= 4;
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight - 8);
	VectorAdd(offset, g_offset, offset);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	fire_sword(ent, start, forward, damage, SWORD_KICK);

}

void Weapon_Sword_Fire(edict_t *ent)
{
	int damage;
	if (deathmatch->value)
		damage = SWORD_DEATHMATCH_DAMAGE;
	else
		damage = SWORD_NORMAL_DAMAGE;
	sword_attack(ent, vec3_origin, damage);
	ent->client->ps.gunframe++;
}

void Weapon_Sword(edict_t *ent)
{
	static int      pause_frames[] = { 19, 32, 0 };
	static int      fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 8, 52, 55, pause_frames, fire_frames, Weapon_Sword_Fire);
}

//
//!Pridkett
/*

================
Use_Weapon

Make the weapon ready if there is ammo
================
*/
void Lightning_Attack(edict_t *ent, gitem_t *item)
{
	
}