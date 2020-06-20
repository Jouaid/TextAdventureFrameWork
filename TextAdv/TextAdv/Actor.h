#pragma once
#include "Input.h"
#include <string>

class Actor
{

protected:
  const char* name;
  int level;
  int baseDmg;
  int currentHP;
  int maximumHP;
  int xpToLevelUp = 0;
  int dmgModifier;

public:
  void SetActorStats(const char* charName, int lvl, int maxHP, int dmg, int dModifier)
  {
    name = charName;
    level = lvl;
    currentHP = maxHP;
    maximumHP = maxHP;
    baseDmg = dmg;
    dmgModifier = dmgModifier;
  };

  void CheckForLevelUp()
  {
    if (xpToLevelUp >= 100)
    {
      xpToLevelUp = 0;
      level++;
      baseDmg += dmgModifier;
      currentHP = maximumHP;

      Input::PrintSentence("YOU HAVE LEVELD UP");
      Input::PrintSentence("LEVEL: ", std::to_string(level));
      Input::PrintSentence("HP: ", std::to_string(currentHP));
      Input::PrintSentence("DMG: ", std::to_string(baseDmg));
    }
  }

  void Heal(int amount)
  {
    currentHP += amount;

    if (currentHP > maximumHP)
      currentHP = maximumHP;

    Input::PrintSentence("Health is at: ", std::to_string(GetHP()));
  }

  void TakeDamage(int amount)
  {
    currentHP -= amount;

    if (currentHP <= 0)
    {
      Input::PrintSentence(GetName(), "Is Dead");
    }
  }

  void XpGained(int amount)
  {
    xpToLevelUp += amount;
    CheckForLevelUp();
  }

  const char* GetName() { return name; }

  int GetLevel() { return level; }

  int GetHP() { return currentHP; }

  int GetDmg() { return baseDmg; }

};

