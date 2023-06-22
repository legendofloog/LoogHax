bool DidUnitBreak();
void BreakPostBattle();
void ClearActiveFactionBreakStatus();

typedef struct BreakEntry BreakEntry;

struct BreakEntry
{
	u8 breakerWType;
	u8 brokenWType;
};

extern struct BreakEntry BreakTargetTable[];

extern u8 BreakExemptCharacterList[];
extern u8 BreakExemptClassList[];

//need these for break display

bool CheckEventId(int eventId);
void UnsetEventId(int eventId);
void SetEventId(int eventId);

//skillsys stuff
extern u8 DazzleIDLink;

extern bool(*gSkillTester)(Unit* unit, int skillID);


#define SKILLSTATE_BREAK (1 << 2)
#define SKILLSTATE_BROKEN_IN_BATTLE (1 << 3)
#define BATTLE_HIT_BREAK BATTLE_HIT_ATTR_12 // (1 << 17)